#include <bits/stdc++.h>

#include "ColoredString.h"
#include "InputLP.h"
#include "LP.h"
#include "Simplex.h"

using namespace std;

/// This changes polynomial to string.
string to_string(const string &varname, const vector<int64_t> &arr, size_t n, bool show_empty,
	const vector<size_t> &max_num_len) {
	const string VAR = varname + "_";
	size_t MAX_LEN_OF_VAR = (to_string(n) + VAR).size();
	string r;
	bool exits_before = false;

	// if coefficient of a variable is zero should call this function. this calculates number of space.
	auto generate_empty = [MAX_LEN_OF_VAR, show_empty, max_num_len](size_t colum_index) -> string {
		// if show_empty equals to false doesn't need to calculate space
		if(!show_empty)
			return "";
		string r;
		int operator_size = 3;
		if(colum_index == 0)
			operator_size = 0;
		for(size_t i = 0; i < MAX_LEN_OF_VAR + max_num_len[colum_index] + operator_size; ++i)
			r += " ";
		return r;
	};
	// This calculates number of space needed to before of variable.
	auto generate_space = [](const string &s, size_t max_num_len) {
		string r;
		if(max_num_len > s.size())
			for(size_t i = 0; i < max_num_len - s.size(); ++i)
				r += " ";
		return r;
	};

	for(size_t i = 0; i < n; ++i) {
		if(arr[i] == 0)
			r += generate_empty(i);
		else {
			int64_t num;
			if(arr[i] > 0) {
				if(exits_before)
					r += " + ";
				else if(i != 0)
					r += "   ";
				num = arr[i];
			} else if(arr[i] < 0) {
				r += " - ";
				num = -arr[i];
			}
			string num_str = to_string(num);
			if(i != 0)
				r += generate_space(num_str, max_num_len[i]);
			r += num_str;
			r += VAR + to_string(i + 1);
			exits_before = true;
		}
	}
	return r;
}

/**
  * this function calculates maximum length in column.
  * @return list of maximum length in each column.
  */
vector<size_t> max2DVec(const LP::TableType &v) {
	vector<size_t> m(v[0].size(), 0);
	for(const auto &b: v) {
		for(size_t i = 0; i < b.size(); ++i) {
			auto s = to_string(b[i]).size();
			if(b[i] < 0)
				s--;
			if(s > m[i])
				m[i] = s;
		}
	}
	return m;
}

/// this function prints the LP
void print(const string &varname, size_t number_of_x, LP::TypeOfOptimization type_of_optimization, const LP::ZType &z,
	size_t number_of_line, const LP::TableType &table, LP::ComparativesType comparatives,
	const LP::RHSesType &rhSes, LP::SignsType signs) {
	// Z
	cout << ColoredString::magenta(LP::to_string(type_of_optimization)) << " "
	     << ColoredString::red(to_string(varname, z, number_of_x, false, vector<size_t>(number_of_x, 0)))
	     << endl;

	// S.T.
	for(int i = 0; i < number_of_line; ++i)
		cout << ColoredString::yellow(to_string(varname, table[i], number_of_x, true, max2DVec(table))) << " "
		     << ColoredString::green(LP::to_string(comparatives[i])) << " "
		     << ColoredString::yellow(to_string(rhSes[i]))
		     << endl;

	// Signs of variables
	map<LP::Sign, vector<size_t>> sign;
	for(size_t i = 0; i < number_of_x; i++)
		sign[signs[i]].emplace_back(i);

	const string VAR = varname + "_";

	for(const auto &s1: sign) {
		string s;
		for(const auto &s2: s1.second)
			s += VAR + to_string(s2 + 1) + ", ";
		s.pop_back();
		s.pop_back();
		cout << ColoredString::magenta(s) << " " << ColoredString::magenta(LP::to_string(s1.first)) << endl;
	}
}


Simplex::Simplex(LP last_lp) {
	creat_std_lp(last_lp);
	print("x", lp.get_number_of_x(), lp.get_type_of_optimization(), lp.get_z(), lp.get_number_of_line(), lp.get_table(),
		lp.get_comparatives(), lp.get_rhs(), lp.get_signs());


	print("y", lp.get_number_of_x(), lp.get_type_of_optimization(), lp.get_z(), lp.get_number_of_line(),
		lp.get_table(), lp.get_comparatives(), lp.get_rhs(), lp.get_signs());
}

