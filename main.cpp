#include <bits/stdc++.h>
#include "InputLP.h"
#include "ColoredString.h"
#include "Dual.h"
#include "Simplex.h"

using namespace std;


int main() {
//	auto input = InputLP();
//	LP lp = input.input();
//	bool dual;
//	cout << ColoredString::white("Please input your work:")
//	     << ColoredString::red("(dual enter 1 nad simplex enter 0)")
//	     << endl;
//	cin >> dual;
//	if(dual) {
//		Dual d(lp);
//	} else {
//		Simplex s(lp);
//	}

	LP lp = {3, 3, LP::TypeOfOptimization::min, {1, 0, -5}, {{1, 2, 100}, {10, 20, 0}, {-5, 0, 2}}, {0, 0, 0},
		{LP::Comparative::equal, LP::Comparative::lower, LP::Comparative::greater},
		{LP::Sign::positive, LP::Sign::negative, LP::Sign::freeSign}};
//	std::cout << lp.to_string("x");
	Simplex d(lp);
}