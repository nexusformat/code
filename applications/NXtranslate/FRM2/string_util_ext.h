#ifndef __STRING_UTIL_EXT_H
#define __STRING_UTIL_EXT_H
#include <string>
#include <vector>
#include <stdexcept>

namespace string_util{
	extern std::vector<std::string> split_values(const std::string &str);
	extern std::vector<unsigned int> split_ints(const std::string &str);
	extern std::vector<double> split_doubles(const std::string &str);
	extern std::vector<std::string> split_whitespace(const std::string &);
	extern std::vector<std::string> split_colons(const std::string &);
	extern std::vector<std::string> strip_punct(std::vector<std::string> &strvec);
	extern bool contains(std::string &str, std::string substr);
	extern std::string lower_str(std::string str);
}
#endif
