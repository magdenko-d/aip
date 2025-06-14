#include "game.h"
#include <iostream>


Game::Game() : window(nullptr), renderer(nullptr), font(nullptr), running(true),
               showHint1(false), showHint2(false), gameWon(false) {
    initSDL();
}

Game::~Game() {
    cleanup();
}

void Game::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    if (TTF_Init() == -1) {
        std::cerr << "TTF could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        exit(1);
    }

    window = SDL_CreateWindow("Cipher Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    font = TTF_OpenFont("NotoSans-Regular.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        exit(1);
    }
}

void Game::cleanup() {
    if (font) TTF_CloseFont(font);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void Game::run() {
    while (running) {
        handleEvents();
        render();
        SDL_Delay(16);
    }
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
        }
        else if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
            else if (e.key.keysym.sym == SDLK_RETURN) {
                checkAnswer();
            }
            else if (e.key.keysym.sym == SDLK_BACKSPACE && !gameWon) {
                if (!userInput.empty()) {
                    userInput.pop_back();
                }
            }
        }
        else if (e.type == SDL_TEXTINPUT && !gameWon) {
            userInput += e.text.text;
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);

            if (gameWon) {

                if (x >= 200 && x <= 400 && y >= 400 && y <= 460) {
                    showCipherScreen(currentCipher);
                }

                else if (x >= 400 && x <= 600 && y >= 400 && y <= 460) {
                    encryptedWord.clear();
                    decryptedWord.clear();
                    cipherKey.clear();
                    userInput.clear();
                    showHint1 = false;
                    showHint2 = false;
                    gameWon = false;
                }
            }
            else if (!encryptedWord.empty()) {

                if (x >= 100 && x <= 300 && y >= 450 && y <= 510) {
                    encryptedWord.clear();
                    decryptedWord.clear();
                    cipherKey.clear();
                    userInput.clear();
                    showHint1 = false;
                    showHint2 = false;
                }

                else if (x >= 300 && x <= 500 && y >= 450 && y <= 510) {
                    showHint1 = true;
                }

                else if (x >= 500 && x <= 700 && y >= 450 && y <= 510) {
                    showHint2 = true;
                }
            }
            else if (encryptedWord.empty()) {
                if (x >= 250 && x <= 550) {
                    if (y >= 220 && y <= 280) {
                        showCipherScreen(CipherType::CAESAR);
                    }
                    else if (y >= 300 && y <= 360) {
                        showCipherScreen(CipherType::AFFINE);
                    }
                    else if (y >= 380 && y <= 440) {
                        showCipherScreen(CipherType::VIGENERE);
                    }
                }
            }
        }
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    SDL_RenderClear(renderer);

    if (gameWon) {
        showResult(true);
    }
    else if (encryptedWord.empty()) {
        showMainMenu();
    }
    else {
        renderCipherScreen();
    }

    SDL_RenderPresent(renderer);
}

