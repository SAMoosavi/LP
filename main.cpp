#include <iostream>

#include "ColoredString.h"
#include "Dual.h"
#include "InputLP.h"
#include "Simplex.h"

int main()
{
	auto input = InputLP();
	LP lp = input.input();
	bool dual;
	std::cout << ColoredString::white("Please input your work:")
	          << ColoredString::red("(dual enter 1 nad simplex enter 0)") << std::endl;
	std::cin >> dual;
	if(dual) {
		std::cout << std::endl << ColoredString::blue("Prime:") << std::endl;
		std::cout << lp.to_string("x") << std::endl;
		Dual d(lp);

		std::cout << std::endl << ColoredString::blue("Dual:") << std::endl;
		std::cout << d.get_dual_lp().to_string("y") << std::endl;
	} else {
		Simplex s(lp);
	}
}