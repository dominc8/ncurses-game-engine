cmake_minimum_required(VERSION 3.14)
project(NcursesGameEngine)

set(CMAKE_CXX_STANDARD 17)

add_executable(SimpleKeyboardExample main.cpp NcursesGameEngine.cpp simple-keyboard-example/SimpleKeyboardExample.cpp)
target_compile_definitions(SimpleKeyboardExample PRIVATE SIMPLE_KEYBOARD_EXAMPLE)
target_link_libraries(SimpleKeyboardExample ncurses)

add_executable(FlappyBird main.cpp NcursesGameEngine.cpp flappy-bird/FlappyBird.cpp)
target_compile_definitions(FlappyBird PRIVATE FLAPPY_BIRD)
target_link_libraries(FlappyBird ncurses)



