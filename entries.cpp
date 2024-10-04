#include "entries.hpp"

#include <format>
#include <string>
#include <stdexcept>

Entries::Entries(const std::string& name)
{
  sqlite3* tempDB;
  if (sqlite3_open(name.c_str(), &tempDB) != SQLITE_OK)
  {
    throw std::runtime_error("Can't open database: " + std::string(sqlite3_errmsg(tempDB)));
  }
  db_.reset(tempDB);
}

void Entries::insert(const entry_t& entry)
{
  char* errMessage = nullptr;
  std::string query = std::format("INSERT INTO ENTRIES (name, posX, posY, type, time) VALUES ('{}', {}, {}, '{}', {});",
                  wstringToString(entry.name), entry.pos_x, entry.pos_y, wstringToString(entry.type), entry.time);
  if (sqlite3_exec(db_.get(), query.c_str(), nullptr, nullptr, &errMessage) != SQLITE_OK)
  {
    std::string error = "SQL error: " + std::string(errMessage);
    sqlite3_free(errMessage);
    throw std::runtime_error(error);
  }
}

std::vector<entry_t> Entries::fetchAll() const
{
  std::vector<entry_t> res{};

  std::string query = "SELECT * FROM ENTRIES;";
  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(db_.get(), query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
  {
    throw std::runtime_error("Failed to prepare statement: " + std::string(sqlite3_errmsg(db_.get())));
  }

  while (sqlite3_step(stmt) == SQLITE_ROW)
  {
    entry_t e;
    e.name = stringToWstring(std::string{reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))});
    e.pos_x = sqlite3_column_double(stmt, 1);
    e.pos_y = sqlite3_column_double(stmt, 2);
    e.type = stringToWstring(std::string{reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))});
    double time = sqlite3_column_double(stmt, 4);
    e.time = std::time_t(time);

    res.push_back(e);
  }

  return res;
}