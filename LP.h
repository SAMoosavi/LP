#ifndef LP_LP_H
#define LP_LP_H

#include <bits/stdc++.h>
#include "MNumber.h"
using namespace std;

/// Save linear programing information
class LP {
public:
	enum TypeOfOptimization {
		min = 0, max = 1
	};

	enum Sign {
		negative = -1,
		freeSign = 0,
		positive = 1
	};

	enum Comparative {
		lower = -1,
		equal = 0,
		greater = 1
	};

	typedef MNumber<double> Coefficient;
	typedef vector<Coefficient> ZType;
	typedef vector<Coefficient> RHSesType;
	typedef Coefficient CellOfTable;
	typedef vector<CellOfTable> RowOfTable;
	typedef vector<RowOfTable> TableType;
	typedef vector<Comparative> ComparativesType;
	typedef vector<Sign> SignsType;

	LP();

	LP(size_t number_of_x, size_t number_of_line, TypeOfOptimization type_of_optimization, ZType z,
		TableType table, RHSesType rhses, ComparativesType comparatives, SignsType signs);

	void set_number_of_x(size_t num);
	size_t get_number_of_x() const noexcept;

	void set_number_of_line(size_t num);
	size_t get_number_of_line() const noexcept;

	void set_type_of_optimization(TypeOfOptimization type);
	TypeOfOptimization get_type_of_optimization() const noexcept;

	void set_z(const ZType &other_z);
	ZType get_z() const noexcept;
	Coefficient z_at(size_t index) const noexcept;

	void set_table(const TableType &t);
	TableType get_table() const noexcept;
	CellOfTable table_at(size_t number_of_row, size_t number_of_column) const noexcept;

	void set_rhs(const RHSesType &rhs);
	RHSesType get_rhs() const noexcept;
	Coefficient rhs_at(size_t index) const noexcept;

	void set_comparatives(const ComparativesType &c);
	ComparativesType get_comparatives() const noexcept;
	Comparative comparative_at(size_t index) const noexcept;

	void set_signs(const SignsType &s);
	SignsType get_signs() const noexcept;
	Sign sign_at(size_t index) const noexcept;

	static string to_string(TypeOfOptimization type) noexcept;

	static string to_string(Sign s) noexcept;

	static string to_string(Comparative c) noexcept;

private:
	/// Check size of @c table is @c number_of_line * @c number_of_x
	void validate_table() const;

	/// Check size of @c rhses is @c number_of_line
	void validate_rhses() const;

	/// Check size of @c comparatives is @c number_of_line
	void validate_comparatives() const;

	/// Check size of @c signs is @c number_of_x
	void validate_signs() const;

	/// Check size of @c z is @c number_of_x
	void validate_z() const;

	size_t number_of_x;
	size_t number_of_line;
	TypeOfOptimization type_of_optimization;
	ZType z;
	TableType table;
	RHSesType rhses;
	ComparativesType comparatives;
	SignsType signs;
};


#endif