void Game::renderCipherScreen() {
    SDL_Color black = {0, 0, 0, 255};
    std::string cipherName;
    switch (currentCipher) {
        case CipherType::CAESAR: cipherName = "Caesar Cipher"; break;
        case CipherType::AFFINE: cipherName = "Affine Cipher"; break;
        case CipherType::VIGENERE: cipherName = "Vigenere Cipher"; break;
    }
    
    SDL_Surface* cipherSurface = TTF_RenderText_Solid(font, cipherName.c_str(), black);
    SDL_Texture* cipherTexture = SDL_CreateTextureFromSurface(renderer, cipherSurface);
    SDL_Rect cipherRect = {400 - cipherSurface->w/2, 50, cipherSurface->w, cipherSurface->h};
    SDL_RenderCopy(renderer, cipherTexture, nullptr, &cipherRect);
    SDL_FreeSurface(cipherSurface);
    SDL_DestroyTexture(cipherTexture);

    SDL_Surface* encryptedSurface = TTF_RenderText_Solid(font, encryptedWord.c_str(), black);
    SDL_Texture* encryptedTexture = SDL_CreateTextureFromSurface(renderer, encryptedSurface);
    SDL_Rect encryptedRect = {400 - encryptedSurface->w/2, 150, encryptedSurface->w, encryptedSurface->h};
    SDL_RenderCopy(renderer, encryptedTexture, nullptr, &encryptedRect);
    SDL_FreeSurface(encryptedSurface);
    SDL_DestroyTexture(encryptedTexture);

    SDL_Rect inputRect = {50, 250, 700, 40};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &inputRect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &inputRect);
    
    std::string inputPrompt = "Decrypt: " + userInput;
    SDL_Surface* inputSurface = TTF_RenderText_Solid(font, inputPrompt.c_str(), black);
    SDL_Texture* inputTexture = SDL_CreateTextureFromSurface(renderer, inputSurface);
    SDL_Rect inputTextRect = {inputRect.x + 10, inputRect.y + (inputRect.h - inputSurface->h)/2, 
                             inputSurface->w, inputSurface->h};
    SDL_RenderCopy(renderer, inputTexture, nullptr, &inputTextRect);
    SDL_FreeSurface(inputSurface);
    SDL_DestroyTexture(inputTexture);

    if (showHint1) {
        std::string hintText = "Hint: First letter is '" + std::string(1, decryptedWord[0]) + "'";
        SDL_Surface* hint1Surface = TTF_RenderText_Solid(font, hintText.c_str(), black);
        SDL_Texture* hint1Texture = SDL_CreateTextureFromSurface(renderer, hint1Surface);
        SDL_Rect hint1Rect = {400 - hint1Surface->w/2, 350, hint1Surface->w, hint1Surface->h};
        SDL_RenderCopy(renderer, hint1Texture, nullptr, &hint1Rect); 
        SDL_FreeSurface(hint1Surface);
        SDL_DestroyTexture(hint1Texture);
    }

    if (showHint2) {
        std::string hintText = "Key: " + cipherKey;
        SDL_Surface* hint2Surface = TTF_RenderText_Solid(font, hintText.c_str(), black);
        SDL_Texture* hint2Texture = SDL_CreateTextureFromSurface(renderer, hint2Surface);
        SDL_Rect hint2Rect = {400 - hint2Surface->w/2, 400, hint2Surface->w, hint2Surface->h};
        SDL_RenderCopy(renderer, hint2Texture, nullptr, &hint2Rect);
        SDL_FreeSurface(hint2Surface);
        SDL_DestroyTexture(hint2Texture);
    }


    SDL_Color buttonColor = {220, 220, 220, 255};
    

    SDL_Rect menuRect = {100, 450, 200, 60};
    SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    SDL_RenderFillRect(renderer, &menuRect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &menuRect);
    
    SDL_Surface* menuSurface = TTF_RenderText_Solid(font, "Menu", black);
    SDL_Texture* menuTexture = SDL_CreateTextureFromSurface(renderer, menuSurface);
    SDL_Rect menuTextRect = {menuRect.x + (menuRect.w - menuSurface->w)/2, 
                            menuRect.y + (menuRect.h - menuSurface->h)/2, 
                            menuSurface->w, menuSurface->h};
    SDL_RenderCopy(renderer, menuTexture, nullptr, &menuTextRect);
    SDL_FreeSurface(menuSurface);
    SDL_DestroyTexture(menuTexture);

    SDL_Rect hint1Rect = {300, 450, 200, 60};
    SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    SDL_RenderFillRect(renderer, &hint1Rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &hint1Rect);
    
    SDL_Surface* hint1BtnSurface = TTF_RenderText_Solid(font, "Hint 1", black);
    SDL_Texture* hint1BtnTexture = SDL_CreateTextureFromSurface(renderer, hint1BtnSurface);
    SDL_Rect hint1BtnTextRect = {hint1Rect.x + (hint1Rect.w - hint1BtnSurface->w)/2, 
                                hint1Rect.y + (hint1Rect.h - hint1BtnSurface->h)/2, 
                                hint1BtnSurface->w, hint1BtnSurface->h};
    SDL_RenderCopy(renderer, hint1BtnTexture, nullptr, &hint1BtnTextRect);
    SDL_FreeSurface(hint1BtnSurface);
    SDL_DestroyTexture(hint1BtnTexture);


    SDL_Rect hint2Rect = {500, 450, 200, 60};
    SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    SDL_RenderFillRect(renderer, &hint2Rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &hint2Rect);
    
    SDL_Surface* hint2BtnSurface = TTF_RenderText_Solid(font, "Hint 2", black);
    SDL_Texture* hint2BtnTexture = SDL_CreateTextureFromSurface(renderer, hint2BtnSurface);
    SDL_Rect hint2BtnTextRect = {hint2Rect.x + (hint2Rect.w - hint2BtnSurface->w)/2, 
                                hint2Rect.y + (hint2Rect.h - hint2BtnSurface->h)/2, 
                                hint2BtnSurface->w, hint2BtnSurface->h};
    SDL_RenderCopy(renderer, hint2BtnTexture, nullptr, &hint2BtnTextRect);
    SDL_FreeSurface(hint2BtnSurface);
    SDL_DestroyTexture(hint2BtnTexture);
}

