#include "sbuild.h"

#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#if defined(__AVR__) || defined(ESP8266)
#include <avr/pgmspace.h>
#define SB_HAS_PGM
#elif defined(ARDUINO_ARCH_ESP32)
#include <pgmspace.h>
#define SB_HAS_PGM
#else
//
#endif

namespace sbuild {

uint16_t addChar(char sym, char* buf, int16_t left) {
    if (left) {
        *buf++ = sym;
        *buf = 0;
        return 1;
    }
    return 0;
}

uint16_t addChar(char sym, int16_t amount, char* buf, int16_t left) {
    if (left >= 0 && amount > left) amount = left;
    memset(buf, sym, amount);
    buf[amount] = 0;
    return amount;
}

uint16_t addPstr(const void* pstr, int16_t len, char* buf, int16_t left) {
#ifdef SB_HAS_PGM
    if (left >= 0 && len > left) len = left;
    memcpy_P(buf, pstr, len);
    buf[len] = 0;
    return len;
#else
    return addStr((const char*)pstr, len, buf, left);
#endif
}

uint16_t addPstr(const void* pstr, char* buf, int16_t left) {
#ifdef SB_HAS_PGM
    return addPstr(pstr, strlen_P((PGM_P)pstr), buf, left);
#else
    return addStr((const char*)pstr, buf, left);
#endif
}

uint16_t addStr(const char* str, int16_t len, char* buf, int16_t left) {
    if (left >= 0 && len > left) len = left;
    memcpy(buf, str, len);
    buf[len] = 0;
    return len;
}
uint16_t addStr(const char* str, char* buf, int16_t left) {
    return addStr(str, strlen(str), buf, left);
}

uint8_t addUint(uint32_t v, uint8_t base, char* buf, int16_t left) {
    if (!left || !(base == 10 || base == 16 || base == 2)) return 0;

    if (v < 10 && base == 10) {
        *buf++ = v + '0';
        *buf = 0;
        return 1;
    }

    struct fdiv10 {
        uint32_t div(uint32_t num) {
            quot = num >> 1;
            quot += quot >> 1;
            quot += quot >> 4;
            quot += quot >> 8;
            quot += quot >> 16;
            uint32_t qq = quot;
            quot >>= 3;
            rem = uint8_t(num - ((quot << 1) + (qq & ~7ul)));
            if (rem > 9) rem -= 10, ++quot;
            return quot;
        }

        uint32_t quot = 0;
        uint8_t rem = 0;
    };

    char* p = buf;

    if (base == 10) {
        fdiv10 fdiv;
        do {
            v = fdiv.div(v);
            *p++ = fdiv.rem + '0';
        } while (v && --left);
    } else {
        do {
            uint8_t c = v & (base - 1);
            v >>= (base == 16) ? 4 : 1;
            *p++ = (c < 10) ? (c + '0') : (c + 'a' - 10);
        } while (v && --left);
    }
    *p = 0;

    uint8_t len = p - buf;
    --p;
    while (p > buf) {
        char tmp = *buf;
        *buf++ = *p;
        *p-- = tmp;
    }
    return len;
}

uint8_t addInt(int32_t v, uint8_t base, char* buf, int16_t left) {
    if (v < 0 && base == 10) {
        if (!left) return 0;
        *buf = '-';
        return 1 + addUint(-v, base, buf + 1, left - 1);
    }
    return addUint(v, base, buf, left);
}

uint8_t addUint64(uint64_t v, uint8_t base, char* buf, int16_t left) {
    if (!left || !(base == 10 || base == 16 || base == 2)) return 0;

    if (v <= UINT32_MAX) return addUint(v, base, buf, left);

    char* p = buf;

    if (base == 10) {
        do {
            *p++ = (v % 10) + '0';
            v /= 10;
        } while (v && --left);
    } else {
        do {
            uint8_t c = v & (base - 1);
            v >>= (base == 16) ? 4 : 1;
            *p++ = (c < 10) ? (c + '0') : (c + 'a' - 10);
        } while (v && --left);
    }
    *p = 0;

    uint8_t len = p - buf;
    --p;
    while (p > buf) {
        char tmp = *buf;
        *buf++ = *p;
        *p-- = tmp;
    }
    return len;
}

uint8_t addInt64(int64_t v, uint8_t base, char* buf, int16_t left) {
    if (v > INT32_MIN && v <= INT32_MAX) return addInt(v, base, buf, left);

    if (v < 0 && base == 10) {
        if (!left) return 0;
        *buf = '-';
        return 1 + addUint64(-v, base, buf + 1, left - 1);
    }
    return addUint64(v, base, buf, left);
}

uint8_t addFloat(float v, uint8_t dec, char* buf, int16_t left) {
    if (isnan(v)) return addStr("nan", 3, buf, left);
    if (isinf(v)) return addStr("inf", 3, buf, left);

    char* p = buf;

    if (v < 0.0) {
        if (left) *p++ = '-', --left;
        v = -v;
    }

    float rounding = 0.5;
    for (uint8_t i = 0; i < dec; ++i) rounding /= 10.0;

    v += rounding;
    uint8_t res = addUint(v, 10, p, left);
    p += res;
    left -= res;

    if (dec) {
        if (left) *p++ = '.', --left;

        float rem = v - uint32_t(v);

        while (dec--) {
            rem *= 10.0;
            if (left) *p++ = uint8_t(rem) + '0', --left;
            rem -= uint8_t(rem);
        }
    }

    *p = 0;
    return p - buf;
}

}  // namespace sbuild