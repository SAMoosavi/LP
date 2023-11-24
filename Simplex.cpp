#include "Simplex.h"

#include <iostream>
#include <string>
#include <vector>

#include "ColoredString.h"
#include "LP.h"
#include "MNumber.h"

Simplex::Simplex(const LP &last_lp)
{
	creat_std_lp(last_lp);
	made_base();
	if(number_of_r > 0) {
		bool big_m = false;
		std::cout << "use big M or 2 faz?(big m enter 1, 2 faz enter 0)";
		std::cin >> big_m;
		if(big_m) {
			ans();
			print_table();
		} else {
			auto last_z = lp.get_z();
			auto last_type_of_optimization = lp.get_type_of_optimization();

			edit_base();
			ans();
			print_table();

			if(z_bar != 0) {
				std::cout << "R exist in base so don't has answer." << std::endl;
				return;
			}
			number_of_r++;

			auto table = lp.get_table();
			auto rhs = lp.get_rhs();
			auto signs = lp.get_signs();
			auto comparatives = lp.get_comparatives();

			for(auto& row : table)
				row.emplace_back(0);

			table.emplace_back(lp.get_number_of_x() + 1, 0);
			for(size_t i = number_of_x + number_of_s; i < number_of_x + number_of_s + number_of_r ;++i)
				table.back()[i] = 1;

			rhs.emplace_back(0);
			signs.emplace_back(LP::Sign::positive);
			comparatives.emplace_back(LP::Comparative::equal);
			last_z.emplace_back(0);
			auto rb_last_z = last_z.rbegin();
			for(int i = 0; i < number_of_r; ++i,++rb_last_z)
				*rb_last_z = 0;

			lp.set_number_of_x(number_of_x + number_of_s + number_of_r);
			lp.set_number_of_line(lp.get_number_of_line()+1);
			lp.set_z(last_z);
			lp.set_type_of_optimization(last_type_of_optimization);
			lp.set_table(table);
			lp.set_rhs(rhs);
			lp.set_signs(signs);
			lp.set_comparatives(comparatives);

			cj.push_back(lp.get_number_of_x()-1);
			ans();
			print_table();
		}
	} else {
		ans();
		print_table();
	}
	print_ans();
}

