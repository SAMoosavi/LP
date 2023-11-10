#include <bits/stdc++.h>
#include "InputLP.h"
#include "ColoredString.h"
#include "Dual.h"
#include "Simplex.h"

using namespace std;


int main() {
	//	auto input = InputLP();
	//	LP lp = input.input();
	LP lp(5, 2, LP::TypeOfOptimization::max, {5, 2, 3, -1, 1}, {{1, 2, 2, 1, 0},
	                                                            {3, 4, 1, 0, 1}}, {8, 7},
		{LP::Comparative::lower, LP::Comparative::greater},
		{LP::Sign::positive, LP::Sign::positive, LP::Sign::positive, LP::Sign::positive, LP::Sign::positive});

	bool dual;
	//	cout << ColoredString::white("Please input your work:")
	//	     << ColoredString::red("(dual enter 1 nad simplex enter 0)")
	//	     << endl;
	//	cin >> dual;
	dual = false;
	if(dual) {
		Dual d(lp);
	} else {
		Simplex s(lp);
	}
}