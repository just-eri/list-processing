#ifndef EL_SERVER_HPP
#define EL_SERVER_HPP

#include <memory>
#include <string>
#include <vector>

#include "entries.hpp"
#include "types.hpp"

class ListHandler
{
public:
  enum class GroupingMethod
  {
    ByDistance,
    ByName,
    ByType,
    ByTime,
  };

  enum class Timestamp
  {
    Today,
    Yesterday,
    ThisWeek,
    ThisMonth,
    ThisYear,
    LongAgo
  };

  enum class Distance
  {
    Under100,
    Under1000,
    Under10000,
    TooFarAway,
  };

  ListHandler(const std::string& path);

  void add(const entry_t& entry);
  void saveToFile(const std::string& path) const;
  std::wstring group(const GroupingMethod& method) const;

  std::vector<entry_t> getEntries();

private:
  Entries entries_;
  std::shared_ptr<std::vector<entry_t>> entryList_;

  std::wstring groupByDistance() const;
  std::wstring groupByName() const;
  std::wstring groupByType() const;
  std::wstring groupByTime() const;

  Distance checkDistance(double posX, double posY) const;
  std::wstring getDistanceName(const Distance& distance) const;

  bool isRULetter(wchar_t letter) const;

  Timestamp checkTime(const std::time_t& timestamp) const;
  std::wstring getTimeName(const Timestamp& timestamp) const;
};

#endif