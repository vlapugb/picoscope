# PicoScope

Программа запускает измерения с осциллографом PicoScope (серия 4000A), используя настройки из XML-файла и записывает данные по каналам в формате `.csv`.

## Зависимости
Необходимо предустановить следующие библиотеки для запуска программы:
- [`pugixml`](https://pugixml.org/) — парсинг XML
- [`libps4000a`](https://github.com/picotech/picosdk-c-examples/tree/master/ps4000a) — работа с PicoScope 4000A

## Сборка

```bash
mkdir build
cd build
cmake ..
make
