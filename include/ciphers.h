/**
 * @file ciphers.h
 * @brief Заголовочный файл с реализацией шифров
 */

#ifndef CIPHERS_H
#define CIPHERS_H

#include <string>
#include <utility>
#include <vector>
/**
 * @enum CipherType
 * @brief Типы доступных шифров
 */
enum class CipherType { 
    CAESAR,  ///< Шифр Цезаря
    AFFINE,  ///< Аффинный шифр
    VIGENERE ///< Шифр Виженера
};

// Общие функции

/**
 * @brief Проверить, являются ли числа взаимно простыми
 * @param a Первое число
 * @param b Второе число
 * @return true если числа взаимно простые, иначе false
 */
bool isPrime(int a, int b);

/**
 * @brief Генерировать случайное число в диапазоне
 * @param min Минимальное значение
 * @param max Максимальное значение
 * @return Случайное число в диапазоне [min, max]
 */
int randNum(int min, int max);

// Шифр Цезаря

/**
 * @brief Зашифровать текст шифром Цезаря
 * @param text Исходный текст
 * @param key Ключ шифрования (сдвиг)
 * @return Зашифрованный текст
 */
std::string caesarEncrypt(const std::string& text, int key);

/**
 * @brief Сгенерировать случайный ключ для шифра Цезаря
 * @return Ключ шифрования (1-25)
 */
int generateCaesarKey();

/**
 * @brief Получить случайное слово для шифра Цезаря
 * @return Случайное слово из базы данных
 */
std::string getRandomCaesarWord();

// Аффинный шифр

/**
 * @brief Зашифровать текст аффинным шифром
 * @param text Исходный текст
 * @param a Первый ключ (должен быть взаимно прост с 26)
 * @param b Второй ключ
 * @return Зашифрованный текст
 * @throw std::invalid_argument Если a не взаимно прост с 26
 */
std::string affineEncrypt(const std::string& text, int a, int b);

/**
 * @brief Сгенерировать ключи для аффинного шифра
 * @return Пара ключей (a, b)
 */
std::pair<int, int> generateAffineKeys();

/**
 * @brief Получить случайное слово для аффинного шифра
 * @return Случайное слово из базы данных
 */
std::string getRandomAffineWord();

// Шифр Виженера

/**
 * @brief Зашифровать текст шифром Виженера
 * @param text Исходный текст
 * @param key Ключевое слово
 * @return Зашифрованный текст
 */
std::string vigenereEncrypt(const std::string& text, const std::string& key);

/**
 * @brief Сгенерировать случайный ключ для шифра Виженера
 * @return Ключевое слово
 */
std::string generateVigenereKey();

/**
 * @brief Расширить ключ до длины текста
 * @param key Ключевое слово
 * @param textLength Длина текста
 * @return Расширенный ключ
 */
std::string keyToLenght(const std::string& key, size_t textLength);

/**
 * @brief Получить случайное слово для шифра Виженера
 * @return Случайное слово из базы данных
 */
std::string getRandomVigenereWord();

#endif