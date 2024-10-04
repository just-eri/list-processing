#include "list-handler.hpp"

#include <iostream>

int main()
{
  try
  {
    std::locale::global(std::locale("ru_RU.UTF-8"));
    std::wcout.imbue(std::locale("ru_RU.UTF-8"));

    ListHandler handler{};
    std::string line;
    while (std::getline(std::cin, line))
    {
      std::istringstream input(line);
      std::string command;
      input >> command;

      if (command == "add")
      {
        std::string name{};
        double posX{0.0};
        double posY{0.0};
        std::string type{};
        std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        input >> name >> posX >> posY >> type;

        handler.add({stringToWstring(name), posX, posY, stringToWstring(type), time});
      }
      else if (command == "group")
      {
        std::string method;
        input >> method;
        if (method == "distance")
        {
          std::wcout << handler.group(ListHandler::GroupingMethod::ByDistance);
        }
        else if (method == "name")
        {
          std::wcout << handler.group(ListHandler::GroupingMethod::ByName);
        }
        else if (method == "type")
        {
          std::wcout << handler.group(ListHandler::GroupingMethod::ByType);
        }
        else if (method == "time")
        {
          std::wcout << handler.group(ListHandler::GroupingMethod::ByTime);
        }
      }
      else if (command == "save")
      {
        std::string path;
        input >> path;
        handler.saveToFile(path);
      }
      else
      {
        std::wcout << L"Wrong command\n";
      }
    }
    
    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;

    return 1;
  }
}