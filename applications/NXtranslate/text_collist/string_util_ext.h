#ifndef __STRING_UTIL_EXT_H
#define __STRING_UTIL_EXT_H
#include <string>
#include <vector>
#include <stdexcept>

namespace string_util{
	extern std::vector<std::string> string_util::split_values(const std::string &str);
	extern std::vector<std::string> string_util::split_whitespace(const std::string &);
	extern std::vector<std::string> string_util::strip_punct(std::vector<std::string> &strvec);
}
#endif
