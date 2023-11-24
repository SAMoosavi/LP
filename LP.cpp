#include "LP.h"

#include <algorithm>
#include <map>
#include <stdexcept>
#include <string>

#include "ColoredString.h"
#include "MNumber.h"

std::string LP::to_string(LP::TypeOfOptimization type) noexcept
{
	switch(type) {
	case TypeOfOptimization::min:
		return "min";
	case TypeOfOptimization::max:
		return "max";
	default:
		return "";
	}
}

std::string LP::to_string(LP::Sign s) noexcept
{
	switch(s) {
	case Sign::negative:
		return "<= 0";
	case Sign::positive:
		return ">= 0";
	case Sign::freeSign:
		return "free";
	default:
		return "";
	}
}

std::string LP::to_string(LP::Comparative c) noexcept
{
	switch(c) {
	case Comparative::lower:
		return "<=";
	case Comparative::equal:
		return " =";
	case Comparative::greater:
		return ">=";
	default:
		return "";
	}
}

LP::LP()
: number_of_x(0)
, number_of_line(0)
, type_of_optimization(TypeOfOptimization::max)
, z()
, table()
, rhses()
, comparatives()
, signs()
{
}

LP::LP(size_t number_of_x, size_t number_of_line, TypeOfOptimization type_of_optimization, ZType z, TableType table,
       RHSesType rhses, ComparativesType comparatives, SignsType signs)
: number_of_x(number_of_x)
, number_of_line(number_of_line)
, type_of_optimization(type_of_optimization)
, z(std::move(z))
, table(std::move(table))
, rhses(std::move(rhses))
, comparatives(std::move(comparatives))
, signs(std::move(signs))
{
	validate_z();
	validate_rhses();
	validate_table();
	validate_comparatives();
	validate_signs();
}

void LP::validate_table() const
{
	if(this->table.size() != number_of_line)
		throw std::runtime_error(ColoredString::red("Invalid size of table."));
	for(auto &t: this->table)
		if(t.size() != number_of_x)
			throw std::runtime_error(ColoredString::red("Invalid size of table."));
}

void LP::validate_rhses() const
{
	if(this->rhses.size() != number_of_line)
		throw std::runtime_error(ColoredString::red("Invalid size of right hand side."));
}

void LP::validate_z() const
{
	if(this->z.size() != number_of_x)
		throw std::runtime_error(ColoredString::red("Invalid size of z."));
}

void LP::validate_comparatives() const
{
	if(this->comparatives.size() != number_of_line)
		throw std::runtime_error(ColoredString::red("Invalid size of comparatives."));
}

void LP::validate_signs() const
{
	if(this->signs.size() != number_of_x)
		throw std::runtime_error(ColoredString::red("Invalid size of signs."));
}

size_t LP::get_number_of_x() const noexcept
{
	return number_of_x;
}

size_t LP::get_number_of_line() const noexcept
{
	return number_of_line;
}

LP::TypeOfOptimization LP::get_type_of_optimization() const noexcept
{
	return type_of_optimization;
}

LP::ZType LP::get_z() const noexcept
{
	return z;
}

LP::Coefficient LP::z_at(size_t index) const noexcept
{
	return z[index];
}

LP::TableType LP::get_table() const noexcept
{
	return table;
}

LP::CellOfTable LP::table_at(size_t number_of_row, size_t number_of_column) const noexcept
{
	return table[number_of_row][number_of_column];
}

LP::RHSesType LP::get_rhs() const noexcept
{
	return rhses;
}

LP::Coefficient LP::rhs_at(size_t index) const noexcept
{
	return rhses[index];
}

LP::ComparativesType LP::get_comparatives() const noexcept
{
	return comparatives;
}

LP::Comparative LP::comparative_at(size_t index) const noexcept
{
	return comparatives[index];
}

LP::SignsType LP::get_signs() const noexcept
{
	return signs;
}

LP::Sign LP::sign_at(size_t index) const noexcept
{
	return signs[index];
}

