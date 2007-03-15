#ifndef __PREFERENCES_HPP_GUARD__
#define __PREFERENCES_HPP_GUARD__

#include <string>
#include <vector>
#include "nxsummary.hpp"

namespace nxsum {
  void loadPreferences(const std::string &filename, std::vector<Item> &preferences);
  void writePreferences(const std::string &filename, const std::vector<Item> &preferences);
  Item getPreference(const std::string &label, const std::vector<Item> &preferences);
}
#endif
