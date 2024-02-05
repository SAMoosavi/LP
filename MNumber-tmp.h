#include <stdexcept>
#include <string>

#include "ColoredString.h"
#include "MNumber.h"

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber>::MNumber()
: inf(0)
, num(0)
{
}

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber>::MNumber(BaseNumber num)
: inf(0)
, num(num)
{
}

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber>::MNumber(BaseNumber inf, BaseNumber num)
: inf(inf)
, num(num)
{
}

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber>::MNumber(const MNumber<BaseNumber> &other)
    : inf(other.inf)
    , num(other.num)
{
}

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber>::MNumber(MNumber &&other) noexcept
    : inf(other.inf)
    , num(other.num)
{
}

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber> &MNumber<BaseNumber>::operator=(const BaseNumber &other_num)
{
	inf = 0;
	num = other_num;
	return *this;
}

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber> &MNumber<BaseNumber>::operator=(const MNumber<BaseNumber> &other)
{
	inf = other.inf;
	num = other.num;
	return *this;
}

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber> &MNumber<BaseNumber>::operator=(MNumber<BaseNumber> &&other) noexcept
{
	inf = other.inf;
	num = other.num;
	return *this;
}

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber> MNumber<BaseNumber>::operator+(const MNumber<BaseNumber> &obj) const noexcept
{
	return {inf + obj.inf, num + obj.num};
}

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber> MNumber<BaseNumber>::operator+(const BaseNumber &other_num) const noexcept
{
	return {inf, num + other_num};
}

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber> operator+(const Arithmetic auto &obj, const MNumber<BaseNumber> &oo) noexcept
{
	return oo + obj;
}

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber> MNumber<BaseNumber>::operator+=(const MNumber<BaseNumber> &obj) noexcept
{
	inf += obj.inf;
	num += obj.num;
	return *this;
}

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber> MNumber<BaseNumber>::operator+=(const BaseNumber &other_num) noexcept
{
	num += other_num;
	return *this;
}

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber> MNumber<BaseNumber>::operator-() const noexcept
{
	return {-inf, -num};
}

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber> MNumber<BaseNumber>::operator-(const MNumber<BaseNumber> &obj) const noexcept
{
	return *this + (-obj);
}

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber> MNumber<BaseNumber>::operator-(const BaseNumber &other_num) const noexcept
{
	return *this - MNumber(other_num);
	;
}

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber> &MNumber<BaseNumber>::operator-=(const MNumber<BaseNumber> &obj) noexcept
{
	inf -= obj.inf;
	num -= obj.num;
	return *this;
}

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber> &MNumber<BaseNumber>::operator-=(const BaseNumber &other_num) noexcept
{
	num -= other_num;
	return *this;
}

template<Arithmetic BaseNumber>
inline bool MNumber<BaseNumber>::operator==(const MNumber<BaseNumber> &other) const noexcept
{
	return (num == other.num) && (inf == other.inf);
}

template<Arithmetic BaseNumber>
inline bool MNumber<BaseNumber>::operator==(const BaseNumber &other_num) const noexcept
{
	return (num == other_num) && (inf == 0);
}

template<Arithmetic BaseNumber>
inline bool MNumber<BaseNumber>::operator>(const MNumber<BaseNumber> &other) const noexcept
{
	return (inf > other.inf) || ((inf == other.inf) && (num > other.num));
}

template<Arithmetic BaseNumber>
inline bool MNumber<BaseNumber>::operator>(const BaseNumber &other_num) const noexcept
{
	return *this > MNumber(other_num);
}

template<Arithmetic BaseNumber>
inline bool MNumber<BaseNumber>::operator>=(const MNumber<BaseNumber> &other) const noexcept
{
	return (*this > other) || (*this == other);
}

template<Arithmetic BaseNumber>
inline bool MNumber<BaseNumber>::operator>=(const BaseNumber &other_num) const noexcept
{
	return *this >= MNumber(other_num);
}

template<Arithmetic BaseNumber>
inline bool MNumber<BaseNumber>::operator<(const MNumber<BaseNumber> &other) const noexcept
{
	return !(*this >= other);
}