void LP::set_number_of_x(size_t num)
{
	number_of_x = num;
	for(auto &t: table)
		t.resize(num);
	z.resize(num);
	signs.resize(num);
}

void LP::set_number_of_line(size_t num)
{
	number_of_line = num;
	table.resize(num);
	for(auto &t: table)
		t.resize(number_of_x);
	rhses.resize(num);
	comparatives.resize(num);
}

void LP::set_type_of_optimization(LP::TypeOfOptimization type)
{
	type_of_optimization = type;
}

void LP::set_z(const LP::ZType &other_z)
{
	z = other_z;
	validate_z();
}

void LP::set_table(const LP::TableType &t)
{
	table = t;
	validate_table();
}

void LP::set_rhs(const LP::RHSesType &rhs)
{
	rhses = rhs;
	validate_rhses();
}

void LP::set_comparatives(const LP::ComparativesType &c)
{
	comparatives = c;
	validate_comparatives();
}

void LP::set_signs(const LP::SignsType &s)
{
	signs = s;
	validate_signs();
}

std::string LP::to_string(const std::string &name_of_var) const noexcept
{
	std::string result;
	result += z_to_string(name_of_var);
	result += body_to_string(name_of_var);
	result += signs_to_string(name_of_var);
	return result;
}

std::string LP::z_to_string(const std::string &name_of_var) const noexcept
{
	std::string result;
	result += ColoredString::yellow(to_string(type_of_optimization)) + " ";
	for(int i = 0; i < z.size(); ++i) {
		if(z[i] == 0)
			continue;
		std::string z_str = (std::string)z[i] + name_of_var + std::to_string(i + 1);
		if(!z_str.starts_with('-'))
			result += ColoredString::blue("+");
		result += ColoredString::blue(z_str + " ");
	}
	result += '\n';
	return result;
}

std::string LP::body_to_string(const std::string &name_of_var) const noexcept
{
	std::string result;
	std::vector<std::string> rows(number_of_line, "");
	for(int column_index = 0; column_index < number_of_x; ++column_index) {
		std::vector<std::string> column(number_of_line);
		for(int row_index = 0; row_index < number_of_line; row_index++) {
			if(table[row_index][column_index] == 0) {
				column[row_index] = "";
				continue;
			} else if(!std::string(table[row_index][column_index]).starts_with('-'))
				column[row_index] = "+";
			column[row_index] +=
			    std::string(table[row_index][column_index]) + name_of_var + std::to_string(column_index + 1);
		}

		size_t maxStringLength = (*std::ranges::max_element(column, [](const auto &a, const auto &b) {
			                         return a.length() < b.length();
		                         })).length();

		const auto generate_space = [](size_t num) {
			std::string s;
			for(size_t i = 0; i < num; ++i)
				s += " ";
			return s;
		};
		for(int row_index = 0; row_index < number_of_line; row_index++)
			rows[row_index] += generate_space(maxStringLength - column[row_index].length()) + column[row_index] + " ";
	}

	for(int row_index = 0; row_index < number_of_line; row_index++)
		result += ColoredString::green(rows[row_index]) + ColoredString::yellow(to_string(comparatives[row_index])) +
		    " " + ColoredString::green(Coefficient::to_string(rhses[row_index])) + '\n';

	return result;
}

std::string LP::signs_to_string(const std::string &name_of_var) const noexcept
{
	std::string result;
	std::map<LP::Sign, std::vector<size_t>> sign;
	for(size_t i = 0; i < number_of_x; i++)
		sign[signs[i]].emplace_back(i);

	for(const auto &s1: sign) {
		std::string s;
		for(const auto &s2: s1.second)
			s += name_of_var + std::to_string(s2 + 1) + ", ";
		s.pop_back();
		s.pop_back();
		result += ColoredString::magenta(s) + " " + ColoredString::magenta(LP::to_string(s1.first)) + '\n';
	}

	return result;
}