void Simplex::creat_std_lp(const LP &last_lp) {
	size_t number_of_free_var = 0;
	for(auto &sign: last_lp.get_signs())
		number_of_free_var += (sign == LP::Sign::freeSign);

	number_of_s = 0;
	for(auto &comparative: last_lp.get_comparatives())
		number_of_s += (comparative != LP::Comparative::equal);

	size_t number_of_x = last_lp.get_number_of_x() + number_of_free_var + number_of_s;
	lp.set_number_of_x(number_of_x);
	lp.set_number_of_line(last_lp.get_number_of_line());

	/// Creat @c LP::z,@c LP::table and creat  @c transformers
	LP::ZType z = lp.get_z();
	LP::TableType table = lp.get_table();
	transformers.resize(last_lp.get_number_of_x());

	/// create each var to positive
	size_t index_of_x_in_new_lp = 0;
	for(size_t index_of_x_in_last_lp = 0;
		index_of_x_in_last_lp < last_lp.get_number_of_x(); ++index_of_x_in_new_lp, ++index_of_x_in_last_lp) {
		switch(last_lp.sign_at(index_of_x_in_last_lp)) {
			case LP::freeSign: {
				// create a new x and set first positive of value and second value negative
				for(size_t index_of_row = 0; index_of_row < last_lp.get_number_of_line(); ++index_of_row) {
					table[index_of_row][index_of_x_in_new_lp] = last_lp.table_at(index_of_row, index_of_x_in_last_lp);
					table[index_of_row][index_of_x_in_new_lp + 1] = -last_lp.table_at(index_of_row,
						index_of_x_in_last_lp);
				}
				z[index_of_x_in_new_lp] = last_lp.z_at(index_of_x_in_last_lp);
				z[index_of_x_in_new_lp + 1] = -last_lp.z_at(index_of_x_in_last_lp);
				transformers[index_of_x_in_last_lp] = [index_of_x_in_new_lp](const Simplex::ListOfX &list) {
					return list[index_of_x_in_new_lp] - list[index_of_x_in_new_lp + 1];
				};
				++index_of_x_in_new_lp;
				break;
			}
			case LP::negative:
			case LP::positive: {
				// If negative use negative value or not use positive value
				int sign = last_lp.sign_at(index_of_x_in_last_lp) == LP::negative? -1: 1;
				for(size_t index_of_row = 0; index_of_row < last_lp.get_number_of_line(); ++index_of_row)
					table[index_of_row][index_of_x_in_new_lp] =
						sign * last_lp.table_at(index_of_row, index_of_x_in_last_lp);

				z[index_of_x_in_new_lp] = sign * last_lp.z_at(index_of_x_in_last_lp);
				transformers[index_of_x_in_last_lp] = [index_of_x_in_new_lp, sign](const Simplex::ListOfX &list) {
					return sign * list[index_of_x_in_new_lp];
				};
				break;
			}
		}
	}
	lp.set_z(z);
	lp.set_signs(LP::SignsType(number_of_x, LP::Sign::positive));

	/// change each @c b to positive
	LP::RHSesType rhs = lp.get_rhs();
	LP::ComparativesType comparatives = last_lp.get_comparatives();
	for(size_t index_of_row = 0; index_of_row < lp.get_number_of_line(); ++index_of_row) {
		if(last_lp.rhs_at(index_of_row) >= 0)
			rhs[index_of_row] = last_lp.rhs_at(index_of_row);
		else {
			rhs[index_of_row] = -last_lp.rhs_at(index_of_row);
			for(LP::CellOfTable &cell: table[index_of_row])
				cell = -cell;
			switch(comparatives[index_of_row]) {
				case LP::Comparative::lower: comparatives[index_of_row] = LP::Comparative::greater;
					break;
				case LP::Comparative::equal:break;
				case LP::Comparative::greater: comparatives[index_of_row] = LP::Comparative::lower;
					break;
			}
		}
	}
	lp.set_rhs(rhs);

	/// change each @c comparatives to @c LP::Comparative::equal
	const size_t index_last_of_x = last_lp.get_number_of_x() + number_of_free_var;
	size_t index_of_s = 0;
	for(size_t index_of_row = 0; index_of_row < lp.get_number_of_line(); ++index_of_row) {
		if(comparatives[index_of_row] == LP::Comparative::equal)
			continue;
		/// because @c LP::Comparative::lower equals -1 and @c LP::Comparative::positive equals 1
		table[index_of_row][index_last_of_x + index_of_s] = -comparatives[index_of_row];
		comparatives[index_of_row] = LP::Comparative::equal;
		++index_of_s;
	}
	lp.set_comparatives(comparatives);
	lp.set_table(table);
}


int main() {
	//	LP lp = InputLP().input();
	LP lp(3, 2, LP::TypeOfOptimization::max, {0, 1, 1}, {{1, 1, 1},
	                                                     {2, 2, 0}}, {1, 2},
		{LP::Comparative::lower, LP::Comparative::equal},
		{LP::Sign::negative, LP::Sign::positive, LP::Sign::freeSign});

}