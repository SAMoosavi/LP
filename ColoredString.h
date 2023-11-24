#ifndef LP_COLOREDSTRING_H
#define LP_COLOREDSTRING_H

#include <string>

class ColoredString
{
public:
	ColoredString() = delete;

	static std::string red(const std::string &s) noexcept;

	static std::string green(const std::string &s) noexcept;

	static std::string yellow(const std::string &s) noexcept;

	static std::string blue(const std::string &s) noexcept;

	static std::string magenta(const std::string &s) noexcept;

	static std::string white(const std::string &s) noexcept;
};

#endif
