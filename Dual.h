#ifndef LP_DUAL_H
#define LP_DUAL_H

#include "LP.h"

class Dual
{
public:
	Dual(LP lp);

	LP get_dual_lp() const noexcept;
	LP get_primal_lp() const noexcept;

private:
	/// This function calculates transpose of @c table.
	LP::TableType calculate_table_of_dual() const noexcept;

	/// This function gets comparatives of lines and creates signs of dual variables.
	LP::SignsType calculate_signs_of_dual() const noexcept;

	/// This function gets sings of variables and creates comparatives of dual lines.
	LP::ComparativesType calculate_comparative_of_dual() const noexcept;

	/// This function swaps max to min(or min to max).
	LP::TypeOfOptimization calculate_type_of_optimization_dual() const noexcept;

	LP dual_lp;
	LP primal_lp;
};

#endif
