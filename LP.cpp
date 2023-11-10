#include <bits/stdc++.h>

#include "LP.h"
#include "ColoredString.h"

using namespace std;

string LP::to_string(LP::TypeOfOptimization type) noexcept {
	switch(type) {
		case TypeOfOptimization::min: return "min";
		case TypeOfOptimization::max: return "max";
		default: return "";
	}
}

string LP::to_string(LP::Sign s) noexcept {
	switch(s) {
		case Sign::negative: return "<= 0";
		case Sign::positive: return ">= 0";
		case Sign::freeSign: return "free";
		default: return "";

	}
}

string LP::to_string(LP::Comparative c) noexcept {
	switch(c) {
		case Comparative::lower: return "<=";
		case Comparative::equal: return " =";
		case Comparative::greater: return ">=";
		default: return "";
	}
}

string LP::to_string(LP::M m) noexcept {
	return m;
}

LP::LP()
	: number_of_x(0)
	, number_of_line(0)
	, type_of_optimization(TypeOfOptimization::max)
	, z()
	, table()
	, b()
	, comparatives()
	, signs() {}

LP::LP(size_t number_of_x, size_t number_of_line, TypeOfOptimization type_of_optimization, ZType z,
	TableType table, RHSesType b, ComparativesType comparatives, SignsType signs)
	: number_of_x(number_of_x)
	, number_of_line(number_of_line)
	, type_of_optimization(type_of_optimization)
	, z(std::move(z))
	, table(std::move(table))
	, b(std::move(b))
	, comparatives(std::move(comparatives))
	, signs(std::move(signs)) {
	validate_z();
	validate_b();
	validate_table();
	validate_comparatives();
	validate_signs();
}

void LP::validate_table() const {
	if(this->table.size() != number_of_line)
		throw runtime_error(ColoredString::red("Invalid size of table."));
	for(auto &t: this->table)
		if(t.size() != number_of_x)
			throw runtime_error(ColoredString::red("Invalid size of table."));
}

void LP::validate_b() const {
	if(this->b.size() != number_of_line)
		throw runtime_error(ColoredString::red("Invalid size of right hand side."));
}

void LP::validate_z() const {
	if(this->z.size() != number_of_x)
		throw runtime_error(ColoredString::red("Invalid size of z."));
}

void LP::validate_comparatives() const {
	if(this->comparatives.size() != number_of_line)
		throw runtime_error(ColoredString::red("Invalid size of comparatives."));

}

void LP::validate_signs() const {
	if(this->signs.size() != number_of_x)
		throw runtime_error(ColoredString::red("Invalid size of signs."));

}

size_t LP::get_number_of_x() const noexcept {
	return number_of_x;
}

size_t LP::get_number_of_line() const noexcept {
	return number_of_line;
}

LP::TypeOfOptimization LP::get_type_of_optimization() const noexcept {
	return type_of_optimization;
}

auto LP::get_z() const noexcept -> ZType {
	return z;
}

LP::Coefficient LP::z_at(size_t index) const noexcept {
	return z[index];
}

auto LP::get_table() const noexcept -> TableType {
	return table;
}

LP::CellOfTable LP::table_at(size_t number_of_row, size_t number_of_column) const noexcept {
	return table[number_of_row][number_of_column];
}


auto LP::get_rhs() const noexcept -> RHSesType {
	return b;
}

LP::Coefficient LP::rhs_at(size_t index) const noexcept {
	return b[index];
}

auto LP::get_comparatives() const noexcept -> ComparativesType {
	return comparatives;
}

LP::Comparative LP::comparative_at(size_t index) const noexcept {
	return comparatives[index];
}

auto LP::get_signs() const noexcept -> SignsType {
	return signs;
}

LP::Sign LP::sign_at(size_t index) const noexcept {
	return signs[index];
}

void LP::set_number_of_x(size_t num) {
	number_of_x = num;
	for(auto &t: table)
		t.resize(num);
	z.resize(num);
	signs.resize(num);
}

void LP::set_number_of_line(size_t num) {
	number_of_line = num;
	table.resize(num);
	for(auto &t: table)
		t.resize(number_of_x);
	b.resize(num);
	comparatives.resize(num);
}

void LP::set_type_of_optimization(LP::TypeOfOptimization type) {
	type_of_optimization = type;
}

void LP::set_z(const LP::ZType &other_z) {
	z = other_z;
	validate_z();
}

void LP::set_table(const LP::TableType &t) {
	table = t;
	validate_table();
}

void LP::set_rhs(const LP::RHSesType &rhs) {
	b = rhs;
	validate_b();
}

void LP::set_comparatives(const LP::ComparativesType &c) {
	comparatives = c;
	validate_comparatives();
}

