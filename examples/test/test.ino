#include <Arduino.h>

#include "StringN.h"

static uint16_t passed = 0;
static uint16_t failed = 0;

void pass(const __FlashStringHelper* name) {
    ++passed;
    Serial.print(F("[PASS] "));
    Serial.println(name);
}

void fail(const __FlashStringHelper* name, const char* expected, const char* actual) {
    ++failed;

    Serial.print(F("[FAIL] "));
    Serial.println(name);

    Serial.print(F("  expected: \""));
    Serial.print(expected);
    Serial.println(F("\""));

    Serial.print(F("  actual:   \""));
    Serial.print(actual);
    Serial.println(F("\""));
}

template <typename S>
void check(
    const __FlashStringHelper* name,
    const S& str,
    const char* expected) {
    if (strcmp(str.c_str(), expected) == 0) {
        pass(name);
    } else {
        fail(name, expected, str.c_str());
    }
}

void checkCStr(
    const __FlashStringHelper* name,
    const char* str,
    const char* expected) {
    if (strcmp(str, expected) == 0) {
        pass(name);
    } else {
        fail(name, expected, str);
    }
}

void checkInt(
    const __FlashStringHelper* name,
    long actual,
    long expected) {
    if (actual == expected) {
        pass(name);
    } else {
        ++failed;

        Serial.print(F("[FAIL] "));
        Serial.println(name);

        Serial.print(F("  expected: "));
        Serial.println(expected);

        Serial.print(F("  actual:   "));
        Serial.println(actual);
    }
}

// Проверяем главный сценарий:
// передача временной StringN в функцию, принимающую const char*
void foo(
    const char* str,
    const __FlashStringHelper* name,
    const char* expected) {
    checkCStr(name, str, expected);
}

void runTests() {
    Serial.println();
    Serial.println(F("=== StringN tests ==="));
    Serial.println();

    // ---------------------------------------------------------
    // 1. Temporary builder
    // ---------------------------------------------------------

    {
        String32 s = String32() + 123 + "abc";

        check(
            F("temporary: String32() + 123 + abc"),
            s,
            "123abc");
    }

    {
        foo(
            String32() + 123 + "abc",
            F("foo(String32() + 123 + abc)"),
            "123abc");
    }

    {
        foo(
            String64() + "foo=" + 123 + ", x=" + 456,
            F("long temporary chain"),
            "foo=123, x=456");
    }

    // ---------------------------------------------------------
    // 2. += должен мутировать lvalue
    // ---------------------------------------------------------

    {
        String32 s("foo");

        s += "bar";
        s += 123;

        check(
            F("operator+= mutates lvalue"),
            s,
            "foobar123");
    }

    // ---------------------------------------------------------
    // 3. StringN + literal
    // Исходный StringN не должен измениться
    // ---------------------------------------------------------

    {
        String32 s("foo");

        auto result = s + "bar";

        check(
            F("StringN + literal: source unchanged"),
            s,
            "foo");

        check(
            F("StringN + literal: result"),
            result,
            "foobar");

        checkInt(
            F("StringN + literal: capacity"),
            result.capacity(),
            35);
    }

    // ---------------------------------------------------------
    // 4. literal + StringN
    // ---------------------------------------------------------

    {
        String32 s("bar");

        auto result = "foo" + s;

        check(
            F("literal + StringN: source unchanged"),
            s,
            "bar");

        check(
            F("literal + StringN: result"),
            result,
            "foobar");

        checkInt(
            F("literal + StringN: capacity"),
            result.capacity(),
            35);
    }

    // ---------------------------------------------------------
    // 5. StringN + StringN
    // ---------------------------------------------------------

    {
        String16 a("foo");
        String32 b("bar");

        auto result = a + b;

        check(
            F("StringN + StringN: lhs unchanged"),
            a,
            "foo");

        check(
            F("StringN + StringN: rhs unchanged"),
            b,
            "bar");

        check(
            F("StringN + StringN: result"),
            result,
            "foobar");

        checkInt(
            F("StringN + StringN: capacity"),
            result.capacity(),
            48);
    }

    // ---------------------------------------------------------
    // 6. Самое важное:
    // первый + создаёт объект,
    // остальные должны работать через operator+() &&
    // ---------------------------------------------------------

    {
        String32 s("abc");

        auto result = s + "def" + 123 + "xyz";

        check(
            F("chain after lvalue: source unchanged"),
            s,
            "abc");

        check(
            F("chain after lvalue: result"),
            result,
            "abcdef123xyz");
    }

    {
        String32 s("abc");

        auto result = "pre-" + s + 123 + "-end";

        check(
            F("literal + lvalue chain: source unchanged"),
            s,
            "abc");

        check(
            F("literal + lvalue chain: result"),
            result,
            "pre-abc123-end");
    }

    {
        String16 a("foo");
        String16 b("bar");

        auto result = a + b + "!" + 123;

        check(
            F("StringN + StringN chain: lhs"),
            a,
            "foo");

        check(
            F("StringN + StringN chain: rhs"),
            b,
            "bar");

        check(
            F("StringN + StringN chain: result"),
            result,
            "foobar!123");
    }

    // ---------------------------------------------------------
    // 7. Непосредственная передача в const char*
    // ---------------------------------------------------------

    {
        String32 s("abc");

        foo(
            s + "def" + 123,
            F("foo(lvalue + literal + number)"),
            "abcdef123");

        check(
            F("foo chain does not mutate source"),
            s,
            "abc");
    }

    {
        String32 s("abc");

        foo(
            "value=" + s + ", n=" + String16("42"),
            F("mixed StringN chain"),
            "value=abc, n=42");

        check(
            F("mixed chain source unchanged"),
            s,
            "abc");
    }

    // ---------------------------------------------------------
    // 8. Числовые типы
    // ---------------------------------------------------------

    {
        foo(
            String64() + (char)'A' + ':' + (int)-123 + ',' + (unsigned int)456,
            F("integer types"),
            "A:-123,456");
    }

    // ---------------------------------------------------------
    // 9. bool
    // ---------------------------------------------------------

    {
        foo(
            String16() + true + false,
            F("bool"),
            "10");
    }

    // ---------------------------------------------------------
    // 10. Temporary + StringN
    // ---------------------------------------------------------

    {
        String16 value("abc");

        foo(
            String32() + "pre-" + value + "-post",
            F("temporary + StringN"),
            "pre-abc-post");

        check(
            F("temporary + StringN source unchanged"),
            value,
            "abc");
    }

    // ---------------------------------------------------------
    // 11. Capacity / truncation
    // ---------------------------------------------------------

    {
        StringN<5> s;

        s += "123456789";

        check(
            F("truncate"),
            s,
            "12345");

        checkInt(
            F("truncate length"),
            s.length(),
            5);

        checkInt(
            F("truncate isFull"),
            s.isFull(),
            1);
    }

    {
        auto s = "12345" + StringN<3>("abc");

        check(
            F("calculated capacity content"),
            s,
            "12345abc");

        checkInt(
            F("calculated capacity"),
            s.capacity(),
            8);
    }

    // ---------------------------------------------------------
    // Summary
    // ---------------------------------------------------------

    Serial.println();
    Serial.println(F("=== RESULT ==="));

    Serial.print(F("Passed: "));
    Serial.println(passed);

    Serial.print(F("Failed: "));
    Serial.println(failed);

    if (failed == 0) {
        Serial.println(F("ALL TESTS PASSED"));
    } else {
        Serial.println(F("TESTS FAILED"));
    }
}

void setup() {
    Serial.begin(115200);
    delay(500);
    runTests();
}

void loop() {
}