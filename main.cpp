#include <bits/stdc++.h>
#include "InputLP.h"
#include "ColoredString.h"
#include "Dual.h"
#include "Simplex.h"

using namespace std;


int main() {
	auto input = InputLP();
	LP lp = input.input();
	bool dual;
	cout << ColoredString::white("Please input your work:")
	     << ColoredString::red("(dual enter 1 nad simplex enter 0)")
	     << endl;
	cin >> dual;
	if(dual) {
		Dual d(lp);
	} else {
		Simplex s(lp);
	}
}