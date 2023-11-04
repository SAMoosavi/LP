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

vector<size_t> max2DVec(const vector<vector<int64_t>> &v) {
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

void print(const string &varname, size_t number_of_x, InputLP::TypeOfOptimization type_of_optimization,
           const vector<int64_t> &z,
           size_t number_of_line, const vector<vector<int64_t>> &table, vector<InputLP::InputLP::Comparative> as,
           const vector<int64_t> &b,
           vector<InputLP::InputLP::Sign> signs) {
	cout << ColoredString::magenta(InputLP::to_string(type_of_optimization)) << " "
	     << ColoredString::red(to_string(varname, z, number_of_x, false, vector<size_t>(number_of_x, 0)))
		 << endl;

	for (int i = 0; i < number_of_line; ++i)
		cout << ColoredString::yellow(to_string(varname, table[i], number_of_x, true, max2DVec(table))) << " "
		     << ColoredString::green(InputLP::to_string(as[i])) << " " << ColoredString::yellow(to_string(b[i]))
		     << endl;

	map<InputLP::Sign, vector<size_t>> sign;
	for (size_t i = 0; i < number_of_x; i++)
		sign[signs[i]].emplace_back(i);

	const string VAR = varname + "_";

	string s;
	for (const auto &s1: sign) {
		s = "";
		for (const auto &s2: s1.second)
			s += VAR + to_string(s2 + 1) + ", ";
		s = s.substr(0, s.size() - 2);
		cout << ColoredString::magenta(s) << " " << ColoredString::magenta(InputLP::to_string(s1.first)) << endl;
	}
}

vector<vector<int64_t>> T(const vector<vector<int64_t>> &v) {
	vector<vector<int64_t>> r(v[0].size(), vector<int64_t>(v.size()));
	for (int i = 0; i < v.size(); ++i)
		for (int j = 0; j < v[i].size(); ++j)
			r[j][i] = v[i][j];
	return r;
}

vector<InputLP::InputLP::Sign> ComparativeToSign(const vector<InputLP::InputLP::Comparative> &as, size_t num,
                                                 InputLP::TypeOfOptimization type_of_optimization) {
	vector<InputLP::InputLP::Sign> r(num);
	for (int i = 0; i < num; ++i) {
		switch (as[i]) {
			case InputLP::Comparative::lower:
				if (type_of_optimization == InputLP::TypeOfOptimization::max)
					r[i] = InputLP::Sign::positive;
				if (type_of_optimization == InputLP::TypeOfOptimization::min)
					r[i] = InputLP::Sign::negative;
				break;
			case InputLP::Comparative::equal:
				r[i] = InputLP::Sign::freeSign;
				break;
			case InputLP::Comparative::greater:
				if (type_of_optimization == InputLP::TypeOfOptimization::max)
					r[i] = InputLP::Sign::negative;
				if (type_of_optimization == InputLP::TypeOfOptimization::min)
					r[i] = InputLP::Sign::positive;
				break;
		}
	}
	return r;
}

vector<InputLP::Comparative>
SignToComparative(const vector<InputLP::Sign> &r, size_t num, InputLP::TypeOfOptimization type_of_optimization) {
	vector<InputLP::Comparative> comparatives(num);
	for (int i = 0; i < num; ++i) {
		switch (r[i]) {
			case InputLP::Sign::negative:
				if (type_of_optimization == InputLP::TypeOfOptimization::max)
					comparatives[i] = InputLP::Comparative::lower;
				if (type_of_optimization == InputLP::TypeOfOptimization::min)
					comparatives[i] = InputLP::Comparative::greater;
				break;
			case InputLP::Sign::freeSign:
				comparatives[i] = InputLP::Comparative::equal;
				break;
			case InputLP::Sign::positive:
				if (type_of_optimization == InputLP::TypeOfOptimization::max)
					comparatives[i] = InputLP::Comparative::greater;
				if (type_of_optimization == InputLP::TypeOfOptimization::min)
					comparatives[i] = InputLP::Comparative::lower;
				break;
		}
	}
	return comparatives;
}

InputLP::TypeOfOptimization change(InputLP::TypeOfOptimization t) {
	switch (t) {
		case InputLP::TypeOfOptimization::min:
			return InputLP::TypeOfOptimization::max;
		case InputLP::TypeOfOptimization::max:
			return InputLP::TypeOfOptimization::min;
	}
	throw std::runtime_error("InputLP::TypeOfOptimization is not correct!");
}

int main() {
	auto input = InputLP();

	cout << endl << ColoredString::blue("Prime:") << endl;
	print("x", input.get_number_of_x(), input.get_type_of_optimization(), input.get_z(), input.get_number_of_line(),
	      input.get_table(), input.get_comparatives(), input.get_b(), input.get_signs());

	cout << endl << ColoredString::blue("Dual:") << endl;
	print("y", input.get_number_of_line(), change(input.get_type_of_optimization()), input.get_b(),
	      input.get_number_of_x(), T(input.get_table()),
	      SignToComparative(input.get_signs(), input.get_number_of_x(), input.get_type_of_optimization()),
	      input.get_z(),
	      ComparativeToSign(input.get_comparatives(), input.get_number_of_line(), input.get_type_of_optimization()));

}