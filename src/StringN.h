#pragma once
#include "sbuild.h"

template <uint16_t maxlen>
class StringN {
   public:
    static_assert(maxlen <= INT16_MAX, "StringN capacity exceeds INT16_MAX");

    StringN() = default;
    StringN(const StringN& str) = default;
    StringN& operator=(const StringN& str) = default;

    StringN(char c) {
        add(c);
    }
    StringN(char c, int16_t amount) {
        add(c, amount);
    }
    StringN(const char* str) {
        add(str);
    }
    StringN(const char* str, int16_t len) {
        add(str, len);
    }

#ifdef ARDUINO
    StringN(const __FlashStringHelper* fstr) {
        add(fstr);
    }
    StringN(const __FlashStringHelper* fstr, int16_t len) {
        add(fstr, len);
    }
#endif

    template <uint16_t N>
    StringN(const StringN<N>& str) {
        add(str);
    }

    template <typename T>
    StringN(T val) {
        add(val);
    }

    template <typename T, typename P>
    StringN(T val, P p) {
        add(val, (uint8_t)p);
    }

    // ============== OPERATOR ==============

    // + val
    template <typename T>
    StringN& operator+(T val) {
        return add(val);
    }
    template <uint16_t N>
    StringN& operator+(const StringN<N>& other) {
        return add<N>(other);
    }

    // += val
    template <typename T>
    StringN& operator+=(T val) {
        return add(val);
    }
    template <uint16_t N>
    StringN& operator+=(const StringN<N>& other) {
        return add<N>(other);
    }

    // = val
    template <typename T>
    StringN& operator=(T val) {
        clear();
        return add(val);
    }

    // ============== StringN ==============
    template <uint16_t N>
    StringN& add(const StringN<N>& str) {
        return add(str.c_str(), str.length());
    }

    // ============== CHAR ==============
    StringN& add(char c) {
        if (_len < maxlen) {
            _buf[_len++] = c;
            _buf[_len] = 0;
        }
        return *this;
    }
    StringN& add(char c, int16_t amount) {
        _len += sbuild::addChar(c, amount, _buf + _len, _left());
        return *this;
    }

    // ============== STRING ==============
    StringN& add(const char* str) {
        _len += sbuild::addStr(str, _buf + _len, _left());
        return *this;
    }
    StringN& add(const char* str, int16_t len) {
        _len += sbuild::addStr(str, len, _buf + _len, _left());
        return *this;
    }

#ifdef ARDUINO
    StringN& add(const __FlashStringHelper* fstr) {
        _len += sbuild::addPstr(fstr, _buf + _len, _left());
        return *this;
    }
    StringN& add(const __FlashStringHelper* fstr, int16_t len) {
        _len += sbuild::addPstr(fstr, len, _buf + _len, _left());
        return *this;
    }
#endif

    // ============== BOOL ==============
    StringN& add(bool v) {
        return add(v ? '1' : '0');
    }

    // ============== INT ==============
    StringN& add(unsigned char v, uint8_t base = 10) {
        return add((unsigned long)v, base);
    }
    StringN& add(unsigned short v, uint8_t base = 10) {
        return add((unsigned long)v, base);
    }
    StringN& add(unsigned int v, uint8_t base = 10) {
        return add((unsigned long)v, base);
    }
    StringN& add(unsigned long v, uint8_t base = 10) {
#ifndef STRN_DEFAULT_INT
        _len += sbuild::addUint(v, base, _buf + _len, _left());
        return *this;
#else
        char temp[33];
        ultoa(v, temp, base);
        return add(temp);
#endif
    }
    StringN& add(unsigned long long v, uint8_t base = 10) {
        _len += sbuild::addUint64(v, base, _buf + _len, _left());
        return *this;
    }

    StringN& add(signed char v, uint8_t base = 10) {
        return add((long)v, base);
    }
    StringN& add(short v, uint8_t base = 10) {
        return add((long)v, base);
    }
    StringN& add(int v, uint8_t base = 10) {
        return add((long)v, base);
    }
    StringN& add(long v, uint8_t base = 10) {
#ifndef STRN_DEFAULT_INT
        _len += sbuild::addInt(v, base, _buf + _len, _left());
        return *this;
#else
        char temp[33];
        ltoa(v, temp, base);
        return add(temp);
#endif
    }
    StringN& add(long long v, uint8_t base = 10) {
        _len += sbuild::addInt64(v, base, _buf + _len, _left());
        return *this;
    }

    // ============== FLOAT ==============
    StringN& add(float v, uint8_t dec = 2) {
#ifndef STRN_DEFAULT_FLOAT
        _len += sbuild::addFloat(v, dec, _buf + _len, _left());
        return *this;
#else
        char temp[32];
        dtostrf(v, 0, dec, temp);
        return add(temp);
#endif
    }
    StringN& add(double v, uint8_t dec = 2) {
        return add(float(v), dec);
    }

    // ============== EXPORT ==============
    char* c_str() {
        return _buf;
    }
    operator char*() {
        return _buf;
    }

    const char* c_str() const {
        return _buf;
    }
    operator const char*() const {
        return _buf;
    }

    // ============== MISC ==============
    // новая строка \r\n
    StringN& rn() {
        add('\r');
        return add('\n');
    }

    // очистить
    StringN& clear() {
        _buf[0] = 0;
        _len = 0;
        return *this;
    }

    // хэш
    uint32_t hash() const {
        uint32_t res = 0;
        uint16_t len = _len;
        const char* p = _buf;
        while (len--) res = res + (res << 5) + *p++;
        return res;
    }

    // текущая длина
    uint16_t length() const {
        return _len;
    }

    // макс. длина
    uint16_t capacity() const {
        return maxlen;
    }

    // строка заполнена (возможно текст обрезан)
    bool isFull() const {
        return _len == maxlen;
    }

   private:
    char _buf[maxlen + 1] = {};
    uint16_t _len = 0;

    int16_t _left() const {
        return maxlen - _len;
    }
};

using String8 = StringN<8>;
using String16 = StringN<16>;
using String24 = StringN<24>;
using String32 = StringN<32>;
using String64 = StringN<64>;
using String128 = StringN<128>;
using String256 = StringN<256>;
