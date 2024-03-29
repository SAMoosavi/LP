#ifndef LP_SIMPLEX_H
#define LP_SIMPLEX_H

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "ColoredString.h"
#include "InputLP.h"
#include "LP.h"

class Simplex
{
public:
	typedef LP::Coefficient ValueOfX;
	typedef std::vector<ValueOfX> ListOfX;
	typedef std::function<ValueOfX(const ListOfX &)> Transformer;
	typedef std::vector<Transformer> Transformers;
	typedef std::vector<size_t> Bases;

	explicit Simplex(const LP &last_lp);

private:
	typedef std::vector<std::string> PrintRow;
	typedef std::vector<PrintRow> PrintTable;

	enum TypeOfBoxDrawing
	{
		horizontal,
		vertical,
		topLeft,
		topRight,
		bottomLeft,
		bottomRight,
		horizontalAndVertical,
		horizontalUp,
		horizontalDown,
		horizontalRight,
		horizontalLeft,
	};
	const std::map<TypeOfBoxDrawing, std::string> BOX_DRAWING_CHARACTERS = {
	    {TypeOfBoxDrawing::horizontal, "─"},
	    {TypeOfBoxDrawing::vertical, "│"},
	    {TypeOfBoxDrawing::topLeft, "┌"},
	    {TypeOfBoxDrawing::topRight, "┐"},
	    {TypeOfBoxDrawing::bottomLeft, "└"},
	    {TypeOfBoxDrawing::bottomRight, "┘"},
	    {TypeOfBoxDrawing::horizontalAndVertical, "┼"},
	    {TypeOfBoxDrawing::horizontalUp, "┬"},
	    {TypeOfBoxDrawing::horizontalDown, "┴"},
	    {TypeOfBoxDrawing::horizontalRight, "├"},
	    {TypeOfBoxDrawing::horizontalLeft, "┤"}};
	const std::string PRINT_VERTICAL = " " + BOX_DRAWING_CHARACTERS.at(TypeOfBoxDrawing::vertical) + " ";

	void creat_std_lp(const LP &last_lp);

	void made_base();

	void edit_base();

	void ans();

	ssize_t select_column() const noexcept;

	std::string name_of_var(const size_t &index_of_var, bool prim = true) const noexcept;

	void generate_first_row_of_table_for_print(PrintTable &table) const noexcept;
	void generate_second_row_of_table_for_print(PrintTable &table) const noexcept;
	void generate_thread_row_of_table_for_print(PrintTable &table) const noexcept;
	void generate_base_of_table_for_print(PrintTable &table) const noexcept;
	void generate_last_row_of_table_for_print(PrintTable &table) const noexcept;

	size_t size_of_the_biggest_string_in_column_of_table_for_print(const PrintTable &table,
	                                                               size_t number_of_column) const noexcept;

	void set_start_table_for_print(PrintRow &row_for_print) const noexcept;
	size_t set_first_and_second_column_for_print(const PrintTable &table, PrintRow &row_for_print,
	                                             size_t index_col) const noexcept;
	void set_base_column_for_print(const PrintTable &table, PrintRow &row_for_print, size_t index_col) const noexcept;
	void set_last_base_table_for_print(PrintRow &row_for_print) const noexcept;
	void set_last_column_table_for_print(const PrintTable &table, PrintRow &row_for_print) const noexcept;

	std::string print_string_column(const std::string &s, const size_t &size_of_result_string) const noexcept;

	void put_specific_char(std::string &str, size_t number_of_char,
	                       const std::string &specific_char = " ") const noexcept;

	void print_table();

	void print_ans();

	LP::Coefficient calculate_table(LP::TableType &table, LP::RHSesType &rhs, size_t row, size_t column) noexcept;

	void print_transformers_ans() const noexcept;

	LP lp;
	Transformers transformers;
	size_t number_of_x{};
	size_t number_of_s{};
	size_t number_of_r{};
	Bases bases;
	std::vector<LP::Coefficient> cb{};
	std::vector<size_t> cj{};
	std::vector<LP::Coefficient> c_bar{};
	LP::Coefficient z_bar;
};

#endif
