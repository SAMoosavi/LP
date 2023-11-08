#include <bits/stdc++.h>
#include "InputLP.h"
#include "ColoredString.h"
#include "Dual.h"
#include "Simplex.h"

using namespace std;


int main() {
	//	auto input = InputLP();
	//	LP lp = input.input();
	LP lp(3, 2, LP::TypeOfOptimization::max, {0, -1, 1}, {{1, -1, 1},
	                                                     {2, -2, 0}}, {1, -2},
		{LP::Comparative::lower, LP::Comparative::equal},
		{LP::Sign::negative, LP::Sign::positive, LP::Sign::freeSign});

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