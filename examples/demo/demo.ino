#include <Arduino.h>
#include <StringN.h>

void setup() {
    Serial.begin(115200);

    // sbuild
    uint8_t maxlen = 21;
    char str[maxlen + 1];
    uint8_t len = 0;
    len += sbuild::addChar('h', str + len, maxlen - len);
    len += sbuild::addStr("ello", str + len, maxlen - len);
    len += sbuild::addPstr(F(" world! "), str + len, maxlen - len);
    len += sbuild::addInt(123, 10, str + len, maxlen - len);
    len += sbuild::addFloat(456.789, 5, str + len, maxlen - len);
    Serial.println(str);

    // StringN
    Serial.println(String64('h') + "ello" + F(" world") + 12345 + true);
    Serial.println(String32("val: ") + String8(3.1415, 3));
    Serial.println(StringN<24>("val: ").add(3.1415, 3));

    String32 s;
    s += 'h';
    s.add("ello").add(" world ");
    s += 123;
    Serial.println(s);
}

void loop() {
}