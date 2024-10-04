# Тестовое задание, обработка списка объектов

Сборка программы:

    c++ types.cpp entries.cpp list-handler.cpp main.cpp -o ./listhandler -lsqlite3 --std=c++20

Запуск программы:

    ./listhandler <путь к файлу бд SQLITE3>

## Команды внутри программы:


Добавление:

    add <имя> <коорд. Х> <коорд. Y> <тип> #добавить запись, время будет сгенерировано на основе текущего времени
 Пример:
 

    add Александр 3,4 108,9 Человек

Группировка:

    group <метод группировки> #возможные варианты группировки - distance, name, type, time

Сохранение в текстовый файл:

    save <путь к файлу> #группирует данные по очереди всеми методами и сохраняет в файл результат



#### Время выполнения - с 11:00 по 16:00 мск 04.10.2024
