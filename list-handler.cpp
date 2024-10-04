#include "list-handler.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>

constexpr std::size_t MINIMUM_ENTRIES = 2;

ListHandler::ListHandler() :
  entries_("test.db"),
  entryList_(std::make_shared<std::vector<entry_t>>(entries_.fetchAll()))
{

}

void ListHandler::add(const entry_t& entry)
{
  entries_.insert(entry);
}

void ListHandler::saveToFile(const std::string& path) const
{
  std::wofstream output(path);

  if (output.is_open())
  {
    output << L"Группировка по расстоянию:\n";
    output << groupByDistance();
    output << L"----------------------------\n";
    output << L"Группировка по имени:\n";
    output << groupByName();
    output << L"----------------------------\n";
    output << L"Группировка по типу:\n";
    output << groupByType();
    output << L"----------------------------\n";
    output << L"Группировка по времени:\n";
    output << groupByTime();
    output << L"----------------------------\n";
    output.close();
  }
}

std::wstring ListHandler::group(const ListHandler::GroupingMethod& method) const
{
  switch (method)
  {
  case GroupingMethod::ByDistance:
    return groupByDistance();
    break;
  
  case GroupingMethod::ByName:
    return groupByName();
    break;
  
  case GroupingMethod::ByType:
    return groupByType();
    break;
  
  case GroupingMethod::ByTime:
    return groupByTime();
    break;
  
  default:
    return L"Error";
    break;
  }
}

std::wstring ListHandler::groupByDistance() const
{
  auto list = entries_.fetchAll();
  std::map<Distance, std::vector<entry_t>> distanceGroups;

  for (const auto& it: list)
  {
    Distance dist = checkDistance(it.pos_x, it.pos_y);
    distanceGroups[dist].push_back(it);
  }

  std::wstring res{};
  for (auto& pair: distanceGroups)
  {
    res += std::format(L"{}:\n", getDistanceName(pair.first));
    std::sort(pair.second.begin(), pair.second.end(), [this](const entry_t& lhs, const entry_t& rhs)
    {
      double lhsDistance = std::sqrt(lhs.pos_x * lhs.pos_x + lhs.pos_y * lhs.pos_y);
      double rhsDistance = std::sqrt(rhs.pos_x * rhs.pos_x + rhs.pos_y * rhs.pos_y);
      return lhsDistance < rhsDistance;
    });
    for (const auto& e: pair.second)
    {
      res += std::format(L"{}, {}, {}, {}, {}\n", e.name, e.pos_x, e.pos_y, e.time, e.type);
    }
  }

  return res;
}

std::wstring ListHandler::groupByName() const
{
  auto list = entries_.fetchAll();
  std::map<wchar_t, std::vector<entry_t>> nameGroups;

  for (const auto& it: list)
  {
    wchar_t firstLetter = std::toupper(it.name.at(0));
    if (isRULetter(firstLetter))
    {
      nameGroups[firstLetter].push_back(it);
    }
    else
    {
      nameGroups['#'].push_back(it);
    }
  }

  std::wstring res{};
  for (auto& pair: nameGroups)
  {
    res += std::format(L"{}:\n", pair.first);
    std::sort(pair.second.begin(), pair.second.end(), [](const entry_t& lhs, const entry_t& rhs)
    {
      return lhs.name < rhs.name;
    });
    for (const auto& e: pair.second)
    {
      res += std::format(L"{}, {}, {}, {}, {}\n", e.name, e.pos_x, e.pos_y, e.time, e.type);
    }
  }

  return res;
}

std::wstring ListHandler::groupByType() const
{
  auto list = entries_.fetchAll();

  std::map<std::wstring, std::vector<entry_t>> typeGroups;
  for (const auto& it: list)
  {
    typeGroups[it.type].push_back(it);
  }

  std::wstring res{};
  std::vector<entry_t> other{};
  for (auto& pair: typeGroups)
  {
    if (pair.second.size() >= MINIMUM_ENTRIES)
    {
      res += std::format(L"{}:\n", pair.first);
      std::sort(pair.second.begin(), pair.second.end(), [](const entry_t& lhs, const entry_t& rhs)
      {
        return lhs.name < rhs.name;
      });
      for (const auto& e: pair.second)
      {
        res += std::format(L"{}, {}, {}, {}, {}\n", e.name, e.pos_x, e.pos_y, e.time, e.type);
      }
    }
    else
    {
      for (const auto& e: pair.second)
      {
        other.push_back(e);
      }
    }
  }

  if (!other.empty())
  {
    res += L"Разное:\n";
    std::sort(other.begin(), other.end(), [](const entry_t& lhs, const entry_t& rhs)
    {
      return lhs.name < rhs.name;
    });
    for (const auto& e: other)
    {
      res += std::format(L"{}, {}, {}, {}, {}\n", e.name, e.pos_x, e.pos_y, e.time, e.type);
    }
  }
  return res;
}