void Simplex::creat_std_lp(const LP &last_lp)
{
	lp.set_type_of_optimization(last_lp.get_type_of_optimization());

	size_t number_of_free_var = 0;
	for(auto &sign: last_lp.get_signs())
		number_of_free_var += (sign == LP::Sign::freeSign);

	number_of_s = 0;
	for(auto &comparative: last_lp.get_comparatives())
		number_of_s += (comparative != LP::Comparative::equal);

	number_of_x = last_lp.get_number_of_x() + number_of_free_var;
	size_t number_of_x_prime = number_of_x + number_of_s;
	lp.set_number_of_x(number_of_x_prime);
	lp.set_number_of_line(last_lp.get_number_of_line());

	/// Creat @c LP::z,@c LP::table and creat  @c transformers
	LP::ZType z = lp.get_z();
	LP::TableType table = lp.get_table();
	transformers.resize(last_lp.get_number_of_x());

	/// create each var to positive
	size_t index_of_x_in_new_lp = 0;
	for(size_t index_of_x_in_last_lp = 0; index_of_x_in_last_lp < last_lp.get_number_of_x();
	    ++index_of_x_in_new_lp, ++index_of_x_in_last_lp) {
		switch(last_lp.sign_at(index_of_x_in_last_lp)) {
		case LP::freeSign: {
			// create a new x and set first positive of value and second value negative
			for(size_t index_of_row = 0; index_of_row < last_lp.get_number_of_line(); ++index_of_row) {
				table[index_of_row][index_of_x_in_new_lp] = last_lp.table_at(index_of_row, index_of_x_in_last_lp);
				table[index_of_row][index_of_x_in_new_lp + 1] = -last_lp.table_at(index_of_row, index_of_x_in_last_lp);
			}
			z[index_of_x_in_new_lp] = last_lp.z_at(index_of_x_in_last_lp);
			z[index_of_x_in_new_lp + 1] = -last_lp.z_at(index_of_x_in_last_lp);
			transformers[index_of_x_in_last_lp] = [index_of_x_in_new_lp](const Simplex::ListOfX &list) {
				return list[index_of_x_in_new_lp] - list[index_of_x_in_new_lp + 1];
			};
			++index_of_x_in_new_lp;
			break;
		}
		case LP::negative:
		case LP::positive: {
			// If negative use negative value or not use positive value
			int sign = last_lp.sign_at(index_of_x_in_last_lp) == LP::negative ? -1 : 1;
			for(size_t index_of_row = 0; index_of_row < last_lp.get_number_of_line(); ++index_of_row)
				table[index_of_row][index_of_x_in_new_lp] =
				    last_lp.table_at(index_of_row, index_of_x_in_last_lp) * sign;

			z[index_of_x_in_new_lp] = last_lp.z_at(index_of_x_in_last_lp) * sign;
			transformers[index_of_x_in_last_lp] = [index_of_x_in_new_lp, sign](const Simplex::ListOfX &list) {
				return list[index_of_x_in_new_lp] * sign;
			};
			break;
		}
		}
	}
	lp.set_z(z);
	lp.set_signs(LP::SignsType(number_of_x_prime, LP::Sign::positive));

	/// change each @c rhses to positive
	LP::RHSesType rhs = lp.get_rhs();
	LP::ComparativesType comparatives = last_lp.get_comparatives();
	for(size_t index_of_row = 0; index_of_row < lp.get_number_of_line(); ++index_of_row) {
		if(last_lp.rhs_at(index_of_row) >= 0)
			rhs[index_of_row] = last_lp.rhs_at(index_of_row);
		else {
			rhs[index_of_row] = -last_lp.rhs_at(index_of_row);
			for(LP::CellOfTable &cell: table[index_of_row])
				cell = -cell;
			switch(comparatives[index_of_row]) {
			case LP::Comparative::lower:
				comparatives[index_of_row] = LP::Comparative::greater;
				break;
			case LP::Comparative::equal:
				break;
			case LP::Comparative::greater:
				comparatives[index_of_row] = LP::Comparative::lower;
				break;
			}
		}
	}
	lp.set_rhs(rhs);

	/// change each @c comparatives to @c LP::Comparative::equal
	const size_t index_last_of_x = last_lp.get_number_of_x() + number_of_free_var;
	size_t index_of_s = 0;
	for(size_t index_of_row = 0; index_of_row < lp.get_number_of_line(); ++index_of_row) {
		if(comparatives[index_of_row] == LP::Comparative::equal)
			continue;
		/// because @c LP::Comparative::lower equals -1 and @c LP::Comparative::positive equals 1
		table[index_of_row][index_last_of_x + index_of_s] = -comparatives[index_of_row];
		comparatives[index_of_row] = LP::Comparative::equal;
		++index_of_s;
	}
	lp.set_comparatives(comparatives);
	lp.set_table(table);
}

void Simplex::made_base()
{
	LP new_lp;
	number_of_r = 0;
	LP::Coefficient m;
	switch(lp.get_type_of_optimization()) {
	case LP::min:
		m = LP::Coefficient(1, 0);
		break;
	case LP::max:
		m = LP::Coefficient(-1, 0);
		break;
	}

	auto z = lp.get_z();
	for(auto &row: lp.get_table()) {
		bool exists_one = false;
		for(size_t i = number_of_x; i < lp.get_number_of_x(); ++i) {
			if(row[i] == 1) {
				exists_one = true;
				break;
			}
		}
		if(!exists_one) {
			++number_of_r;
			z.push_back(m);
		}
	}

	size_t number_of_var = lp.get_number_of_x() + number_of_r;
	new_lp.set_number_of_x(number_of_var);
	new_lp.set_number_of_line(lp.get_number_of_line());
	new_lp.set_comparatives(lp.get_comparatives());
	new_lp.set_signs(LP::SignsType(number_of_var, LP::Sign::positive));
	new_lp.set_comparatives(lp.get_comparatives());
	new_lp.set_rhs(lp.get_rhs());
	new_lp.set_type_of_optimization(lp.get_type_of_optimization());
	new_lp.set_z(z);
	LP::TableType new_table = new_lp.get_table();
	LP::TableType last_table = lp.get_table();
	for(int i = 0; i < last_table.size(); ++i)
		for(int j = 0; j < last_table[i].size(); ++j)
			new_table[i][j] = last_table[i][j];

	size_t index_of_r = 0;
	for(int i = 0; i < new_table.size(); ++i) {
		bool exists_one = false;
		for(int j = number_of_x; j < new_table[i].size(); ++j) {
			if(new_table[i][j] == 1) {
				exists_one = true;
				break;
			}
		}
		if(!exists_one) {
			new_table[i][number_of_x + number_of_s + index_of_r] = 1;
			++index_of_r;
		}
	}
	new_lp.set_table(new_table);
	lp = new_lp;
}