void LP::set_signs(const LP::SignsType &s) {
	signs = s;
	validate_signs();
}


LP::M::M()
	: inf(0)
	, num(0) {}

LP::M::M(double num)
	: inf(0)
	, num(num) {}

LP::M::M(double inf, double num)
	: inf(inf)
	, num(num) {}


LP::M::M(const LP::M &other)
	: inf(other.inf)
	, num(other.num) {}

LP::M::M(LP::M &&other) noexcept
	: inf(other.inf)
	, num(other.num) {}

LP::M &LP::M::operator=(const double &other) {
	inf = 0;
	num = other;
	return *this;
}

LP::M &LP::M::operator=(const LP::M &other) {
	inf = other.inf;
	num = other.num;
	return *this;
}

LP::M &LP::M::operator=(LP::M &&other) noexcept {
	inf = other.inf;
	num = other.num;
	return *this;
}

LP::M LP::M::operator+(const LP::M &obj) const noexcept {
	return {inf + obj.inf, num + obj.num};
}

LP::M LP::M::operator+(const double &obj) const noexcept {
	return *this + M(obj);
}

LP::M LP::M::operator+=(const LP::M &obj) noexcept {
	inf += obj.inf;
	num += obj.num;
	return *this;
}

LP::M LP::M::operator+=(const double &obj) noexcept {
	num += obj;
	return *this;
}

LP::M LP::M::operator-() const noexcept {
	return {-inf, -num};
}

LP::M LP::M::operator-(const LP::M &obj) const noexcept {
	return *this + (-obj);
}

LP::M LP::M::operator-(const double &obj) const noexcept {
	return *this - M(obj);;
}

LP::M &LP::M::operator-=(const LP::M &obj) noexcept {
	inf -= obj.inf;
	num -= obj.num;
	return *this;
}

LP::M &LP::M::operator-=(const double &obj) noexcept {
	num -= obj;
	return *this;
}

bool LP::M::operator==(const LP::M &other) const noexcept {
	return (num == other.num) && (inf == other.inf);
}

bool LP::M::operator==(const double &other) const noexcept {
	return (num == other) && (inf == 0);
}

bool LP::M::operator>(const LP::M &other) const noexcept {
	return (inf > other.inf) || ((inf == other.inf) && (num > other.num));
}

bool LP::M::operator>(const double &other) const noexcept {
	return *this > M(other);
}

bool LP::M::operator>=(const LP::M &other) const noexcept {
	return (*this > other) || (*this == other);
}

bool LP::M::operator>=(const double &other) const noexcept {
	return *this >= M(other);
}

bool LP::M::operator<(const LP::M &other) const noexcept {
	return !(*this >= other);
}

bool LP::M::operator<(const double &other) const noexcept {
	return *this < M(other);
}

bool LP::M::operator<=(const LP::M &other) const noexcept {
	return !(*this > other);
}

bool LP::M::operator<=(const double &other) const noexcept {
	return *this <= M(other);
}

LP::M::operator string() const noexcept {
	string s = "(";
	const auto to_string = [](const double &d) -> string {
		string num_text = std::to_string(d);
		auto r =  num_text.substr(0, num_text.find('.') + 3);
		while(r.back() != '.'){
			if(r.back() == '0')
				r.pop_back();
			else
				break;
		}
		if(r.back() == '.')
			r.pop_back();
		return r;
	};

	if(inf != 0) {
		if(inf > 0)
			s += "+";
		s += to_string(inf) + "M";
	}

	if(num != 0) {
		if(num > 0)
			s += "+";
		s += to_string(num);
	}

	if(s.empty())
		s = to_string(0);

	s += ")";
	return s;
}

LP::M LP::M::operator*(const LP::M &obj) const {
	if(inf == 0)
		return obj * num;
	else if(obj.inf == 0)
		return *this * obj.num;
	else
		throw std::runtime_error(ColoredString::red("M * M must be one of they inf equal to 0"));
}

LP::M LP::M::operator*(const double &obj) const noexcept {
	return {inf * obj, num * obj};
}

LP::M LP::M::operator*=(const LP::M &obj) {
	*this = *this * obj;
	return *this;
}

LP::M LP::M::operator*=(const double &obj) noexcept {
	*this = *this * obj;
	return *this;
}

LP::M LP::M::operator/(const LP::M &obj) const {
	if(obj.inf == 0)
		return *this / obj.num;
	else
		throw std::runtime_error(ColoredString::red("M1 / M2 must be M2 inf equal to 0"));
}

LP::M LP::M::operator/(const double &obj) const noexcept {
	return {inf / obj, num / obj};
}
