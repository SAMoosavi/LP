#include "Dual.h"

Dual::Dual(const LP &lp)
: primal_lp(lp)
{
	dual_lp = LP(lp.get_number_of_line(), lp.get_number_of_x(), calculate_type_of_optimization_dual(), lp.get_rhs(),
	             calculate_table_of_dual(), lp.get_z(), calculate_comparative_of_dual(), calculate_signs_of_dual());
}

LP Dual::get_dual_lp() const noexcept
{
	return dual_lp;
}

LP Dual::get_primal_lp() const noexcept
{
	return primal_lp;
}

/// This function calculates transpose of @c table.
LP::TableType Dual::calculate_table_of_dual() const noexcept
{
	LP::TableType table_of_dual(primal_lp.get_number_of_x(), LP::RowOfTable(primal_lp.get_number_of_line()));
	for(int i = 0; i < primal_lp.get_number_of_line(); ++i)
		for(int j = 0; j < primal_lp.get_number_of_x(); ++j)
			table_of_dual[j][i] = primal_lp.table_at(i, j);
	return std::move(table_of_dual);
}

/// This function gets comparatives of lines and creates signs of dual variables.
LP::SignsType Dual::calculate_signs_of_dual() const noexcept
{
	LP::SignsType dual_signs(primal_lp.get_number_of_line());
	for(int i = 0; i < primal_lp.get_comparatives().size(); ++i) {
		switch(primal_lp.get_comparatives()[i]) {
		case LP::Comparative::lower:
			if(primal_lp.get_type_of_optimization() == LP::TypeOfOptimization::max)
				dual_signs[i] = LP::Sign::positive;
			if(primal_lp.get_type_of_optimization() == LP::TypeOfOptimization::min)
				dual_signs[i] = LP::Sign::negative;
			break;
		case LP::Comparative::equal:
			dual_signs[i] = LP::Sign::freeSign;
			break;
		case LP::Comparative::greater:
			if(primal_lp.get_type_of_optimization() == LP::TypeOfOptimization::max)
				dual_signs[i] = LP::Sign::negative;
			if(primal_lp.get_type_of_optimization() == LP::TypeOfOptimization::min)
				dual_signs[i] = LP::Sign::positive;
			break;
		}
	}
	return std::move(dual_signs);
}

/// This function gets sings of variables and creates comparatives of dual lines.
LP::ComparativesType Dual::calculate_comparative_of_dual() const noexcept
{
	LP::ComparativesType dual_comparatives(primal_lp.get_number_of_x());
	for(int i = 0; i < primal_lp.get_signs().size(); ++i) {
		switch(primal_lp.get_signs()[i]) {
		case LP::Sign::negative:
			if(primal_lp.get_type_of_optimization() == LP::TypeOfOptimization::max)
				dual_comparatives[i] = LP::Comparative::lower;
			else if(primal_lp.get_type_of_optimization() == LP::TypeOfOptimization::min)
				dual_comparatives[i] = LP::Comparative::greater;
			break;
		case LP::Sign::freeSign:
			dual_comparatives[i] = LP::Comparative::equal;
			break;
		case LP::Sign::positive:
			if(primal_lp.get_type_of_optimization() == LP::TypeOfOptimization::max)
				dual_comparatives[i] = LP::Comparative::greater;
			else if(primal_lp.get_type_of_optimization() == LP::TypeOfOptimization::min)
				dual_comparatives[i] = LP::Comparative::lower;
			break;
		}
	}
	return std::move(dual_comparatives);
}

/// This function swaps max to min(or min to max).
LP::TypeOfOptimization Dual::calculate_type_of_optimization_dual() const noexcept
{
	int value_of_type = primal_lp.get_type_of_optimization();
	return LP::TypeOfOptimization((value_of_type + 1) % 2);
}
