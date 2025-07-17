[![latest](https://img.shields.io/github/v/release/GyverLibs/StringN.svg?color=brightgreen)](https://github.com/GyverLibs/StringN/releases/latest/download/StringN.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/StringN.svg)](https://registry.platformio.org/libraries/gyverlibs/StringN)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/StringN?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# StringN
Лёгкий и быстрый статический String-билдер
- Статический буфер
- Быстрые и лёгкие функции конвертации чисел
- Поддержка 64-бит чисел
- В ~5 раз быстрее Arduino String
- На ~3 кБ легче Arduino String

### Совместимость
Совместима со всеми Arduino платформами (используются Arduino-функции)

## Содержание
- [Использование](#usage)
- [Версии](#versions)
- [Установка](#install)
- [Баги и обратная связь](#feedback)

<a id="usage"></a>

## Использование
### sbuild
Функции для ручной сборки строки в массиве

```cpp
uint16_t sbuild::addChar(char sym, char* buf, int16_t left = -1);
uint16_t sbuild::addChar(char sym, int16_t amount, char* buf, int16_t left = -1);
uint16_t sbuild::addPstr(const void* pstr, int16_t len, char* buf, int16_t left = -1);
uint16_t sbuild::addPstr(const void* pstr, char* buf, int16_t left = -1);
uint16_t sbuild::addStr(const char* str, int16_t len, char* buf, int16_t left = -1);
uint16_t sbuild::addStr(const char* str, char* buf, int16_t left = -1);
uint8_t sbuild::addUint(uint32_t v, uint8_t base, char* buf, int16_t left = -1);
uint8_t sbuild::addInt(int32_t v, uint8_t base, char* buf, int16_t left = -1);
uint8_t sbuild::addUint64(uint64_t v, uint8_t base, char* buf, int16_t left = -1);
uint8_t sbuild::addInt64(int64_t v, uint8_t base, char* buf, int16_t left = -1);
uint8_t sbuild::addFloat(float v, uint8_t dec, char* buf, int16_t left = -1);
```

Параметр `left` - оставшееся место в буфере, если он отрицательный - функция игнорирует ограничение длины. Пример использования:

```cpp
uint8_t maxlen = 21;
char str[maxlen + 1];

uint8_t len = 0;
len += sbuild::addChar('h', str + len, maxlen - len);
len += sbuild::addStr("ello", str + len, maxlen - len);
len += sbuild::addPstr(F(" world! "), str + len, maxlen - len);
len += sbuild::addInt(123, 10, str + len, maxlen - len);
len += sbuild::addFloat(456.789, 5, str + len, maxlen - len);

Serial.println(str);    // hello world! 123456.7
```

### StringN
Класс стринг билдера. Доступен вариант с ручным указанием размера:

```cpp
StringN<max_len>;
```

И несколько фиксированных:

```cpp
String8;
String16;
String24;
String32;
String64;
String128;
String256;
```

Описание класса:

```cpp
StringN();
StringN(любой_тип);
StringN(целый, основание);
StringN(float, знаков);
StringN(строка, длина);
StringN(символ, количество);

operator+(любой_тип);   // мутирующий!
operator+=(любой_тип);
operator=(любой_тип);

StringN& add(любой_тип);
StringN& add(целый, основание);
StringN& add(float, знаков);
StringN& add(строка, длина);
StringN& add(символ, количество);

// вывод, доступ
const char* c_str();
operator const char*();

// новая строка \r\n
StringN& rn();

// очистить
StringN& clear();

// хэш
uint32_t hash();

// текущая длина
uint16_t length();

// макс. длина
uint16_t capacity();

// строка заполнена (возможно текст обрезан)
bool isFull();
```

> [!NOTE]
> `base` для целых чисел поддерживается только `10`, `16` и `2`

Инструмент задуман для сборки строк и передачи в функции, которые принимают `const char*`. Например в печать, для создания файлов (генерация имён и путей), генерации строк с численными константами и так далее. Пример использования:

```cpp
Serial.println(String64('h') + "ello" + F(" world") + 12345 + true);

Serial.println(String32("val: ") + String8(3.1415, 3));

Serial.println(StringN<20>("val: ").add(3.1415, 3));
```

Примечания:
- Указанное число - максимальное количество читаемых символов (не считая терминатор)
- Оператор `+` - мутирующий, он изменяет исходную строку
- При сложении цепочкой первое слагаемое должно быть `StringX`. Если строка нужна вторым слагаемым - создаём её пустой: `foo(String64() + 123 + "hello")`

<a id="versions"></a>

## Версии
- v1.0

<a id="install"></a>
## Установка
- Библиотеку можно найти по названию **StringN** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/StringN/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Обновление
- Рекомендую всегда обновлять библиотеку: в новых версиях исправляются ошибки и баги, а также проводится оптимизация и добавляются новые фичи
- Через менеджер библиотек IDE: найти библиотеку как при установке и нажать "Обновить"
- Вручную: **удалить папку со старой версией**, а затем положить на её место новую. "Замену" делать нельзя: иногда в новых версиях удаляются файлы, которые останутся при замене и могут привести к ошибкам!

<a id="feedback"></a>

## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!

При сообщении о багах или некорректной работе библиотеки нужно обязательно указывать:
- Версия библиотеки
- Какой используется МК
- Версия SDK (для ESP)
- Версия Arduino IDE
- Корректно ли работают ли встроенные примеры, в которых используются функции и конструкции, приводящие к багу в вашем коде
- Какой код загружался, какая работа от него ожидалась и как он работает в реальности
- В идеале приложить минимальный код, в котором наблюдается баг. Не полотно из тысячи строк, а минимальный код