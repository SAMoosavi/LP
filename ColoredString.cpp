#include "ColoredString.h"

string ColoredString::red(const string & s) noexcept {return "\033[1;31m" + s + "\033[0m";}

string ColoredString::green(const string & s) noexcept {return "\033[1;32m" + s + "\033[0m";}

string ColoredString::yellow(const string & s) noexcept {return "\033[1;33m" + s + "\033[0m";}

string ColoredString::blue(const string & s) noexcept {return "\033[1;34m" + s + "\033[0m";}

string ColoredString::magenta(const string & s) noexcept {return "\033[1;35m" + s + "\033[0m";}

string ColoredString::white(const string & s) noexcept {return "\033[1;37m" + s + "\033[0m";}


