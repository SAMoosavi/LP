#include <bits/stdc++.h>

#include "ColoredString.h"
#include "InputLP.h"
#include "LP.h"
#include "Simplex.h"

using namespace std;

/// This changes polynomial to string.
template<typename T>
string to_string(const string &varname, const vector<T> &arr, size_t n, bool show_empty,
	const vector<size_t> &max_num_len) {
	const string VAR = varname + "_";
	size_t MAX_LEN_OF_VAR = (std::to_string(n) + VAR).size();
	string r;
	bool exits_before = false;

	// if coefficient of a variable is zero should call this function. this calculates number of space.
	auto generate_empty = [MAX_LEN_OF_VAR, show_empty, max_num_len](size_t colum_index) -> string {
		// if show_empty equals to false doesn't need to calculate space
		if(!show_empty)
			return "";
		string r;
		int operator_size = 2;
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
			if(arr[i] > 0) {
				if(exits_before)
					r += " +";
				else if(i != 0)
					r += "  ";
			} else if(arr[i] < 0) {
				if(exits_before)
					r += " ";
			}
			string num_str = LP::to_string(arr[i]);
			r += generate_space(num_str, max_num_len[i]);
			r += num_str;
			r += VAR + std::to_string(i + 1);
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
			auto s = LP::to_string(b[i]).size();
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

	// S.Transpose.
	for(int i = 0; i < number_of_line; ++i)
		cout << ColoredString::yellow(to_string(varname, table[i], number_of_x, true, max2DVec(table))) << " "
		     << ColoredString::green(LP::to_string(comparatives[i])) << " "
		     << ColoredString::yellow(LP::to_string(rhSes[i]))
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
	print("x", last_lp.get_number_of_x(), last_lp.get_type_of_optimization(), last_lp.get_z(),
		last_lp.get_number_of_line(), last_lp.get_table(), last_lp.get_comparatives(), last_lp.get_rhs(),
		last_lp.get_signs());

	creat_std_lp(last_lp);

	print("y", lp.get_number_of_x(), lp.get_type_of_optimization(), lp.get_z(), lp.get_number_of_line(),
		lp.get_table(), lp.get_comparatives(), lp.get_rhs(), lp.get_signs());

	made_base();

	print("y", lp.get_number_of_x(), lp.get_type_of_optimization(), lp.get_z(), lp.get_number_of_line(),
		lp.get_table(), lp.get_comparatives(), lp.get_rhs(), lp.get_signs());

	ans();

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

	number_of_x = last_lp.get_number_of_x() + number_of_free_var;
	size_t number_of_x_prime = number_of_x + number_of_s;
	lp.set_number_of_x(number_of_x_prime);
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
						last_lp.table_at(index_of_row, index_of_x_in_last_lp) * sign;

				z[index_of_x_in_new_lp] = last_lp.z_at(index_of_x_in_last_lp) * sign;
				transformers[index_of_x_in_last_lp] = [index_of_x_in_new_lp, sign](const Simplex::ListOfX &list) {
					return sign * list[index_of_x_in_new_lp];
				};
				break;
			}
		}
	}
	lp.set_z(z);
	lp.set_signs(LP::SignsType(number_of_x_prime, LP::Sign::positive));

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

void Simplex::made_base() {
	LP new_lp;
	number_of_r = 0;
	LP::M m;
	switch(lp.get_type_of_optimization()) {
		case LP::min:
			m = LP::M(1, 0);
			break;
		case LP::max:
			m = LP::M(-1, 0);
			break;
	}

	auto z = lp.get_z();
	for(auto &row: lp.get_table()) {
		bool exists_one = false;
		for(size_t i = number_of_x; i < lp.get_number_of_x(); ++i) {
			if(row[i] == 1) {
				exists_one = true;
				break;
			}
		}
		if(!exists_one) {
			++number_of_r;
			z.push_back(m);
		}
	}

	size_t number_of_var = lp.get_number_of_x() + number_of_r;
	new_lp.set_number_of_x(number_of_var);
	new_lp.set_number_of_line(lp.get_number_of_line());
	new_lp.set_comparatives(lp.get_comparatives());
	new_lp.set_signs(LP::SignsType(number_of_var, LP::Sign::positive));
	new_lp.set_comparatives(lp.get_comparatives());
	new_lp.set_rhs(lp.get_rhs());
	new_lp.set_type_of_optimization(lp.get_type_of_optimization());
	new_lp.set_z(z);
	LP::TableType new_table = new_lp.get_table();
	LP::TableType last_table = lp.get_table();
	for(int i = 0; i < last_table.size() ; ++i)
		for(int j = 0; j < last_table[i].size(); ++j)
			new_table[i][j] = last_table[i][j];

	size_t index_of_r = 0;
	for(int i = 0; i <new_table.size(); ++i) {
		bool exists_one = false;
		for(int j = number_of_x; j < new_table[i].size(); ++j) {
			if(new_table[i][j] == 1) {
				exists_one = true;
				break;
			}
		}
		if(!exists_one) {
			new_table[i][number_of_x + number_of_s + index_of_r] = 1;
			++index_of_r;
		}
	}
	new_lp.set_table(new_table);
	lp = new_lp;
}

