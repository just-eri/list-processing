#include "types.hpp"

#include <filesystem>

std::string wstringToString(const std::wstring& str)
{
  std::filesystem::path p(str);
  return p.string();
}

std::wstring stringToWstring(const std::string& str)
{
  std::filesystem::path p(str);
  return p.wstring();
}