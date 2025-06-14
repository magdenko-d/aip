#include "ciphers.h" 
#include "database.h"
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <memory>

static std::unique_ptr<Database> global_db;

void initializeDatabase() {
    if (!global_db) {
        global_db = std::make_unique<Database>("ciphers_database.db");
    }
}

bool isPrime(int a, int b) {
    while (b != 0) {
        int flag = b;
        b = a % b;
        a = flag;
    }
    return a == 1;
}

int randNum(int min, int max) {
    return min + rand() % (max - min + 1);
}

std::string caesarEncrypt(const std::string& text, int key) {
    std::string result;
    for (char c : text) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            result += static_cast<char>((c - base + key) % 26 + base);
        } else {
            result += c;
        }
    }
    return result;
}

int generateCaesarKey() {
    return randNum(1, 25);
}

std::string getRandomCaesarWord() {
    initializeDatabase();
    return global_db->getRandomWord("caesar_cipher");
}

std::string affineEncrypt(const std::string& text, int a, int b) {
    if (!isPrime(a, 26)) {
        throw std::invalid_argument("a и 26 должны быть взаимно простыми");
    }
    
    std::string result;
    for (char c : text) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            int x = c - base;
            result += static_cast<char>((a * x + b) % 26 + base);
        } else {
            result += c;
        }
    }
    return result;
}

std::pair<int, int> generateAffineKeys() {
    int a;
    do {
        a = randNum(1, 25);
    } while (!isPrime(a, 26));
    
    int b = randNum(0, 25);
    return std::make_pair(a, b);
}

std::string getRandomAffineWord() {
    initializeDatabase();
    return global_db->getRandomWord("affine_cipher");
}

std::string vigenereEncrypt(const std::string& text, const std::string& key) {
    std::string result;
    for (size_t i = 0; i < text.size(); ++i) {
        char c = text[i];
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            char keyChar = key[i % key.size()];
            int keyShift = toupper(keyChar) - 'A';
            result += static_cast<char>((c - base + keyShift) % 26 + base);
        } else {
            result += c;
        }
    }
    return result;
}

std::string generateVigenereKey() {
    const std::vector<std::string> keys = {
        "FOX", "LIFE", "OIL", "WATER", 
        "CIPHER", "WORK", "RANDOM", "PEN"
    };
    return keys[randNum(0, keys.size() - 1)];
}


std::string getRandomVigenereWord() {
    initializeDatabase();
    return global_db->getRandomWord("vigenere_cipher");
}