void Simplex::edit_base()
{
	lp.set_type_of_optimization(LP::TypeOfOptimization::min);

	LP::ZType z(lp.get_number_of_x(), 0);
	auto ref_to_rs = z.rbegin();
	for(int i = 0; i < number_of_r; ++i, ++ref_to_rs) {
		*ref_to_rs = 1;
	}
	lp.set_z(z);
}

template<typename T>
T operator*(const std::vector<T> &a, const std::vector<T> &b)
{
	if(a.size() != b.size())
		throw std::runtime_error(ColoredString::red("length of a and rhses must be equal"));

	T r;
	for(int i = 0; i < a.size(); ++i)
		r = r + (a[i] * b[i]);

	return r;
}

LP::TableType Transpose(const LP::TableType &table)
{
	LP::TableType r(table[0].size(), LP::RowOfTable(table.size()));
	for(int i = 0; i < table.size(); ++i)
		for(int j = 0; j < table[i].size(); ++j)
			r[j][i] = table[i][j];
	return r;
}

template<typename T>
ssize_t max(const std::vector<T> &a)
{
	ssize_t max_index = -1;
	T max_val = 0;
	for(ssize_t i = 0; i < a.size(); ++i) {
		if(a[i] > max_val) {
			max_val = a[i];
			max_index = i;
		}
	}
	return max_index;
}

template<typename T>
ssize_t min(const std::vector<T> &a)
{
	ssize_t min_index = -1;
	T min_val = 0;
	for(ssize_t i = 0; i < a.size(); ++i) {
		if(a[i] < min_val) {
			min_val = a[i];
			min_index = i;
		}
	}
	return min_index;
}

template<typename T>
ssize_t min_test(const std::vector<T> &column, const std::vector<T> &rhs)
{
	ssize_t min_index = -1;
	T min_val = -1;
	for(ssize_t i = 0; i < column.size(); ++i) {
		if(column[i] > 0) {
			if(min_val == -1) {
				min_val = rhs[i] / column[i];
				min_index = i;
			} else if(rhs[i] / column[i] < min_val) {
				min_val = rhs[i] / column[i];
				min_index = i;
			}
		}
	}
	return min_index;
}

ssize_t Simplex::select_column() const noexcept
{
	return lp.get_type_of_optimization() == LP::TypeOfOptimization::max ? max(c_bar) : min(c_bar);
	;
}

void Simplex::ans()
{
	cb.clear();
	c_bar.clear();
	auto table = lp.get_table();
	if(cj.empty()) {
		for(const auto &row: table) {
			for(size_t i = number_of_x; i < row.size(); ++i) {
				if(row[i] == 1) {
					cj.push_back(i);
					cb.push_back(lp.z_at(i));
				}
			}
		}
	} else {
		for(const auto &i: cj)
			cb.push_back(lp.z_at(i));
	}

	z_bar = cb * lp.get_rhs();
	auto t_table = Transpose(table);
	for(size_t i = 0; i < t_table.size(); ++i)
		c_bar.push_back(lp.z_at(i) - cb * t_table[i]);

	auto rhs = lp.get_rhs();
	ssize_t new_base_column_index = select_column();
	ssize_t new_base_row_index = min_test(t_table[new_base_column_index], rhs);
	while(new_base_column_index > -1 && new_base_row_index > -1) {
		print_table();

		auto last_z_bar = calculate_table(table, rhs, new_base_row_index, new_base_column_index);

		t_table = Transpose(table);
		new_base_column_index = select_column();
		if(new_base_column_index > -1)
			new_base_row_index = min_test(t_table[new_base_column_index], rhs);
		lp.set_rhs(rhs);
		lp.set_table(table);
	}
}

