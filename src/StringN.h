#pragma once
#include "sbuild.h"

template <uint16_t maxlen>
class StringN {
   public:
    StringN() = default;
    StringN(const StringN& str) = default;
    StringN& operator=(const StringN& str) = default;

    template <typename T>
    StringN(T val) {
        add(val);
    }

    template <typename T>
    StringN(T val, uint16_t p) {
        add(val, p);
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
    StringN& add(char c, uint16_t amount) {
        _len += sbuild::addChar(c, amount, _buf + _len, maxlen - _len);
        return *this;
    }

    // ============== STRING ==============
    StringN& add(const char* str) {
        _len += sbuild::addStr(str, _buf + _len, maxlen - _len);
        return *this;
    }
    StringN& add(const char* str, uint16_t len) {
        _len += sbuild::addStr(str, len, _buf + _len, maxlen - _len);
        return *this;
    }

#ifdef ARDUINO
    StringN& add(const __FlashStringHelper* fstr) {
        _len += sbuild::addPstr(fstr, _buf + _len, maxlen - _len);
        return *this;
    }
    StringN& add(const __FlashStringHelper* fstr, uint16_t len) {
        _len += sbuild::addPstr(fstr, len, _buf + _len, maxlen - _len);
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
        _len += sbuild::addUint(v, base, _buf + _len, maxlen - _len);
        return *this;
#else
        char temp[33];
        ultoa(v, temp, base);
        return add(temp);
#endif
    }
    StringN& add(unsigned long long v, uint8_t base = 10) {
        _len += sbuild::addUint64(v, base, _buf + _len, maxlen - _len);
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
        _len += sbuild::addInt(v, base, _buf + _len, maxlen - _len);
        return *this;
#else
        char temp[33];
        ltoa(v, temp, base);
        return add(temp);
#endif
    }
    StringN& add(long long v, uint8_t base = 10) {
        _len += sbuild::addInt64(v, base, _buf + _len, maxlen - _len);
        return *this;
    }

    // ============== FLOAT ==============
    StringN& add(float v, uint8_t dec = 2) {
#ifndef STRN_DEFAULT_FLOAT
        _len += sbuild::addFloat(v, dec, _buf + _len, maxlen - _len);
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
};

using String8 = StringN<8>;
using String16 = StringN<16>;
using String24 = StringN<24>;
using String32 = StringN<32>;
using String64 = StringN<64>;
using String128 = StringN<128>;
using String256 = StringN<256>;