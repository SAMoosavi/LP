#include <bits/stdc++.h>
#include "InputLP.h"
#include "ColoredString.h"
#include "Dual.h"
#include "Simplex.h"

using namespace std;


int main() {
//	auto input = InputLP();
//	LP lp = input.input();
	LP lp(2, 2, LP::TypeOfOptimization::max, {1, 5}, {{1, 5},
	                                                  {1, 5}}, {1, 2},
		{LP::Comparative::greater, LP::Comparative::lower}, {LP::Sign::freeSign, LP::Sign::negative});

	bool dual= false;
//	cout << ColoredString::white("Please input your work:")
//	     << ColoredString::red("(dual enter 1 nad simplex enter 0)")
//	     << endl;
//	cin >> dual;
	if(dual) {
		Dual d(lp);
	} else {
		Simplex s(lp);
	}
}