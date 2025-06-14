#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include "../include/ciphers.h"

TEST_CASE("Test Caesar ") {
    SUBCASE("Encryption") {
        CHECK(caesarEncrypt("abc", 1) == "bcd");
        CHECK(caesarEncrypt("xyz", 3) == "abc");
        CHECK(caesarEncrypt("ABC", 1) == "BCD");
    }
    
    SUBCASE("Edge") {
        CHECK(caesarEncrypt("", 5) == "");
        CHECK(caesarEncrypt("a!b@c#", 1) == "b!c@d#");
        CHECK(caesarEncrypt("123", 5) == "123");
    }
    
    SUBCASE("Key generate") {
        int key = generateCaesarKey();
        CHECK(key >= 1);
        CHECK(key <= 25);
    }
}

TEST_CASE("Test Affine") {
    SUBCASE("Encryption") {
        CHECK(affineEncrypt("abc", 1, 1) == "bcd");
    
    }
    
    SUBCASE("Invalid") {
        CHECK_THROWS(affineEncrypt("test", 2, 0));
    }
    
    SUBCASE("Key generate") {
        auto keys = generateAffineKeys();
        CHECK(isPrime(keys.first, 26));
        CHECK(keys.second >= 0);
        CHECK(keys.second <= 25);
    }
}

TEST_CASE("Test Vigenere") {
    SUBCASE("Encryption") {
        CHECK(vigenereEncrypt("attackatdawn", "lemon") == "lxfopvefrnhr");
        CHECK(vigenereEncrypt("hello", "key") == "rijvs");
    }
    
}

TEST_CASE("Test common") {
    CHECK(isPrime(1, 26) == true);
    CHECK(isPrime(3, 26) == true);
    CHECK(isPrime(2, 26) == false);
    
    int num = randNum(5, 10);
    CHECK(num >= 5);
    CHECK(num <= 10);
}