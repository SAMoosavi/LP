#include <bits/stdc++.h>
#include "InputLP.h"
#include "ColoredString.h"

using namespace std;

string to_string(const string &varname, const vector<int64_t> &arr, size_t n, bool show_empty,
                 const vector<size_t> &max_num_len) {
	const string VAR = varname + "_";
	size_t MAX_LEN_OF_VAR = (to_string(n) + VAR).size();
	string r;
	bool exits_before = false;

	auto generate_empty = [MAX_LEN_OF_VAR, show_empty, max_num_len](size_t colum_index) -> string {
		if (!show_empty) return "";
		string r;
		int operator_size = 3;
		if (colum_index == 0)
			operator_size = 0;
		for (size_t i = 0; i < MAX_LEN_OF_VAR + max_num_len[colum_index] + operator_size; ++i) r += " ";
		return r;
	};

	auto generate_space = [](const string &s, size_t max_num_len) {
		string r;
		if (max_num_len > s.size())
			for (size_t i = 0; i < max_num_len - s.size(); ++i)
				r += " ";
		return r;
	};

	for (size_t i = 0; i < n; ++i) {
		if (arr[i] == 0)
			r += generate_empty(i);
		else {
			int64_t num;
			if (arr[i] > 0) {
				if (exits_before)
					r += " + ";
				else if (i != 0)
					r += "   ";
				num = arr[i];
			}
			if (arr[i] < 0) {
				r += " - ";
				num = -arr[i];
			}
			string num_str = to_string(num);
			if (i != 0)
				r += generate_space(num_str, max_num_len[i]);
			r += num_str;
			r += VAR + to_string(i + 1);
			exits_before = true;
		}
	}
	return r;
}

vector<size_t> max2DVec(const LP::TableType &v) {
	vector<size_t> m(v[0].size(), 0);
	for (const auto &b: v) {
		for (size_t i = 0; i < b.size(); ++i) {
			auto s = to_string(b[i]).size();
			if (b[i] < 0)
				s--;
			if (s > m[i])
				m[i] = s;
		}
	}
	return m;
}

void print(const string &varname, size_t number_of_x, LP::TypeOfOptimization type_of_optimization, const LP::ZType &z,
           size_t number_of_line, const LP::TableType &table, LP::ComparativesType comparatives,
           const LP::RHSesType &rhSes, LP::SignsType signs) {
	cout << ColoredString::magenta(LP::to_string(type_of_optimization)) << " "
	     << ColoredString::red(to_string(varname, z, number_of_x, false, vector<size_t>(number_of_x, 0)))
	     << endl;

	for (int i = 0; i < number_of_line; ++i)
		cout << ColoredString::yellow(to_string(varname, table[i], number_of_x, true, max2DVec(table))) << " "
		     << ColoredString::green(LP::to_string(comparatives[i])) << " "
		     << ColoredString::yellow(to_string(rhSes[i]))
		     << endl;

	map<LP::Sign, vector<size_t>> sign;
	for (size_t i = 0; i < number_of_x; i++)
		sign[signs[i]].emplace_back(i);

	const string VAR = varname + "_";

	for (const auto &s1: sign) {
		string s;
		for (const auto &s2: s1.second)
			s += VAR + to_string(s2 + 1) + ", ";
		s.pop_back();
		s.pop_back();
		cout << ColoredString::magenta(s) << " " << ColoredString::magenta(LP::to_string(s1.first)) << endl;
	}
}

LP::TableType T(const LP::TableType &v) {
	LP::TableType r(v[0].size(), vector<int64_t>(v.size()));
	for (int i = 0; i < v.size(); ++i)
		for (int j = 0; j < v[i].size(); ++j)
			r[j][i] = v[i][j];
	return r;
}

LP::SignsType
ComparativeToSign(const LP::ComparativesType &as, size_t num, LP::TypeOfOptimization type_of_optimization) {
	LP::SignsType r(num);
	for (int i = 0; i < num; ++i) {
		switch (as[i]) {
			case LP::Comparative::lower:
				if (type_of_optimization == LP::TypeOfOptimization::max)
					r[i] = LP::Sign::positive;
				if (type_of_optimization == LP::TypeOfOptimization::min)
					r[i] = LP::Sign::negative;
				break;
			case LP::Comparative::equal:
				r[i] = LP::Sign::freeSign;
				break;
			case LP::Comparative::greater:
				if (type_of_optimization == LP::TypeOfOptimization::max)
					r[i] = LP::Sign::negative;
				if (type_of_optimization == LP::TypeOfOptimization::min)
					r[i] = LP::Sign::positive;
				break;
		}
	}
	return r;
}

LP::ComparativesType
SignToComparative(const LP::SignsType &r, size_t num, LP::TypeOfOptimization type_of_optimization) {
	LP::ComparativesType comparatives(num);
	for (int i = 0; i < num; ++i) {
		switch (r[i]) {
			case LP::Sign::negative:
				if (type_of_optimization == LP::TypeOfOptimization::max)
					comparatives[i] = LP::Comparative::lower;
				if (type_of_optimization == LP::TypeOfOptimization::min)
					comparatives[i] = LP::Comparative::greater;
				break;
			case LP::Sign::freeSign:
				comparatives[i] = LP::Comparative::equal;
				break;
			case LP::Sign::positive:
				if (type_of_optimization == LP::TypeOfOptimization::max)
					comparatives[i] = LP::Comparative::greater;
				if (type_of_optimization == LP::TypeOfOptimization::min)
					comparatives[i] = LP::Comparative::lower;
				break;
		}
	}
	return comparatives;
}

LP::TypeOfOptimization change_type_of_optimization(LP::TypeOfOptimization t) {
	switch (t) {
		case LP::TypeOfOptimization::min:
			return LP::TypeOfOptimization::max;
		case LP::TypeOfOptimization::max:
			return LP::TypeOfOptimization::min;
	}
	throw std::runtime_error("LP::TypeOfOptimization is not correct!");
}

int main() {
	auto input = InputLP();
	LP lp = input.input();

	cout << endl << ColoredString::blue("Prime:") << endl;
	print("x", lp.get_number_of_x(), lp.get_type_of_optimization(), lp.get_z(), lp.get_number_of_line(), lp.get_table(),
	      lp.get_comparatives(), lp.get_b(), lp.get_signs());

	cout << endl << ColoredString::blue("Dual:") << endl;
	print("y", lp.get_number_of_line(), change_type_of_optimization(lp.get_type_of_optimization()), lp.get_b(),
	      lp.get_number_of_x(), T(lp.get_table()),
	      SignToComparative(lp.get_signs(), lp.get_number_of_x(), lp.get_type_of_optimization()), lp.get_z(),
	      ComparativeToSign(lp.get_comparatives(), lp.get_number_of_line(), lp.get_type_of_optimization()));

}