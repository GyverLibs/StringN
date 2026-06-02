This is an automatic translation and may be incorrect in some places. See the source README and examples for authoritative information.

[![latest](https://img.shields.io/github/v/release/GyverLibs/StringN.svg?color=brightgreen)](https://github.com/GyverLibs/StringN/releases/latest/download/StringN.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/StringN.svg)](https://registry.platformio.org/libraries/gyverlibs/StringN)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/StringN?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# StringN
Lightweight and fast static String builder
- Static buffer
- Fast and easy number conversion functions
- Support for 64-bit numbers
- 5 times faster than Arduino String
- ~3 kB lighter than Arduino String

### Compatibility
Compatible with all Arduino platforms (Arduino features are used)

## Contents
- [Use of use](#usage)
- [Versions](#versions)
- [Installation](#install)
- [Bugs and feedback](#feedback)

<a id="usage"></a>

## Use of use
### sbuild
Functions for manual string assembly in an array

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

Parameter`left`The remaining space in the buffer, if negative, ignores the length limit. Example of use:

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

or

```cpp
uint8_t maxlen = 21;
char str[maxlen + 1];
char* p = str;
p += sbuild::addChar('h', p, maxlen - (p - str));
p += sbuild::addStr("ello", p, maxlen - (p - str));
p += sbuild::addPstr(F(" world! "), p, maxlen - (p - str));
p += sbuild::addInt(123, 10, p, maxlen - (p - str));
p += sbuild::addFloat(456.789, 5, p, maxlen - (p - str));
Serial.println(str);
```

### StringN
String building class. Available option with manual size indication:

```cpp
StringN<max_len>;
```

And some fixed ones:

```cpp
String8;
String16;
String24;
String32;
String64;
String128;
String256;
```

Class description:

```cpp
StringN();
StringN(любой_тип);
StringN(целый, основание);
StringN(float, знаков);
StringN(строка, длина);
StringN(символ, количество);

operator+(любой_тип);   // mutant!
operator+=(любой_тип);
operator=(любой_тип);

StringN& add(любой_тип);
StringN& add(целый, основание);
StringN& add(float, знаков);
StringN& add(строка, длина);
StringN& add(символ, количество);

// exit
const char* c_str();
operator const char*();

// newline
StringN& rn();

// cleanse
StringN& clear();

// hash
uint32_t hash();

// current-length
uint16_t length();

// Max. length
uint16_t capacity();

// The line is filled (the text may be cropped)
bool isFull();
```

> [!NOTE]
> `base`For integers only supported`10`, `16`and`2`

The tool is conceived to assemble strings and transfer to functions that take`const char*`. For example, printing, creating files (name and path generation), generating strings with numerical constants, and so on. Example of use:

```cpp
Serial.println(String64('h') + "ello" + F(" world") + 12345 + true);

Serial.println(String32("val: ") + String8(3.1415, 3));

Serial.println(StringN<20>("val: ").add(3.1415, 3));
```

Notes:
- The maximum number of characters read (excluding the terminator)
- Operator.`+`- mutating, it changes the original line
- When the chain is added, the first component must be`StringX`. If you need the second term, create it empty:`foo(String64() + 123 + "hello")`

By default, the author’s implementation of the functions of converting numbers into a string is used, it can be disabled by defiances:

```cpp
#define STRN_DEFAULT_FLOAT  // will use dtostrf: 1.2k heavier, 20% faster
#define STRN_DEFAULT_INT    // will use ltoa/ultoa: much slower
```

<a id="versions"></a>

## Versions
- v1.0

<a id="install"></a>
## Installation
- The library can be found by the name **StringN** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download the library](https://github.com/GyverLibs/StringN/archive/refs/heads/main.zip).zip archive for manual installation:
    - Unpack and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unpack and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/ *
    - (Arduino IDE) Automatic installation from .zip: *Sketch/Connect library/Add .ZIP library...* and specify downloaded archive
- Read more detailed instructions for installing libraries[here](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: new versions fix errors and bugs, as well as optimize and add new features.
- Through the library manager IDE: find the library as when installing and click "Update"
- Manually: **Delete the folder with the old version** and then put the new one in its place. “Replacement” can not be done: sometimes new versions delete files that will remain when replaced and can lead to errors!

<a id="feedback"></a>

## Bugs and feedback
If you find bugs, create **Issue**, or better write to the mail immediately.[alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
The library is open for revision and your **Pull Requests*!

When reporting bugs or incorrect work of the library, it is necessary to specify:
- Library version
- What is used by the IC
- SDK version (for ESP)
- Arduino IDE version
- Are embedded examples that use features and designs that cause bugs in your code working correctly?
- What code was downloaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed. Not a canvas of a thousand lines, but a minimum code.