template<Arithmetic BaseNumber>
inline bool MNumber<BaseNumber>::operator<(const BaseNumber &other_num) const noexcept
{
	return *this < MNumber(other_num);
}

template<Arithmetic BaseNumber>
inline bool MNumber<BaseNumber>::operator<=(const MNumber<BaseNumber> &other) const noexcept
{
	return !(*this > other);
}

template<Arithmetic BaseNumber>
inline bool MNumber<BaseNumber>::operator<=(const BaseNumber &other_num) const noexcept
{
	return *this <= MNumber(other_num);
}

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber>::operator std::string() const noexcept
{
	std::string s;

	if(inf != 0) {
		if(inf == 1)
			s += "M";
		else if(inf == -1)
			s+= "-M";
		else
		s += to_string(inf) + "M";
	}
	if(num != 0) {
		if(num > 0 && inf != 0)
			s += "+";
		s += to_string(num);
	}

	if(s.empty())
		s += std::to_string(0);
	return s;
}

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber> MNumber<BaseNumber>::operator*(const MNumber<BaseNumber> &obj) const
{
	if(inf == 0)
		return obj * num;
	else if(obj.inf == 0)
		return *this * obj.num;
	else
		throw std::runtime_error(ColoredString::red("M1 * M2 must be one of they inf equal to 0"));
}

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber> MNumber<BaseNumber>::operator*(const BaseNumber &other_num) const noexcept
{
	return {inf * other_num, num * other_num};
}

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber> MNumber<BaseNumber>::operator*=(const MNumber<BaseNumber> &obj)
{
	*this = *this * obj;
	return *this;
}

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber> MNumber<BaseNumber>::operator*=(const BaseNumber &other_num) noexcept
{
	*this = *this * other_num;
	return *this;
}

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber> MNumber<BaseNumber>::operator/(const MNumber<BaseNumber> &obj) const
{
	if(obj.inf == 0)
		return *this / obj.num;
	else
		throw std::runtime_error(ColoredString::red("M1 / M2 must be MNumber2 inf equal to 0"));
}

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber> MNumber<BaseNumber>::operator/(const BaseNumber &other_num) const noexcept
{
	return {inf / other_num, num / other_num};
}

template<Arithmetic BaseNumber>
inline std::string MNumber<BaseNumber>::to_string(const std::integral auto &d) const noexcept
{
	return std::to_string(d);
};

template<Arithmetic BaseNumber>
inline std::string MNumber<BaseNumber>::to_string(const FloatingPoint auto &d) const noexcept
{
	std::string num_text = std::to_string(d);
	auto r = num_text.substr(0, num_text.find('.') + 3);
	while(r.back() != '.') {
		if(r.back() == '0')
			r.pop_back();
		else
			break;
	}
	if(r.back() == '.')
		r.pop_back();
	return r;
};

template<Arithmetic BaseNumber>
inline std::string MNumber<BaseNumber>::to_string(const MNumber<BaseNumber> &m) noexcept
{
	return m;
}

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber> operator+(const BaseNumber &num, MNumber<BaseNumber> m)
{
	return m + num;
}

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber> operator*(const BaseNumber &num, MNumber<BaseNumber> m)
{
	return m * num;
}

template<Arithmetic BaseNumber>
inline bool operator>(const BaseNumber &num, MNumber<BaseNumber> m)
{
	return m <= num;
}

template<Arithmetic BaseNumber>
inline bool operator>=(const BaseNumber &num, MNumber<BaseNumber> m)
{
	return m < num;
}

template<Arithmetic BaseNumber>
inline bool operator<(const BaseNumber &num, MNumber<BaseNumber> m)
{
	return m >= num;
}

template<Arithmetic BaseNumber>
inline bool operator<=(const BaseNumber &num, MNumber<BaseNumber> m)
{
	return m > num;
}

template<Arithmetic BaseNumber>
inline bool operator==(const BaseNumber &num, MNumber<BaseNumber> m)
{
	return m == num;
}
