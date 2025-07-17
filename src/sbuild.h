#pragma once
#include <stdint.h>

namespace sbuild {

uint16_t addChar(char sym, char* buf, int16_t left = -1);
uint16_t addChar(char sym, int16_t amount, char* buf, int16_t left = -1);
uint16_t addPstr(const void* pstr, int16_t len, char* buf, int16_t left = -1);
uint16_t addPstr(const void* pstr, char* buf, int16_t left = -1);
uint16_t addStr(const char* str, int16_t len, char* buf, int16_t left = -1);
uint16_t addStr(const char* str, char* buf, int16_t left = -1);
uint8_t addUint(uint32_t v, uint8_t base, char* buf, int16_t left = -1);
uint8_t addInt(int32_t v, uint8_t base, char* buf, int16_t left = -1);
uint8_t addUint64(uint64_t v, uint8_t base, char* buf, int16_t left = -1);
uint8_t addInt64(int64_t v, uint8_t base, char* buf, int16_t left = -1);
uint8_t addFloat(float v, uint8_t dec, char* buf, int16_t left = -1);

}  // namespace sbuild