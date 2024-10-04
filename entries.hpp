#ifndef EL_DATABASE_HPP
#define EL_DATABASE_HPP

#include <memory>
#include <string>
#include <vector>

#include <sqlite3.h>

#include "types.hpp"

class Entries
{
public:
  Entries(const std::string& name);

  void insert(const entry_t& entry);
  std::vector<entry_t> fetchAll() const;

private:
  std::unique_ptr<sqlite3, decltype(&sqlite3_close)> db_{nullptr, sqlite3_close};
};

#endif