void Game::showMainMenu() {
    SDL_Color black = {0, 0, 0, 255};
    
    TTF_Font* titleFont = TTF_OpenFont("NotoSans-Regular.ttf", 36);
    SDL_Surface* titleSurface = TTF_RenderText_Solid(titleFont, "Cipher Challenge", black);
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_Rect titleRect = {400 - titleSurface->w/2, 80, titleSurface->w, titleSurface->h};
    SDL_RenderCopy(renderer, titleTexture, nullptr, &titleRect);
    SDL_FreeSurface(titleSurface);
    SDL_DestroyTexture(titleTexture);
    TTF_CloseFont(titleFont);

    SDL_Surface* subtitleSurface = TTF_RenderText_Solid(font, "Select Cipher Type:", black);
    SDL_Texture* subtitleTexture = SDL_CreateTextureFromSurface(renderer, subtitleSurface);
    SDL_Rect subtitleRect = {400 - subtitleSurface->w/2, 150, subtitleSurface->w, subtitleSurface->h};
    SDL_RenderCopy(renderer, subtitleTexture, nullptr, &subtitleRect);
    SDL_FreeSurface(subtitleSurface);
    SDL_DestroyTexture(subtitleTexture);

    SDL_Color buttonColor = {220, 220, 220, 255};
    
    SDL_Rect caesarRect = {250, 220, 300, 60};
    SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    SDL_RenderFillRect(renderer, &caesarRect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &caesarRect);
    
    SDL_Surface* caesarSurface = TTF_RenderText_Solid(font, "Caesar Cipher", black);
    SDL_Texture* caesarTexture = SDL_CreateTextureFromSurface(renderer, caesarSurface);
    SDL_Rect caesarTextRect = {caesarRect.x + (caesarRect.w - caesarSurface->w)/2, 
                              caesarRect.y + (caesarRect.h - caesarSurface->h)/2, 
                              caesarSurface->w, caesarSurface->h};
    SDL_RenderCopy(renderer, caesarTexture, nullptr, &caesarTextRect);
    SDL_FreeSurface(caesarSurface);
    SDL_DestroyTexture(caesarTexture);

    SDL_Rect affineRect = {250, 300, 300, 60};
    SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    SDL_RenderFillRect(renderer, &affineRect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &affineRect);
    
    SDL_Surface* affineSurface = TTF_RenderText_Solid(font, "Affine Cipher", black);
    SDL_Texture* affineTexture = SDL_CreateTextureFromSurface(renderer, affineSurface);
    SDL_Rect affineTextRect = {affineRect.x + (affineRect.w - affineSurface->w)/2, 
                              affineRect.y + (affineRect.h - affineSurface->h)/2, 
                              affineSurface->w, affineSurface->h};
    SDL_RenderCopy(renderer, affineTexture, nullptr, &affineTextRect);
    SDL_FreeSurface(affineSurface);
    SDL_DestroyTexture(affineTexture);

    SDL_Rect vigenereRect = {250, 380, 300, 60};
    SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    SDL_RenderFillRect(renderer, &vigenereRect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &vigenereRect);
    
    SDL_Surface* vigenereSurface = TTF_RenderText_Solid(font, "Vigenere Cipher", black);
    SDL_Texture* vigenereTexture = SDL_CreateTextureFromSurface(renderer, vigenereSurface);
    SDL_Rect vigenereTextRect = {vigenereRect.x + (vigenereRect.w - vigenereSurface->w)/2, 
                                vigenereRect.y + (vigenereRect.h - vigenereSurface->h)/2, 
                                vigenereSurface->w, vigenereSurface->h};
    SDL_RenderCopy(renderer, vigenereTexture, nullptr, &vigenereTextRect);
    SDL_FreeSurface(vigenereSurface);
    SDL_DestroyTexture(vigenereTexture);
}