std::string Simplex::name_of_var(const size_t &index_of_var, bool prim) const noexcept
{
	if(index_of_var < number_of_x) {
		if(prim)
			return "X" + std::to_string(index_of_var + 1);
		else
			return "X" + std::to_string(index_of_var + 1);
	} else if(index_of_var < number_of_x + number_of_s)
		return "S" + std::to_string(index_of_var - number_of_x + 1);
	else if(index_of_var < number_of_x + number_of_s + number_of_r)
		return "R" + std::to_string(index_of_var - (number_of_x + number_of_s) + 1);
	else
		return "NaN";
}

void Simplex::print_table()
{
	const size_t NUMBER_OF_ROW = 4 + lp.get_number_of_line();
	const size_t NUMBER_OF_COLUMN = 3 + lp.get_number_of_x();
	PrintTable table(NUMBER_OF_ROW, PrintRow(NUMBER_OF_COLUMN, ""));
	generate_first_row_of_table_for_print(table);
	generate_second_row_of_table_for_print(table);
	generate_thread_row_of_table_for_print(table);
	generate_base_of_table_for_print(table);
	generate_last_row_of_table_for_print(table);

	size_t NUMBER_OF_LINE_PRINT = NUMBER_OF_ROW + 4;
	PrintRow row_for_print(NUMBER_OF_LINE_PRINT, "");
	set_start_table_for_print(row_for_print);
	size_t size_first_column = set_first_and_second_column_for_print(table, row_for_print, 0);
	size_t size_second_column = set_first_and_second_column_for_print(table, row_for_print, 1);

	row_for_print[NUMBER_OF_LINE_PRINT - 2] +=
	    print_string_column(table.back()[0], size_first_column + size_second_column + 3);
	row_for_print[NUMBER_OF_LINE_PRINT - 2] += PRINT_VERTICAL;

	for(int i = 2; i < lp.get_number_of_x() + 2; ++i)
		set_base_column_for_print(table, row_for_print, i);

	set_last_base_table_for_print(row_for_print);
	set_last_column_table_for_print(table, row_for_print);

	for(auto &row: row_for_print)
		std::cout << row << std::endl;
}

void Simplex::generate_first_row_of_table_for_print(PrintTable &table) const noexcept
{
	auto &first_row = table[0];
	for(int i = 2; i < first_row.size() - 1; ++i)
		first_row[i] = lp.z_at(i - 2);
}

void Simplex::generate_second_row_of_table_for_print(PrintTable &table) const noexcept
{

	auto &second_row = table[1];
	second_row[0] = "Cb";
	second_row[1] = "Cj";
	second_row.back() = "RHS";
}

void Simplex::generate_thread_row_of_table_for_print(PrintTable &table) const noexcept
{
	auto &thread_row = table[2];
	for(size_t index_of_var = 0; index_of_var < number_of_x + number_of_s + number_of_r; ++index_of_var)
		thread_row[index_of_var + 2] = name_of_var(index_of_var);
}

void Simplex::generate_base_of_table_for_print(PrintTable &table) const noexcept
{
	for(size_t index_row_of_base = 0; index_row_of_base < lp.get_number_of_line(); ++index_row_of_base) {
		auto &row_of_base = table[index_row_of_base + 3];
		row_of_base[0] = cb[index_row_of_base];
		row_of_base[1] = name_of_var(cj[index_row_of_base]);

		for(size_t index_column_of_base_table = 0; index_column_of_base_table < lp.get_number_of_x();
		    ++index_column_of_base_table)
			row_of_base[index_column_of_base_table + 2] = lp.table_at(index_row_of_base, index_column_of_base_table);

		row_of_base.back() = lp.rhs_at(index_row_of_base);
	}
}

