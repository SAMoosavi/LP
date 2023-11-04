#include "InputLP.h"


string red(const string &s) { return "\033[1;31m" + s + "\033[0m"; }

string green(const string &s) { return "\033[1;32m" + s + "\033[0m"; }

string yellow(const string &s) { return "\033[1;33m" + s + "\033[0m"; }

string blue(const string &s) { return "\033[1;34m" + s + "\033[0m"; }

string magenta(const string &s) { return "\033[1;35m" + s + "\033[0m"; }

string white(const string &s) { return "\033[1;37m" + s + "\033[0m"; }


istream &operator>>(istream &is, InputLP::TypeOfOptimization &type) {
	int a;
	while (true) {
		is >> a;
		if (a > 1 or a < 0) {
			cout << "your input isn't correct try again:" << flush;
			continue;
		}
		type = static_cast<InputLP::TypeOfOptimization>(a);
		break;
	}
	return is;
}

istream &operator>>(istream &is, InputLP::Sign &sign) {
	int a;
	is >> a;
	if (a < -1 or a > 1)
		throw runtime_error("sign is not a valid");

	sign = static_cast<InputLP::Sign>(a);
	return is;
}

istream &operator>>(istream &is, InputLP::Comparative &type) {
	string a;
	is >> a;
	if (a == "<=")
		type = InputLP::Comparative::lower;
	else if (a == "=")
		type = InputLP::Comparative::equal;
	else if (a == ">=")
		type = InputLP::Comparative::greater;
	else
		throw runtime_error("InputLP::Comparative is not a valid" + a);

	return is;
}

InputLP::InputLP() {
	input();
}

string InputLP::to_string(InputLP::TypeOfOptimization type) noexcept {
	switch (type) {
		case TypeOfOptimization::min:
			return "min";
		case TypeOfOptimization::max:
			return "max";
		default:
			return "";
	}
}

string InputLP::to_string(InputLP::Sign s) noexcept {
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

string InputLP::to_string(InputLP::Comparative c) noexcept {
	switch (c) {
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

void InputLP::input() {
	input_number_of_x();
	input_type_of_optimization();
	input_z();
	input_number_of_line();
	input_lines();
	input_signs();
}

void InputLP::input_number_of_x() {
	cout << white("Pleases enter number of variables:") << flush;
	cin >> number_of_x;
}

void InputLP::input_type_of_optimization() {
	cout << white("Pleases enter min or max: ") << red("(min enter 0 and max 1)") << flush;
	cin >> type_of_optimization;
}

void InputLP::input_number_of_line() {
	cout << white("Please enter the number of line:") << flush;
	cin >> number_of_line;
}

void InputLP::input_z() {
	cout << white("Pleases enter Z: ") << red("(for input 2x_1 + 3 x_3 and you have 4 var enter 2 0 3 0)") << flush;
	z.resize(number_of_x);
	for (auto &x: z)
		cin >> x;
}

void InputLP::input_lines() {
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
	table.resize(number_of_line);
	b.resize(number_of_line);
	comparatives.resize(number_of_line);
	for (int row_number = 0; row_number < number_of_line; ++row_number)
		input_line(row_number);
}

void InputLP::input_line(size_t row_number) {
	auto &row = table[row_number];
	row.resize(number_of_x);
	for (auto &cell: row)
		cin >> cell;
	cin >> comparatives[row_number] >> b[row_number];
}

void InputLP::input_signs() {
	cout << white("Please enter sign of x: ") << red("(0 is free, -1 is mean negative, 1 is positive)") << flush;
	signs.resize(number_of_x);
	for (auto &sign: signs)
		cin >> sign;
}

size_t InputLP::get_number_of_x() const noexcept {
	return number_of_x;
}

size_t InputLP::get_number_of_line() const noexcept {
	return number_of_line;
}

InputLP::TypeOfOptimization InputLP::get_type_of_optimization() const noexcept {
	return type_of_optimization;
}

vector<int64_t> InputLP::get_z() const noexcept {
	return z;
}

vector<vector<int64_t>> InputLP::get_table() const noexcept {
	return table;
}

vector<int64_t> InputLP::get_b() const noexcept {
	return b;
}

vector<InputLP::Comparative> InputLP::get_comparatives() const noexcept {
	return comparatives;
}

vector<InputLP::Sign> InputLP::get_signs() const noexcept {
	return signs;
}

