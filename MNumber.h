#ifndef LP_MNUMBER_H
#define LP_MNUMBER_H

#include <string>

template<typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template<typename T>
concept FloatingPoint = std::is_floating_point_v<T>;

template<Arithmetic BaseNumber>
class MNumber {
public:
	inline MNumber();

	inline MNumber(BaseNumber num);

	inline MNumber(BaseNumber inf, BaseNumber num);

	inline MNumber(const MNumber<BaseNumber> &other);

	inline MNumber(MNumber<BaseNumber> &&other) noexcept;

	inline MNumber<BaseNumber> &operator=(const MNumber<BaseNumber> &other);

	inline MNumber<BaseNumber> &operator=(const BaseNumber &other_num);

	inline MNumber<BaseNumber> &operator=(MNumber<BaseNumber> &&other) noexcept;

	inline MNumber<BaseNumber> operator+(const MNumber<BaseNumber> &obj) const noexcept;

	inline MNumber<BaseNumber> operator+(const BaseNumber &obj) const noexcept;

	inline MNumber<BaseNumber> operator+=(const MNumber<BaseNumber> &obj) noexcept;

	inline MNumber<BaseNumber> operator+=(const BaseNumber &obj) noexcept;

	inline MNumber<BaseNumber> operator*(const MNumber<BaseNumber> &obj) const;

	inline MNumber<BaseNumber> operator*(const BaseNumber &obj) const noexcept;

	inline MNumber<BaseNumber> operator*=(const MNumber<BaseNumber> &obj);

	inline MNumber<BaseNumber> operator*=(const BaseNumber &obj) noexcept;

	inline MNumber<BaseNumber> operator/(const MNumber<BaseNumber> &obj) const;

	inline MNumber<BaseNumber> operator/(const BaseNumber &obj) const noexcept;

	inline MNumber<BaseNumber> operator-() const noexcept;

	inline MNumber<BaseNumber> operator-(const MNumber<BaseNumber> &obj) const noexcept;

	inline MNumber<BaseNumber> operator-(const BaseNumber &obj) const noexcept;

	inline MNumber<BaseNumber> &operator-=(const MNumber<BaseNumber> &obj) noexcept;

	inline MNumber<BaseNumber> &operator-=(const BaseNumber &obj) noexcept;

	inline bool operator==(const MNumber<BaseNumber> &other) const noexcept;

	inline bool operator==(const BaseNumber &other) const noexcept;

	inline bool operator>(const MNumber<BaseNumber> &other) const noexcept;

	inline bool operator>(const BaseNumber &other) const noexcept;

	inline bool operator>=(const MNumber<BaseNumber> &other) const noexcept;

	inline bool operator>=(const BaseNumber &other) const noexcept;

	inline bool operator<(const MNumber<BaseNumber> &other) const noexcept;

	inline bool operator<(const BaseNumber &other) const noexcept;

	inline bool operator<=(const MNumber<BaseNumber> &other) const noexcept;

	inline bool operator<=(const BaseNumber &other) const noexcept;

	inline operator std::string() const noexcept;

	inline static std::string to_string(const MNumber<BaseNumber> &m) noexcept;

private:
	inline std::string to_string(const FloatingPoint auto &d) const noexcept;

	inline std::string to_string(const std::integral auto &d) const noexcept;

	BaseNumber inf;
	BaseNumber num;
};

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber> operator+(const BaseNumber &num, MNumber<BaseNumber> m);

template<Arithmetic BaseNumber>
inline MNumber<BaseNumber> operator*(const BaseNumber &num, MNumber<BaseNumber> m);

template<Arithmetic BaseNumber>
inline bool operator>(const BaseNumber &num, MNumber<BaseNumber> m);

template<Arithmetic BaseNumber>
inline bool operator>=(const BaseNumber &num, MNumber<BaseNumber> m);

template<Arithmetic BaseNumber>
inline bool operator<(const BaseNumber &num, MNumber<BaseNumber> m);

template<Arithmetic BaseNumber>
inline bool operator<=(const BaseNumber &num, MNumber<BaseNumber> m);

template<Arithmetic BaseNumber>
inline bool operator==(const BaseNumber &num, MNumber<BaseNumber> m);

#include "MNumber-tmp.h"

#endif
