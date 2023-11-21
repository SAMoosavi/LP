#ifndef LP_DUAL_H
#define LP_DUAL_H

#include <bits/stdc++.h>
#include "InputLP.h"
#include "ColoredString.h"

using namespace std;

class Dual {
public:
	Dual(LP lp);

private:
	/// This function calculates transpose of @c table.
	LP::TableType Transpose(const LP::TableType &table);

	/// This function gets comparatives of lines and creates signs of dual variables.
	LP::SignsType ComparativeToSign(const LP::ComparativesType &as, size_t num,
		LP::TypeOfOptimization type_of_optimization);

	/// This function gets sings of variables and creates comparatives of dual lines.
	LP::ComparativesType SignToComparative(const LP::SignsType &r, size_t num,
		LP::TypeOfOptimization type_of_optimization);

	/// This function swaps max to min(or min to max).
	LP::TypeOfOptimization change_type_of_optimization(LP::TypeOfOptimization type);

	LP dual_lp;
};

#endif
