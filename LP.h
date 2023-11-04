#ifndef LP_LP_H
#define LP_LP_H

#include <bits/stdc++.h>

using namespace std;

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

	typedef vector<int64_t> ZType;
	typedef vector<int64_t> RHSesType;
	typedef vector<vector<int64_t>> TableType;
	typedef vector<Comparative> ComparativesType;
	typedef vector<Sign> SignsType;

	LP(size_t number_of_x, size_t number_of_line, TypeOfOptimization type_of_optimization, ZType z,
	   TableType table, RHSesType b, ComparativesType comparatives, SignsType signs);

	void set_number_of_x(size_t num);
	size_t get_number_of_x() const noexcept;

	void set_number_of_line(size_t num);
	size_t get_number_of_line() const noexcept;

	void set_type_of_optimization(TypeOfOptimization type);
	TypeOfOptimization get_type_of_optimization() const noexcept;

	void set_z(const ZType& other_z);
	ZType get_z() const noexcept;

	void set_table(const TableType& t);
	TableType get_table() const noexcept;

	void set_b(const RHSesType& rhs);
	RHSesType get_b() const noexcept;

	void set_comparatives(const ComparativesType& c);
	ComparativesType get_comparatives() const noexcept;

	void set_signs(const SignsType& s);
	SignsType get_signs() const noexcept;

	static string to_string(TypeOfOptimization type) noexcept;

	static string to_string(Sign s) noexcept;

	static string to_string(Comparative c) noexcept;

private:
	void validate_table() const;
	void validate_b() const;
	void validate_comparatives() const;
	void validate_signs() const;
	void validate_z() const;

	size_t number_of_x;
	size_t number_of_line;
	TypeOfOptimization type_of_optimization;
	ZType z;
	TableType table;
	RHSesType b;
	ComparativesType comparatives;
	SignsType signs;
};


#endif
