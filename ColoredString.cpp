#include "ColoredString.h"

#include <string>

std::string ColoredString::red(const std::string &s) noexcept
{
	return "\033[1;31m" + s + "\033[0m";
}

std::string ColoredString::green(const std::string &s) noexcept
{
	return "\033[1;32m" + s + "\033[0m";
}

std::string ColoredString::yellow(const std::string &s) noexcept
{
	return "\033[1;33m" + s + "\033[0m";
}

std::string ColoredString::blue(const std::string &s) noexcept
{
	return "\033[1;34m" + s + "\033[0m";
}

std::string ColoredString::magenta(const std::string &s) noexcept
{
	return "\033[1;35m" + s + "\033[0m";
}

std::string ColoredString::white(const std::string &s) noexcept
{
	return "\033[1;37m" + s + "\033[0m";
}
