#include <bits/stdc++.h>

using namespace std;

string red(const string &s) { return "\033[1;31m" + s + "\033[0m"; }

string green(const string &s) { return "\033[1;32m" + s + "\033[0m"; }

string yellow(const string &s) { return "\033[1;33m" + s + "\033[0m"; }

string blue(const string &s) { return "\033[1;34m" + s + "\033[0m"; }

string magenta(const string &s) { return "\033[1;35m" + s + "\033[0m"; }

string white(const string &s) { return "\033[1;37m" + s + "\033[0m"; }

enum TypeOfOptimization {
	min = 0, max = 1
};

string to_string(TypeOfOptimization type) {
	switch (type) {
		case TypeOfOptimization::min:
			return "min";
		case TypeOfOptimization::max:
			return "max";
		default:
			return "";
	}
}

std::istream &operator>>(std::istream &is, TypeOfOptimization &type) {
	int a;
	while (true) {
		is >> a;
		if (a > 1 or a < 0) {
			cout << "your input isn't correct try again:" << flush;
			continue;
		}
		type = static_cast<TypeOfOptimization>(a);
		break;
	}
	return is;
}

enum Sign {
	negative = -1,
	freeSign = 0,
	positive = 1
};

string to_string(Sign s) {
	switch (s) {
		case Sign::negative:
			return "<= 0";
		case Sign::positive:
			return ">= 0";
		case Sign::freeSign:
			return "free";
		default:
			return "";

	}
}

std::istream &operator>>(std::istream &is, Sign &sign) {
	int a;
	is >> a;
	if (a < -1 or a > 1)
		throw std::runtime_error("sign is not a valid");

	sign = static_cast<Sign>(a);
	return is;
}

enum Comparative {
	lower = -1,
	equal = 0,
	greater = 1
};

string to_string(Comparative a) {
	switch (a) {
		case Comparative::lower:
			return "<=";
		case Comparative::equal:
			return " =";
		case Comparative::greater:
			return ">=";
		default:
			return "";
	}
}

std::istream &operator>>(std::istream &is, Comparative &type) {
	string a;
	is >> a;
	if (a == "<=")
		type = Comparative::lower;
	else if (a == "=")
		type = Comparative::equal;
	else if (a == ">=")
		type = Comparative::greater;
	else
		throw std::runtime_error("Comparative is not a valid" + a);

	return is;
}