void Simplex::generate_last_row_of_table_for_print(PrintTable &table) const noexcept
{
	auto &last_row = table.back();
	last_row[0] = "C_bar";
	for(int index_of_c_br = 0; index_of_c_br < c_bar.size(); ++index_of_c_br)
		last_row[index_of_c_br + 2] = c_bar[index_of_c_br];
	last_row.back() = z_bar;
}

size_t Simplex::size_of_the_biggest_string_in_column_of_table_for_print(const Simplex::PrintTable &table,
                                                                        size_t number_of_column) const noexcept
{
	size_t max_size = 0;
	for(const PrintRow &row: table) {
		size_t size_of_string = row[number_of_column].size();
		if(max_size < size_of_string)
			max_size = size_of_string;
	}
	return max_size;
}

void Simplex::set_start_table_for_print(Simplex::PrintRow &row_for_print) const noexcept
{
	const size_t NUMBER_OF_LINE_PRINT = row_for_print.size();
	for(size_t i = 0; i < NUMBER_OF_LINE_PRINT; ++i) {
		if(i == 0) {
			row_for_print[i] = BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::topLeft);
			row_for_print[i] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal);
		} else if(i == 4 || i == (NUMBER_OF_LINE_PRINT - 3)) {
			row_for_print[i] = BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontalRight);
			row_for_print[i] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal);
		} else if(i == NUMBER_OF_LINE_PRINT - 1) {
			row_for_print[i] = BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::bottomLeft);
			row_for_print[i] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal);
		} else {
			row_for_print[i] = BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::vertical);
			row_for_print[i] += " ";
		}
	}
}

std::string Simplex::print_string_column(const std::string &s, const size_t &size_of_result_string) const noexcept
{
	size_t size_of_str = s.size();
	if(size_of_result_string < size_of_str)
		return s;
	size_t diff_size = size_of_result_string - size_of_str;
	size_t first_space_size = diff_size / 2, last_space_size = (diff_size + 1) / 2;
	std::string r;
	put_specific_char(r, first_space_size);
	r += s;
	put_specific_char(r, last_space_size);
	return r;
}

void Simplex::put_specific_char(std::string &str, size_t number_of_space,
                                const std::string &specific_char) const noexcept
{
	for(int i = 0; i < number_of_space; ++i)
		str += specific_char;
}

size_t Simplex::set_first_and_second_column_for_print(const Simplex::PrintTable &table,
                                                      Simplex::PrintRow &row_for_print, size_t index_col) const noexcept
{
	const size_t max_size = size_of_the_biggest_string_in_column_of_table_for_print(table, index_col);
	const size_t NUMBER_OF_LINE_PRINT = row_for_print.size();

	put_specific_char(row_for_print[0], max_size, BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal));
	row_for_print[0] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal);
	row_for_print[0] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontalUp);
	row_for_print[0] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal);

	put_specific_char(row_for_print[1], max_size);
	row_for_print[1] += PRINT_VERTICAL;

	row_for_print[2] += print_string_column(table[1][index_col], max_size);
	if(index_col == 0)
		row_for_print[2] += PRINT_VERTICAL;
	else
		row_for_print[2] += " " + BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontalRight) +
		    BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal);

	put_specific_char(row_for_print[3], max_size);
	row_for_print[3] += PRINT_VERTICAL;

	put_specific_char(row_for_print[4], max_size, BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal));
	row_for_print[4] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal);
	row_for_print[4] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontalAndVertical);
	row_for_print[4] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal);

	for(size_t i = 0; i < lp.get_number_of_line(); ++i) {
		row_for_print[i + 5] += print_string_column(table[i + 3][index_col], max_size);
		row_for_print[i + 5] += PRINT_VERTICAL;
	}

	put_specific_char(row_for_print[(NUMBER_OF_LINE_PRINT - 3)], max_size,
	                  BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal));
	row_for_print[(NUMBER_OF_LINE_PRINT - 3)] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal);
	row_for_print[(NUMBER_OF_LINE_PRINT - 3)] += BOX_DRAWING_CHARACTERS.at(
	    index_col == 1 ? TypeOfBoxDrawing::horizontalAndVertical : TypeOfBoxDrawing::horizontalDown);
	row_for_print[(NUMBER_OF_LINE_PRINT - 3)] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal);

	put_specific_char(row_for_print[(NUMBER_OF_LINE_PRINT - 1)], max_size,
	                  BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal));
	row_for_print[(NUMBER_OF_LINE_PRINT - 1)] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal);
	row_for_print[(NUMBER_OF_LINE_PRINT - 1)] +=
	    BOX_DRAWING_CHARACTERS.at(index_col == 1 ? TypeOfBoxDrawing::horizontalDown : TypeOfBoxDrawing::horizontal);
	row_for_print[(NUMBER_OF_LINE_PRINT - 1)] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal);

	return max_size;
}

