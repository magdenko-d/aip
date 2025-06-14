/**
 * @file database.h
 * @brief Заголовочный файл для работы с базой данных SQLite
 */

#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <sqlite3.h>

/**
 * @class Database
 * @brief Класс для взаимодействия с базой данных SQLite
 * 
 * Обеспечивает подключение к базе данных, создание таблиц и получение случайных слов
 */
class Database {
public:
    /**
     * @brief Конструктор класса Database
     * @param db_path Путь к файлу базы данных
     * @throw std::runtime_error Если не удалось открыть базу данных
     */
    Database(const std::string& db_path);
    
    /**
     * @brief Деструктор класса Database
     */
    ~Database();
    
    /**
     * @brief Получить случайное слово из указанной таблицы
     * @param table_name Имя таблицы ("caesar_cipher", "affine_cipher" или "vigenere_cipher")
     * @return Случайное слово из таблицы
     * @throw std::runtime_error Если таблица пуста или не существует
     */
    std::string getRandomWord(const std::string& table_name);
    
private:
    sqlite3* db; ///< Указатель на соединение с базой данных SQLite
    
    /**
     * @brief Проверить код ошибки SQLite
     * @param rc Код возврата SQLite
     * @param error_msg Сообщение об ошибке
     * @throw std::runtime_error Если код ошибки не SQLITE_OK
     */
    void checkError(int rc, const char* error_msg);
    
    /**
     * @brief Заполнить базу данных тестовыми значениями, если таблицы пусты
     */
    void fillTestDataIfEmpty();
};

#endif