std::wstring ListHandler::groupByTime() const
{
  auto list = entries_.fetchAll();
  std::map<Timestamp, std::vector<entry_t>> timeGroups;

  for (const auto& it: list)
  {
    Timestamp ts = checkTime(it.time);
    timeGroups[ts].push_back(it);
  }

  std::wstring res{};
  for (auto& pair: timeGroups)
  {
    res += std::format(L"{}:\n", getTimeName(pair.first));
    std::sort(pair.second.begin(), pair.second.end(), [](const entry_t& lhs, const entry_t& rhs)
    {
      return lhs.time > rhs.time;
    });
    for (const auto& e: pair.second)
    {
      res += std::format(L"{}, {}, {}, {}, {}\n", e.name, e.pos_x, e.pos_y, e.time, e.type);
    }
  }

  return res;
}

ListHandler::Distance ListHandler::checkDistance(double posX, double posY) const
{
  double distance = std::sqrt((posX * posX) + (posY * posY));

  if (distance < 100.0)
  {
    return Distance::Under100;
  }
  else if (distance < 1000.0)
  {
    return Distance::Under1000;
  }
  else if (distance < 10000.0)
  {
    return Distance::Under10000;
  }
  else
  {
    return Distance::TooFarAway;
  }
}

std::wstring ListHandler::getDistanceName(const ListHandler::Distance& distance) const
{
  switch (distance)
  {
  case Distance::Under100:
    return L"До 100 ед.";
    break;
  
  case Distance::Under1000:
    return L"До 1000 ед.";
    break;
  
  case Distance::Under10000:
    return L"До 10000 ед.";
    break;
  
  case Distance::TooFarAway:
    return L"Слишком далеко";
    break;
  
  default:
    return L"Error";
    break;
  }
}

bool ListHandler::isRULetter(wchar_t letter) const
{
  return (letter >= 0x410 && letter <= 0x42F) || letter == 0x0401; 
}

ListHandler::Timestamp ListHandler::checkTime(const std::time_t& timestamp) const
{
  std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

  std::tm nowTm = *std::localtime(&now);
  std::tm tsTm = *std::localtime(&timestamp);

  std::tm startOfToday = nowTm;
  startOfToday.tm_hour = 0;
  startOfToday.tm_min = 0;
  startOfToday.tm_sec = 0;
  std::tm startOfTs = tsTm;
  startOfTs.tm_hour = 0;
  startOfTs.tm_min = 0;
  startOfTs.tm_sec = 0;

  int daysDiff = std::chrono::duration_cast<std::chrono::hours>(
    std::chrono::system_clock::from_time_t(std::mktime(&startOfToday)) - std::chrono::system_clock::from_time_t(std::mktime(&startOfTs))
  ).count() / 24;

  if (daysDiff == 0)
  {
    return Timestamp::Today;
  }

  if (daysDiff == 1)
  {
    return Timestamp::Yesterday;
  }

  std::tm startOfWeekTm = nowTm;
  startOfWeekTm.tm_mday -= (nowTm.tm_wday == 0 ? 6 : nowTm.tm_wday - 1);
  std::time_t startOfWeek = std::mktime(&startOfWeekTm);
  if (timestamp >= startOfWeek)
  {
      return Timestamp::ThisWeek;
  }

  if (nowTm.tm_year == tsTm.tm_year)
  {
    if (nowTm.tm_mon == tsTm.tm_mon)
    {
      return Timestamp::ThisMonth;
    }
    else
    {
      return Timestamp::ThisYear;
    }
  }
  else
  {
    return Timestamp::LongAgo;
  }
}

std::wstring ListHandler::getTimeName(const ListHandler::Timestamp& timestamp) const
{
  switch (timestamp)
  {
  case Timestamp::Today:
    return L"Сегодня";
    break;
  
  case Timestamp::Yesterday:
    return L"Вчера";
    break;
  
  case Timestamp::ThisWeek:
    return L"На этой неделе";
    break;
  
  case Timestamp::ThisMonth:
    return L"В этом месяце";
    break;
  
  case Timestamp::ThisYear:
    return L"В этом году";
    break;
  
  case Timestamp::LongAgo:
    return L"Ранее";
    break;
  
  default:
    return L"Error";
    break;
  }
}

std::vector<entry_t> ListHandler::getEntries()
{
  return entries_.fetchAll();
}


