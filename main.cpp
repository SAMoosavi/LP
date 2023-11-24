#include "ColoredString.h"
#include "Dual.h"
#include "InputLP.h"
#include "Simplex.h"

#include <iostream>



int main()
{
		auto input = InputLP();
		LP lp = input.input();
		bool dual;
		std::cout << ColoredString::white("Please input your work:")
		     << ColoredString::red("(dual enter 1 nad simplex enter 0)")
		     << std::endl;
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

//	LP lp = {3,
//	         3,
//	         LP::TypeOfOptimization::min,
//	         {1, 0, -5},
//	         {{1, 2, 100}, {10, 20, 0}, {-5, 0, 2}},
//	         {0, 0, 0},
//	         {LP::Comparative::equal, LP::Comparative::lower, LP::Comparative::greater},
//	         {LP::Sign::positive, LP::Sign::negative, LP::Sign::freeSign}};
//	//	std::cout << lp.to_string("x");
//	Simplex d(lp);
}