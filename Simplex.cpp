#include <bits/stdc++.h>

#include "ColoredString.h"
#include "InputLP.h"
#include "LP.h"

using namespace std;

typedef double ValueOfX;
typedef vector<ValueOfX> ListOfX;
typedef function<ValueOfX(const ListOfX &)> Transformer;
typedef vector<Transformer> Transformers;

LP creat_std_lp(const LP & lp, Transformers & transformers, size_t & number_of_s) {
	size_t number_of_free_var = 0;
	for(auto & sign: lp.get_signs())
		number_of_free_var += (sign == LP::Sign::freeSign);

	number_of_s = 0;
	for(auto & comparative: lp.get_comparatives())
		number_of_s += (comparative != LP::Comparative::equal);

	LP new_lp;
	size_t number_of_x = lp.get_number_of_x() + number_of_free_var + number_of_s;
	new_lp.set_number_of_x(number_of_x);
	new_lp.set_number_of_line(lp.get_number_of_line());

	/// Creat @c LP::z,@c LP::table and creat  @c transformers
	LP::ZType z = new_lp.get_z();
	LP::TableType table = new_lp.get_table();
	transformers.resize(lp.get_number_of_x());

	/// create each var to positive
	size_t index_of_x_in_new_lp = 0;
	for(size_t index_of_x_in_last_lp = 0;
		index_of_x_in_last_lp < lp.get_number_of_x(); ++index_of_x_in_new_lp, ++index_of_x_in_last_lp) {
		switch(lp.sign_at(index_of_x_in_last_lp)) {
			case LP::freeSign: {
				// create a new x and set first positive of value and second value negative
				for(size_t index_of_row = 0; index_of_row < lp.get_number_of_line(); ++index_of_row) {
					table[index_of_row][index_of_x_in_new_lp] = lp.table_at(index_of_row, index_of_x_in_last_lp);
					table[index_of_row][index_of_x_in_new_lp + 1] = -lp.table_at(index_of_row, index_of_x_in_last_lp);
				}
				z[index_of_x_in_new_lp] = lp.z_at(index_of_x_in_last_lp);
				z[index_of_x_in_new_lp + 1] = -lp.z_at(index_of_x_in_last_lp);
				transformers[index_of_x_in_last_lp] = [index_of_x_in_new_lp](const ListOfX & list) -> ValueOfX {
					return list[index_of_x_in_new_lp] - list[index_of_x_in_new_lp + 1];
				};
				++index_of_x_in_new_lp;
				break;
			}
			case LP::negative:
			case LP::positive: {
				// If negative use negative value or not use positive value
				int sign = lp.sign_at(index_of_x_in_last_lp) == LP::negative? -1: 1;
				for(size_t index_of_row = 0; index_of_row < lp.get_number_of_line(); ++index_of_row)
					table[index_of_row][index_of_x_in_new_lp] = sign * lp.table_at(index_of_row, index_of_x_in_last_lp);

				z[index_of_x_in_new_lp] = sign * lp.z_at(index_of_x_in_last_lp);
				transformers[index_of_x_in_last_lp] = [index_of_x_in_new_lp, sign](const ListOfX & list) {
					return sign * list[index_of_x_in_new_lp];
				};
				break;
			}
		}
	}
	new_lp.set_z(z);
	new_lp.set_signs(LP::SignsType(number_of_x, LP::Sign::positive));

	/// change each @c b to positive
	LP::RHSesType rhs = new_lp.get_rhs();
	LP::ComparativesType comparatives = lp.get_comparatives();
	for(size_t index_of_row = 0; index_of_row < new_lp.get_number_of_line(); ++index_of_row) {
		if(lp.rhs_at(index_of_row) >= 0)
			rhs[index_of_row] = lp.rhs_at(index_of_row);
		else {
			rhs[index_of_row] = -lp.rhs_at(index_of_row);
			for(LP::CellOfTable & cell: table[index_of_row])
				cell = -cell;
			switch(comparatives[index_of_row]) {
				case LP::Comparative::lower: comparatives[index_of_row] = LP::Comparative::greater;
					break;
				case LP::Comparative::equal:break;
				case LP::Comparative::greater: comparatives[index_of_row] = LP::Comparative::lower;
					break;
			}
		}
	}
	new_lp.set_rhs(rhs);

	/// change each @c comparatives to @c LP::Comparative::equal
	const size_t index_last_of_x = lp.get_number_of_x() + number_of_free_var;
	size_t index_of_s = 0;
	for(size_t index_of_row = 0; index_of_row < new_lp.get_number_of_line(); ++index_of_row) {
		if(comparatives[index_of_row] == LP::Comparative::equal)
			continue;
		/// because @c LP::Comparative::lower equals -1 and @c LP::Comparative::positive equals 1
		table[index_of_row][index_last_of_x + index_of_s] = -comparatives[index_of_row];
		comparatives[index_of_row] = LP::Comparative::equal;
		++index_of_s;
	}
	new_lp.set_comparatives(comparatives);
	new_lp.set_table(table);

	return new_lp;
}

int main() {
	//	LP lp = InputLP().input();
	LP lp(3, 2, LP::TypeOfOptimization::max, {0, 1, 1}, {{1, 1, 1},
	                                                     {2, 2, 0}}, {1, 2},
		{LP::Comparative::lower, LP::Comparative::equal},
		{LP::Sign::negative, LP::Sign::positive, LP::Sign::freeSign});

	Transformers transformers;
	size_t number_of_s;
	LP new_lp = creat_std_lp(lp, transformers, number_of_s);

}