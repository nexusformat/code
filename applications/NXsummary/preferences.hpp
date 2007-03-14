#ifndef __PREFERENCES_HPP_GUARD__
#define __PREFERENCES_HPP_GUARD__

#include <string>
#include <vector>
#include "nxsummary.hpp"

namespace nxsum {
  void loadPreferences(std::string &filename, std::vector<Item> &preferences);
  void writePreferences(std::string &filename, std::vector<Item> &preferences);
}

#endif
