#pragma once

#include <cctype>
#include <string>

namespace helper {
inline bool IsNumber(const std::string &s) {
  if (s.empty()) return false;
  for (char c : s)
    if (!isdigit(c)) return false;
  return true;
}
}  // namespace helper
