#ifndef EL_TYPES_HPP
#define EL_TYPES_HPP

#include <chrono>
#include <string>

struct entry_t
{
  std::wstring name;
  double pos_x;
  double pos_y;
  std::wstring type;
  std::time_t time;
};

std::string wstringToString(const std::wstring& str);
std::wstring stringToWstring(const std::string& str);

#endif