void Simplex::set_base_column_for_print(const Simplex::PrintTable &table, Simplex::PrintRow &row_for_print,
                                        size_t index_col) const noexcept
{
	const size_t max_size = size_of_the_biggest_string_in_column_of_table_for_print(table, index_col);
	const size_t NUMBER_OF_LINE_PRINT = row_for_print.size();

	put_specific_char(row_for_print[0], max_size, BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal));
	row_for_print[0] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal);

	row_for_print[1] += print_string_column(table[0][index_col], max_size);
	row_for_print[1] += " ";

	put_specific_char(row_for_print[2], max_size, BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal));
	row_for_print[2] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal);

	row_for_print[3] += print_string_column(table[2][index_col], max_size);
	row_for_print[3] += " ";

	put_specific_char(row_for_print[4], max_size, BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal));
	row_for_print[4] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal);

	for(size_t i = 0; i < lp.get_number_of_line(); ++i) {
		row_for_print[i + 5] += print_string_column(table[i + 3][index_col], max_size);
		row_for_print[i + 5] += " ";
	}

	put_specific_char(row_for_print[(NUMBER_OF_LINE_PRINT - 3)], max_size,
	                  BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal));
	row_for_print[(NUMBER_OF_LINE_PRINT - 3)] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal);

	row_for_print[(NUMBER_OF_LINE_PRINT - 2)] += print_string_column(table.back()[index_col], max_size);
	row_for_print[(NUMBER_OF_LINE_PRINT - 2)] += " ";

	put_specific_char(row_for_print[(NUMBER_OF_LINE_PRINT - 1)], max_size,
	                  BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal));
	row_for_print[(NUMBER_OF_LINE_PRINT - 1)] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal);
}

void Simplex::set_last_base_table_for_print(Simplex::PrintRow &row_for_print) const noexcept
{
	const size_t NUMBER_OF_LINE_PRINT = row_for_print.size();
	for(size_t i = 0; i < NUMBER_OF_LINE_PRINT; ++i) {
		if(i == 0) {
			row_for_print[i] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontalUp);
			row_for_print[i] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal);
		} else if(i == 2) {
			row_for_print[i] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontalLeft);
			row_for_print[i] += " ";
		} else if(i == 4 || i == (NUMBER_OF_LINE_PRINT - 3)) {
			row_for_print[i] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontalAndVertical);
			row_for_print[i] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal);
		} else if(i == NUMBER_OF_LINE_PRINT - 1) {
			row_for_print[i] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontalDown);
			row_for_print[i] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal);
		} else {
			row_for_print[i] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::vertical);
			row_for_print[i] += " ";
		}
	}
}

