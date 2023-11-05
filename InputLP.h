#ifndef LP_INPUTLP_H
#define LP_INPUTLP_H

#include <bits/stdc++.h>
#include "LP.h"

using namespace std;

class InputLP {
public:
	InputLP() = default;

	LP input();

private:
	void input_number_of_x();

	void input_number_of_line();

	void input_type_of_optimization();

	void input_z();

	void input_lines();

	void input_line(size_t row_number, LP::TableType & table, LP::ComparativesType & comparatives, LP::RHSesType & b);

	void input_signs();

	LP lp{};
};

#endif