template<typename T>
T operator*(const vector<T>& a, const vector<T>& b){
	if(a.size() != b.size())
		throw std::runtime_error(ColoredString::red("length of a and b must be equal"));

	T r;
	for(int i = 0; i < a.size(); ++i)
		r = r + (a[i] * b[i]);

	return r;
}

LP::TableType Transpose(const LP::TableType &table) {
	LP::TableType r(table[0].size(), LP::RowOfTable(table.size()));
	for(int i = 0; i < table.size(); ++i)
		for(int j = 0; j < table[i].size(); ++j)
			r[j][i] = table[i][j];
	return r;
}

template<typename T>
ssize_t max(const vector<T> &a) {
	ssize_t max_index = -1;
	T max_val = 0;
	for(ssize_t i = 0; i < a.size(); ++i) {
		if(a[i] > max_val) {
			max_val = a[i];
			max_index = i;
		}
	}
	return max_index;
}

template<typename T>
ssize_t min_test(const vector<T> &column, const vector<T> &rhs) {
	ssize_t min_index = -1;
	T min_val = -1;
	for(ssize_t i = 0; i < column.size(); ++i) {
		if(column[i] > 0) {
			if(min_val == -1) {
				min_val = rhs[i] / column[i];
				min_index = i;
			} else if(rhs[i] / column[i] < min_val) {
				min_val = rhs[i] / column[i];
				min_index = i;
			}
		}
	}
	return min_index;
}

void Simplex::ans() {
	auto table =  lp.get_table();
	for(const auto &row: table) {
		for(size_t i = number_of_x; i < row.size(); ++i) {
			if(row[i] == 1) {
				cj.push_back(i);
				cb.push_back(lp.z_at(i));
			}
		}
	}

	z_bar = cb * lp.get_rhs();
	auto t_table = Transpose(table);
	for(size_t i = 0; i < t_table.size(); ++i)
		c_bar.push_back(lp.z_at(i) - cb * t_table[i]);

	auto rhs = lp.get_rhs();
	ssize_t new_base_column_index = max(c_bar);
	ssize_t new_base_row_index = min_test(t_table[new_base_column_index], rhs);
	while(new_base_column_index > -1 && new_base_row_index > -1) {
		LP::M a = LP::M(1) / t_table[new_base_column_index][new_base_row_index];
		for(auto &cell: table[new_base_row_index])
			cell *= a;

		rhs[new_base_column_index] *= a;

		cj[new_base_row_index] = new_base_column_index;
		cb[new_base_row_index] = lp.z_at(new_base_column_index);

		auto &new_base_row = table[new_base_row_index];
		for(size_t row_index = 0; row_index < table.size(); ++row_index) {
			auto &t = table[row_index];
			if(t == new_base_row)
				continue;

			a = t[new_base_column_index] / new_base_row[new_base_column_index];
			for(int i = 0; i < t.size(); ++i)
				t[i] -= a * new_base_row[i];

			rhs[row_index] -= a * rhs[new_base_column_index];
		}

		a = c_bar[new_base_column_index] / new_base_row[new_base_column_index];
		for(int i = 0; i < c_bar.size(); ++i)
			c_bar[i] -= a * new_base_row[i];

		auto last_z_bar = z_bar;
		z_bar -= a * rhs[new_base_column_index];
//		if(z_bar == last_z_bar)
//			break;

		t_table = Transpose(table);
		new_base_column_index = max(cb);
		new_base_row_index = min_test(t_table[new_base_column_index], rhs);
		lp.set_rhs(rhs);
		lp.set_table(table);
	}
}
