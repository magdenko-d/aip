/**
 * @file game.h
 * @brief Заголовочный файл для основного игрового класса
 */

#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "ciphers.h"

/**
 * @class Game
 * @brief Основной класс игры, управляющий логикой и отображением
 * 
 * Обрабатывает ввод пользователя, отображает интерфейс и управляет игровым процессом
 */
class Game {
public:
    /**
     * @brief Конструктор класса Game
     * 
     * Инициализирует SDL, создает окно и загружает шрифты
     */
    Game();
    
    /**
     * @brief Деструктор класса Game
     * 
     * Освобождает ресурсы SDL и закрывает соединения
     */
    ~Game();
    
    /**
     * @brief Основной игровой цикл
     */
    void run();

private:
    SDL_Window* window;    ///< Указатель на SDL окно
    SDL_Renderer* renderer;///< Указатель на SDL рендерер
    TTF_Font* font;        ///< Указатель на загруженный шрифт
    
    bool running;          ///< Флаг работы основного цикла
    CipherType currentCipher; ///< Текущий выбранный шифр
    
    // Состояние игры
    std::string encryptedWord; ///< Зашифрованное слово
    std::string decryptedWord; ///< Расшифрованное слово (ответ)
    std::string userInput;     ///< Ввод пользователя
    std::string cipherKey;     ///< Ключ шифрования
    bool showHint1;           ///< Флаг показа первой подсказки
    bool showHint2;           ///< Флаг показа второй подсказки
    bool gameWon;             ///< Флаг победы в текущем раунде

    /**
     * @brief Инициализировать SDL и создать окно
     */
    void initSDL();
    
    /**
     * @brief Освободить ресурсы SDL
     */
    void cleanup();
    
    /**
     * @brief Отрисовать текущее состояние игры
     */
    void render();
    
    /**
     * @brief Обработать события ввода
     */
    void handleEvents();
    
    /**
     * @brief Показать главное меню
     */
    void showMainMenu();
    
    /**
     * @brief Показать экран с выбранным шифром
     * @param cipherType Тип шифра (CAESAR, AFFINE или VIGENERE)
     */
    void showCipherScreen(CipherType cipherType);
    
    /**
     * @brief Отрисовать экран с шифром
     */
    void renderCipherScreen();
    
    /**
     * @brief Проверить ответ пользователя
     */
    void checkAnswer();
    
    /**
     * @brief Показать результат (победа/поражение)
     * @param correct Флаг правильного ответа
     */
    void showResult(bool correct);
};

#endif