template<typename T>
string to_string(const string &varname, T arr, size_t n, bool show_empty, const vector<size_t> &max_num_len) {
	const string VAR = varname + "_";
	size_t MAX_LEN_OF_VAR = (to_string(n) + VAR).size();
	string r;
	bool exits_before = false;

	auto generate_empty = [MAX_LEN_OF_VAR, show_empty, max_num_len](int colum_index) -> string {
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

void print(const string &varname, size_t number_of_x, TypeOfOptimization type_of_optimization, int64_t *z,
           size_t number_of_line, const vector<vector<int64_t>> &table, vector<Comparative> as, int64_t *b,
           vector<Sign> signs) {
	cout << magenta(to_string(type_of_optimization)) << " "
	     << red(to_string(varname, z, number_of_x, false, vector(number_of_x, 0UL))) << endl;

	for (int i = 0; i < number_of_line; ++i)
		cout << yellow(to_string(varname, table[i], number_of_x, true, max2DVec(table))) << " "
		     << green(to_string(as[i])) << " " << yellow(to_string(b[i])) << endl;

	map<Sign, vector<size_t>> sign;
	for (size_t i = 0; i < number_of_x; i++)
		sign[signs[i]].emplace_back(i);

	const string VAR = varname + "_";

	string s;
	for (const auto &s1: sign) {
		s = "";
		for (const auto &s2: s1.second)
			s += VAR + to_string(s2 + 1) + ", ";
		s = s.substr(0, s.size() - 2);
		cout << magenta(s) << " " << magenta(to_string(s1.first)) << endl;
	}
}

vector<vector<int64_t>> T(const vector<vector<int64_t>> &v) {
	vector<vector<int64_t>> r(v[0].size(), vector<int64_t>(v.size()));
	for (int i = 0; i < v.size(); ++i)
		for (int j = 0; j < v[i].size(); ++j)
			r[j][i] = v[i][j];
	return r;
}

vector<Sign> ComparativeToSign(const vector<Comparative> &as, size_t num, TypeOfOptimization type_of_optimization) {
	vector<Sign> r(num);
	for (int i = 0; i < num; ++i) {
		switch (as[i]) {
			case Comparative::lower:
				if (type_of_optimization == TypeOfOptimization::max)
					r[i] = Sign::positive;
				if (type_of_optimization == TypeOfOptimization::min)
					r[i] = Sign::negative;
				break;
			case Comparative::equal:
				r[i] = Sign::freeSign;
				break;
			case Comparative::greater:
				if (type_of_optimization == TypeOfOptimization::max)
					r[i] = Sign::negative;
				if (type_of_optimization == TypeOfOptimization::min)
					r[i] = Sign::positive;
				break;
		}
	}
	return r;
}

vector<Comparative> SignToComparative(const vector<Sign> &r, size_t num, TypeOfOptimization type_of_optimization) {
	vector<Comparative> comparatives(num);
	for (int i = 0; i < num; ++i) {
		switch (r[i]) {
			case Sign::negative:
				if (type_of_optimization == TypeOfOptimization::max)
					comparatives[i] = Comparative::lower;
				if (type_of_optimization == TypeOfOptimization::min)
					comparatives[i] = Comparative::greater;
				break;
			case Sign::freeSign:
				comparatives[i] = Comparative::equal;
				break;
			case Sign::positive:
				if (type_of_optimization == TypeOfOptimization::max)
					comparatives[i] = Comparative::greater;
				if (type_of_optimization == TypeOfOptimization::min)
					comparatives[i] = Comparative::lower;
				break;
		}
	}
	return comparatives;
}

TypeOfOptimization change(TypeOfOptimization t) {
	switch (t) {
		case TypeOfOptimization::min:
			return TypeOfOptimization::max;
		case TypeOfOptimization::max:
			return TypeOfOptimization::min;
	}
	throw std::runtime_error("TypeOfOptimization is not correct!");
}

int main() {
	size_t number_of_x;
	cout << white("Pleases enter number of variables:") << flush;
	cin >> number_of_x;

	TypeOfOptimization type_of_optimization;
	cout << white("Pleases enter min or max: ") << red("(min enter 0 and max 1)") << flush;
	cin >> type_of_optimization;

	int64_t z[number_of_x];
	cout << white("Pleases enter Z: ") << red("(for input 2x_1 + 3 x_3 and you have 4 var enter 2 0 3 0)") << flush;
	for (auto &x: z)
		cin >> x;

	size_t number_of_line;
	cout << white("Please enter the number of line:") << flush;
	cin >> number_of_line;

	vector<vector<int64_t>> table(number_of_line);
	int64_t b[number_of_line];
	vector<Comparative> comparatives(number_of_line);
	cout << white("Please enter line:\n")
	     << red("For example:\n")
	     << yellow("x_1 + x_2        + x_4 ") << green(" = ") << yellow("1\n")
	     << yellow("x_1       - 3x_3       ") << green(">= ") << yellow("4\n")
	     << yellow("      x_2        + x_4 ") << green("<= ") << yellow("2\n")
	     << red("you must enter this:\n")
	     << yellow("1 1  0 1 ") << green(" = ") << yellow("1\n")
	     << yellow("1 0 -1 0 ") << green(">= ") << yellow("4\n")
	     << yellow("0 1  0 1 ") << green("<= ") << yellow("2\n")
	     << flush;
	for (int row_number = 0; row_number < number_of_line; ++row_number) {
		auto &row = table[row_number];
		row.resize(number_of_x);
		for (int i = 0; i < number_of_x; ++i)
			cin >> row[i];
		cin >> comparatives[row_number] >> b[row_number];
	}

	vector<Sign> signs(number_of_x);
	cout << white("Please enter sign of x: ") << red("(0 is free, -1 is mean negative, 1 is positive)") << flush;
	for (auto &sign: signs)
		cin >> sign;

	cout << endl << blue("Prime:") << endl;
	print("x", number_of_x, type_of_optimization, z, number_of_line, table, comparatives, b, signs);

	cout << endl << blue("Dual:") << endl;
	print("y", number_of_line, change(type_of_optimization), b, number_of_x, T(table),
	      SignToComparative(signs, number_of_x, type_of_optimization),
	      z, ComparativeToSign(comparatives, number_of_line, type_of_optimization));

}