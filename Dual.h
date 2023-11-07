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
	/// This changes polynomial to string.
	string to_string(const string & varname, const vector<int64_t> & arr, size_t n, bool show_empty,
		const vector<size_t> & max_num_len);


	/**
	  * this function calculates maximum length in column.
	  * @return list of maximum length in each column.
	  */
	vector<size_t> max2DVec(const LP::TableType & v);


	/// this function prints the LP
	void
	print(const string & varname, size_t number_of_x, LP::TypeOfOptimization type_of_optimization, const LP::ZType & z,
		size_t number_of_line, const LP::TableType & table, LP::ComparativesType comparatives,
		const LP::RHSesType & rhSes, LP::SignsType signs);

	/// This function calculates transpose of @c table.
	LP::TableType T(const LP::TableType & table);


	/// This function gets comparatives of lines and creates signs of dual variables.
	LP::SignsType ComparativeToSign(const LP::ComparativesType & as, size_t num,
		LP::TypeOfOptimization type_of_optimization);


	/// This function gets sings of variables and creates comparatives of dual lines.
	LP::ComparativesType SignToComparative(const LP::SignsType & r, size_t num,
		LP::TypeOfOptimization type_of_optimization);


	/// This function swaps max to min(or min to max).
	LP::TypeOfOptimization change_type_of_optimization(LP::TypeOfOptimization type);
};

#endif
