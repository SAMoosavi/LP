#include <bits/stdc++.h>
#include "InputLP.h"
#include "ColoredString.h"
#include "Dual.h"

using namespace std;


Dual::Dual(LP lp) {
	cout << endl << ColoredString::blue("Prime:") << endl;
	cout << lp.to_string("x") << endl;

	cout << endl << ColoredString::blue("Dual:") << endl;
	dual_lp = {lp.get_number_of_line(), lp.get_number_of_x(),
		change_type_of_optimization(lp.get_type_of_optimization()), lp.get_rhs(), Transpose(lp.get_table()), lp.get_z(),
		SignToComparative(lp.get_signs(), lp.get_number_of_x(), lp.get_type_of_optimization()),
		ComparativeToSign(lp.get_comparatives(), lp.get_number_of_line(), lp.get_type_of_optimization())};
	cout << dual_lp.to_string("y") << endl;
}

/// This function calculates transpose of @c table.
LP::TableType Dual::Transpose(const LP::TableType &table) {
	LP::TableType r(table[0].size(), LP::RowOfTable(table.size()));
	for(int i = 0; i < table.size(); ++i)
		for(int j = 0; j < table[i].size(); ++j)
			r[j][i] = table[i][j];
	return r;
}

/// This function gets comparatives of lines and creates signs of dual variables.
LP::SignsType Dual::ComparativeToSign(const LP::ComparativesType &as, size_t num,
	LP::TypeOfOptimization type_of_optimization) {
	LP::SignsType r(num);
	for(int i = 0; i < num; ++i) {
		switch(as[i]) {
			case LP::Comparative::lower:
				if(type_of_optimization == LP::TypeOfOptimization::max)
					r[i] = LP::Sign::positive;
				if(type_of_optimization == LP::TypeOfOptimization::min)
					r[i] = LP::Sign::negative;
				break;
			case LP::Comparative::equal: r[i] = LP::Sign::freeSign;
				break;
			case LP::Comparative::greater:
				if(type_of_optimization == LP::TypeOfOptimization::max)
					r[i] = LP::Sign::negative;
				if(type_of_optimization == LP::TypeOfOptimization::min)
					r[i] = LP::Sign::positive;
				break;
		}
	}
	return r;
}

/// This function gets sings of variables and creates comparatives of dual lines.
LP::ComparativesType Dual::SignToComparative(const LP::SignsType &r, size_t num,
	LP::TypeOfOptimization type_of_optimization) {
	LP::ComparativesType comparatives(num);
	for(int i = 0; i < num; ++i) {
		switch(r[i]) {
			case LP::Sign::negative:
				if(type_of_optimization == LP::TypeOfOptimization::max)
					comparatives[i] = LP::Comparative::lower;
				else if(type_of_optimization == LP::TypeOfOptimization::min)
					comparatives[i] = LP::Comparative::greater;
				break;
			case LP::Sign::freeSign: comparatives[i] = LP::Comparative::equal;
				break;
			case LP::Sign::positive:
				if(type_of_optimization == LP::TypeOfOptimization::max)
					comparatives[i] = LP::Comparative::greater;
				else if(type_of_optimization == LP::TypeOfOptimization::min)
					comparatives[i] = LP::Comparative::lower;
				break;
		}
	}
	return comparatives;
}

/// This function swaps max to min(or min to max).
LP::TypeOfOptimization Dual::change_type_of_optimization(LP::TypeOfOptimization type) {
	switch(type) {
		case LP::TypeOfOptimization::min: return LP::TypeOfOptimization::max;
		case LP::TypeOfOptimization::max: return LP::TypeOfOptimization::min;
	}
	throw std::runtime_error("LP::TypeOfOptimization is not correct!");
}
