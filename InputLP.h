#ifndef LP_INPUTLP_H
#define LP_INPUTLP_H

#include <bits/stdc++.h>

using namespace std;


string red(const string &s);

string green(const string &s);

string yellow(const string &s);

string blue(const string &s);

string magenta(const string &s);

string white(const string &s);


class InputLP {
public:
	InputLP();

	enum TypeOfOptimization {
		min = 0, max = 1
	};

	static string to_string(TypeOfOptimization type) noexcept;

	enum Sign {
		negative = -1,
		freeSign = 0,
		positive = 1
	};

	static string to_string(Sign s) noexcept;

	enum Comparative {
		lower = -1,
		equal = 0,
		greater = 1
	};

	static string to_string(Comparative c) noexcept;

	void input();

	size_t get_number_of_x() const noexcept;

	size_t get_number_of_line() const noexcept;

	TypeOfOptimization get_type_of_optimization() const noexcept;

	vector<int64_t> get_z() const noexcept;

	vector<vector<int64_t>> get_table() const noexcept;

	vector<int64_t> get_b() const noexcept;

	vector<Comparative> get_comparatives() const noexcept;

	vector<Sign> get_signs() const noexcept;

private:
	void input_number_of_x();

	void input_number_of_line();

	void input_type_of_optimization();

	void input_z();

	void input_lines();

	void input_line(size_t row_number);

	void input_signs();

	size_t number_of_x;
	size_t number_of_line;
	TypeOfOptimization type_of_optimization;
	vector<int64_t> z{};
	vector<vector<int64_t>> table;
	vector<int64_t> b;
	vector<Comparative> comparatives;
	vector<Sign> signs;

};

#endif
