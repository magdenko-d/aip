cmake_minimum_required(VERSION 3.10)
project(aip1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

remove_definitions(-Werror)

if(APPLE)
    # Ищем pkg-config в альтернативных путях
    find_program(PKG_CONFIG_EXECUTABLE NAMES pkg-config REQUIRED)
    
    # Устанавливаем переменную окружения для pkg-config
    set(ENV{PKG_CONFIG_PATH} "/usr/local/lib/pkgconfig:/opt/local/lib/pkgconfig")
endif()

find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(SDL2_TTF REQUIRED IMPORTED_TARGET SDL2_ttf)

find_package(SQLite3 REQUIRED)

add_executable(cipher_program
    src/ciphers.cpp
    src/database.cpp
    src/game.cpp
    src/main.cpp
)

target_include_directories(cipher_program PRIVATE
    include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)


target_link_libraries(cipher_program PRIVATE
    SQLite::SQLite3
    PkgConfig::SDL2
    PkgConfig::SDL2_TTF
)


if(APPLE)
    target_link_libraries(cipher_program PRIVATE
        "-framework Cocoa"
        "-framework Metal"
        "-framework IOKit"
        "-framework CoreVideo"
        "-framework ForceFeedback"
    )
endif()


enable_testing()

add_executable(tests
    src/ciphers.cpp
    src/database.cpp
    test/test_ciphers.cpp
)

target_include_directories(tests PRIVATE
    include
)

target_link_libraries(tests PRIVATE
    SQLite::SQLite3
)

add_test(NAME cipher_tests COMMAND tests)