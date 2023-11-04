#include "InputLP.h"
#include "ColoredString.h"

istream &operator>>(istream &is, LP::TypeOfOptimization &type) {
	int a;
	while (true) {
		is >> a;
		if (a > 1 or a < 0) {
			cout << "your input isn't correct try again:" << flush;
			continue;
		}
		type = static_cast<LP::TypeOfOptimization>(a);
		break;
	}
	return is;
}

istream &operator>>(istream &is, LP::Sign &sign) {
	int a;
	is >> a;
	if (a < -1 or a > 1)
		throw runtime_error("sign is not a valid");

	sign = static_cast<LP::Sign>(a);
	return is;
}

istream &operator>>(istream &is, LP::Comparative &type) {
	string a;
	is >> a;
	if (a == "<=")
		type = LP::Comparative::lower;
	else if (a == "=")
		type = LP::Comparative::equal;
	else if (a == ">=")
		type = LP::Comparative::greater;
	else
		throw runtime_error("InputLP::Comparative is not a valid " + a);

	return is;
}

LP InputLP::input() {
	input_number_of_x();
	input_type_of_optimization();
	input_z();
	input_number_of_line();
	input_lines();
	input_signs();
	return lp;
}

void InputLP::input_number_of_x() {
	size_t number_of_x;
	cout << ColoredString::white("Pleases enter number of variables:") << flush;
	cin >> number_of_x;
	lp.set_number_of_x(number_of_x);
}

void InputLP::input_type_of_optimization() {
	LP::TypeOfOptimization type_of_optimization;
	cout << ColoredString::white("Pleases enter min or max: ")
	     << ColoredString::red("(min enter 0 and max 1)")
	     << flush;
	cin >> type_of_optimization;
	lp.set_type_of_optimization(type_of_optimization);
}

void InputLP::input_number_of_line() {
	size_t number_of_line;
	cout << ColoredString::white("Please enter the number of line:") << flush;
	cin >> number_of_line;
	lp.set_number_of_line(number_of_line);
}

void InputLP::input_z() {
	cout << ColoredString::white("Pleases enter Z: ")
	     << ColoredString::red("(for input 2x_1 + 3 x_3 and you have 4 var enter 2 0 3 0)")
	     << flush;
	LP::ZType z(lp.get_number_of_x());
	for (auto &x: z)
		cin >> x;
	lp.set_z(z);
}

void InputLP::input_lines() {
	cout << ColoredString::white("Please enter line:\n")
	     << ColoredString::red("For example:\n")
	     << ColoredString::yellow("x_1 + x_2        + x_4 ")
	     << ColoredString::green(" = ")
	     << ColoredString::yellow("1\n")
	     << ColoredString::yellow("x_1       - 3x_3       ")
	     << ColoredString::green(">= ")
	     << ColoredString::yellow("4\n")
	     << ColoredString::yellow("      x_2        + x_4 ")
	     << ColoredString::green("<= ")
	     << ColoredString::yellow("2\n")
	     << ColoredString::red("you must enter this:\n")
	     << ColoredString::yellow("1 1  0 1 ")
	     << ColoredString::green(" = ")
	     << ColoredString::yellow("1\n")
	     << ColoredString::yellow("1 0 -1 0 ")
	     << ColoredString::green(">= ")
	     << ColoredString::yellow("4\n")
	     << ColoredString::yellow("0 1  0 1 ")
	     << ColoredString::green("<= ")
	     << ColoredString::yellow("2\n")
	     << flush;

	LP::TableType table(lp.get_number_of_line());
	LP::RHSesType b(lp.get_number_of_line());
	LP::ComparativesType comparatives(lp.get_number_of_line());
	for (int row_number = 0; row_number < lp.get_number_of_line(); ++row_number)
		input_line(row_number, table, comparatives, b);
	lp.set_table(table);
	lp.set_b(b);
	lp.set_comparatives(comparatives);
}

void
InputLP::input_line(size_t row_number, LP::TableType &table, LP::ComparativesType &comparatives, LP::RHSesType &b) {
	auto &row = table[row_number];
	row.resize(lp.get_number_of_x());
	for (auto &cell: row)
		cin >> cell;
	cin >> comparatives[row_number] >> b[row_number];
}

void InputLP::input_signs() {
	cout << ColoredString::white("Please enter sign of x: ")
	     << ColoredString::red("(0 is free, -1 is mean negative, 1 is positive)")
	     << flush;
	LP::SignsType signs(lp.get_number_of_x());
	for (auto &sign: signs)
		cin >> sign;
	lp.set_signs(signs);
}
