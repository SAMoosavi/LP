#ifndef LP_COLOREDSTRING_H
#define LP_COLOREDSTRING_H

#include <string>

using std::string;

class ColoredString {
public:
	ColoredString() = delete;

	static string red(const string &s) noexcept;

	static string green(const string &s) noexcept;

	static string yellow(const string &s) noexcept;

	static string blue(const string &s) noexcept;

	static string magenta(const string &s) noexcept;

	static string white(const string &s) noexcept;
};


#endif