void Simplex::set_last_column_table_for_print(const Simplex::PrintTable &table,
                                              Simplex::PrintRow &row_for_print) const noexcept
{
	const size_t index_col = table[0].size() - 1;
	const size_t max_size = size_of_the_biggest_string_in_column_of_table_for_print(table, index_col);
	const size_t NUMBER_OF_LINE_PRINT = row_for_print.size();

	put_specific_char(row_for_print[0], max_size, BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal));
	row_for_print[0] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal);
	row_for_print[0] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::topRight);

	put_specific_char(row_for_print[1], max_size);
	row_for_print[1] += PRINT_VERTICAL;

	row_for_print[2] += print_string_column(table[1][index_col], max_size);
	row_for_print[2] += PRINT_VERTICAL;

	put_specific_char(row_for_print[3], max_size);
	row_for_print[3] += PRINT_VERTICAL;

	put_specific_char(row_for_print[4], max_size, BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal));
	row_for_print[4] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal);
	row_for_print[4] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontalLeft);

	for(size_t i = 0; i < lp.get_number_of_line(); ++i) {
		row_for_print[i + 5] += print_string_column(table[i + 3][index_col], max_size);
		row_for_print[i + 5] += PRINT_VERTICAL;
	}

	put_specific_char(row_for_print[(NUMBER_OF_LINE_PRINT - 3)], max_size,
	                  BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal));
	row_for_print[(NUMBER_OF_LINE_PRINT - 3)] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal);
	row_for_print[(NUMBER_OF_LINE_PRINT - 3)] += BOX_DRAWING_CHARACTERS.at(horizontalLeft);

	row_for_print[NUMBER_OF_LINE_PRINT - 2] += print_string_column(table.back()[index_col], max_size);
	row_for_print[NUMBER_OF_LINE_PRINT - 2] += PRINT_VERTICAL;

	put_specific_char(row_for_print[(NUMBER_OF_LINE_PRINT - 1)], max_size,
	                  BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal));
	row_for_print[(NUMBER_OF_LINE_PRINT - 1)] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::horizontal);
	row_for_print[(NUMBER_OF_LINE_PRINT - 1)] += BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::bottomRight);
}

void Simplex::print_ans()
{
	for(const auto &num: c_bar) {
		if(lp.get_type_of_optimization() == LP::TypeOfOptimization::max && num > 0) {
			std::cout << ColoredString::blue("the answer of LP is positive infinite!");
			return;
		} else if(lp.get_type_of_optimization() == LP::TypeOfOptimization::min && num < 0) {
			std::cout << ColoredString::blue("the answer of LP is negative infinite!");
			return;
		}
	}

	for(size_t i = 0; i < cj.size(); ++i) {
		if(cj[i] >= number_of_x + number_of_s) {
			if(!(lp.rhs_at(i) == 0)) {
				std::cout << ColoredString::blue("this LP is not possible answer!");
				return;
			}
		}
	}

	size_t number_of_zero = 0;
	for(size_t i = 0; i < number_of_x + number_of_s; i++) {
		if(c_bar[i] == 0)
			++number_of_zero;
	}

	if(number_of_zero > lp.get_number_of_line()) {
		std::cout << ColoredString::blue("this has multi answers! one of answers is:\n");
	}
	for(size_t i = 0; i < cj.size(); i++) {
		std::cout << name_of_var(cj[i]) << " = " << LP::Coefficient::to_string(lp.rhs_at(i)) << std::endl;
	}
	print_transformers_ans();
}

LP::Coefficient Simplex::calculate_table(LP::TableType &table, LP::RHSesType &rhs, size_t row, size_t column) noexcept
{
	LP::Coefficient a = LP::Coefficient(1) / table[row][column];
	for(auto &cell: table[row])
		cell *= a;

	rhs[row] *= a;

	cj[row] = column;
	cb[row] = lp.z_at(column);

	auto &new_base_row = table[row];
	for(size_t row_index = 0; row_index < table.size(); ++row_index) {
		auto &t = table[row_index];
		if(t == new_base_row)
			continue;

		a = t[column] / new_base_row[column];
		for(int i = 0; i < t.size(); ++i)
			t[i] -= a * new_base_row[i];

		rhs[row_index] -= a * rhs[row];
	}

	a = c_bar[column] / new_base_row[column];
	for(int i = 0; i < c_bar.size(); ++i)
		c_bar[i] -= a * new_base_row[i];

	auto last_z_bar = z_bar;
	z_bar = cb * rhs;
	return last_z_bar;
}

void Simplex::print_transformers_ans() const noexcept
{
	Simplex::ListOfX xs(lp.get_number_of_x(), 0);
	for(size_t i = 0; i < cj.size(); i++) {
		xs[cj[i]] = lp.rhs_at(i);
	}
	std::cout << ColoredString::yellow("So answer is:") << std::endl;
	for(size_t i = 0; i < transformers.size(); i++) {
		std::cout << name_of_var(i, false) << " = " << LP::Coefficient::to_string(transformers[i](xs)) << std::endl;
	}
}