void Game::showCipherScreen(CipherType cipherType) {
    currentCipher = cipherType;
    
    decryptedWord = [&]() {
        switch (cipherType) {
            case CipherType::CAESAR: return getRandomCaesarWord();
            case CipherType::AFFINE: return getRandomAffineWord();
            case CipherType::VIGENERE: return getRandomVigenereWord();
            default: return std::string("");
        }
    }();
    
    switch (cipherType) {
        case CipherType::CAESAR: {
            int key = generateCaesarKey();
            cipherKey = std::to_string(key);
            encryptedWord = caesarEncrypt(decryptedWord, key);
            break;
        }
        case CipherType::AFFINE: {
            auto keys = generateAffineKeys();
            cipherKey = std::to_string(keys.first) + ", " + std::to_string(keys.second);
            encryptedWord = affineEncrypt(decryptedWord, keys.first, keys.second);
            break;
        }
        case CipherType::VIGENERE: {
            cipherKey = generateVigenereKey();
            encryptedWord = vigenereEncrypt(decryptedWord, cipherKey);
            break;
        }
    }
    
    userInput.clear();
    showHint1 = false;
    showHint2 = false;
    gameWon = false;
}

void Game::showResult(bool correct) {
    SDL_Color black = {0, 0, 0, 255};
    SDL_Color color = correct ? SDL_Color{0, 150, 0, 255} : SDL_Color{150, 0, 0, 255};
    std::string message = correct ? "Correct! Level passed!" : "Incorrect!";
    
    SDL_Surface* resultSurface = TTF_RenderText_Solid(font, message.c_str(), color);
    SDL_Texture* resultTexture = SDL_CreateTextureFromSurface(renderer, resultSurface);
    SDL_Rect resultRect = {400 - resultSurface->w/2, 200, resultSurface->w, resultSurface->h};
    SDL_RenderCopy(renderer, resultTexture, nullptr, &resultRect);
    SDL_FreeSurface(resultSurface);
    SDL_DestroyTexture(resultTexture);

    if (correct) {
        std::string decryptedText = "Decrypted word: " + decryptedWord;
        SDL_Surface* decryptedSurface = TTF_RenderText_Solid(font, decryptedText.c_str(), black);
        SDL_Texture* decryptedTexture = SDL_CreateTextureFromSurface(renderer, decryptedSurface);
        SDL_Rect decryptedRect = {400 - decryptedSurface->w/2, 250, decryptedSurface->w, decryptedSurface->h};
        SDL_RenderCopy(renderer, decryptedTexture, nullptr, &decryptedRect);
        SDL_FreeSurface(decryptedSurface);
        SDL_DestroyTexture(decryptedTexture);

        std::string keyText = "Key: " + cipherKey;
        SDL_Surface* keySurface = TTF_RenderText_Solid(font, keyText.c_str(), black);
        SDL_Texture* keyTexture = SDL_CreateTextureFromSurface(renderer, keySurface);
        SDL_Rect keyRect = {400 - keySurface->w/2, 300, keySurface->w, keySurface->h};
        SDL_RenderCopy(renderer, keyTexture, nullptr, &keyRect);
        SDL_FreeSurface(keySurface);
        SDL_DestroyTexture(keyTexture);

        SDL_Color buttonColor = {220, 220, 220, 255};
        
        SDL_Rect sameRect = {200, 400, 200, 60};
        SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
        SDL_RenderFillRect(renderer, &sameRect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &sameRect);
        
        SDL_Surface* sameSurface = TTF_RenderText_Solid(font, "Same Cipher", black);
        SDL_Texture* sameTexture = SDL_CreateTextureFromSurface(renderer, sameSurface);
        SDL_Rect sameTextRect = {sameRect.x + (sameRect.w - sameSurface->w)/2, 
                                sameRect.y + (sameRect.h - sameSurface->h)/2, 
                                sameSurface->w, sameSurface->h};
        SDL_RenderCopy(renderer, sameTexture, nullptr, &sameTextRect);
        SDL_FreeSurface(sameSurface);
        SDL_DestroyTexture(sameTexture);

        SDL_Rect newRect = {400, 400, 200, 60};
        SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
        SDL_RenderFillRect(renderer, &newRect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &newRect);
        
        SDL_Surface* newSurface = TTF_RenderText_Solid(font, "New Cipher", black);
        SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, newSurface);
        SDL_Rect newTextRect = {newRect.x + (newRect.w - newSurface->w)/2, 
                                newRect.y + (newRect.h - newSurface->h)/2, 
                                newSurface->w, newSurface->h};
        SDL_RenderCopy(renderer, newTexture, nullptr, &newTextRect);
        SDL_FreeSurface(newSurface);
        SDL_DestroyTexture(newTexture);
    }
}

void Game::checkAnswer() {
    if (userInput == decryptedWord) {
        gameWon = true;
    }
}