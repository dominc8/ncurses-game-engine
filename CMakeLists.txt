cmake_minimum_required(VERSION 3.14)
project(NcursesGameEngine)

set(CMAKE_CXX_STANDARD 11)

add_executable(SimpleKeyboardExample main.cpp NcursesGameEngine.cpp simple-keyboard-example/SimpleKeyboardExample.cpp)
target_compile_definitions(SimpleKeyboardExample PRIVATE SIMPLE_KEYBOARD_EXAMPLE)
target_link_libraries(SimpleKeyboardExample ncurses)

add_executable(SimpleMouseExample main.cpp NcursesGameEngine.cpp simple-mouse-example/SimpleMouseExample.cpp)
target_compile_definitions(SimpleMouseExample PRIVATE SIMPLE_MOUSE_EXAMPLE)
target_link_libraries(SimpleMouseExample ncurses)

add_executable(FlappyBird main.cpp NcursesGameEngine.cpp flappy-bird/FlappyBird.cpp)
target_compile_definitions(FlappyBird PRIVATE FLAPPY_BIRD)
target_link_libraries(FlappyBird ncurses)

add_executable(MazeGenerator main.cpp NcursesGameEngine.cpp maze-generator/MazeGenerator.cpp)
target_compile_definitions(MazeGenerator PRIVATE MAZE_GENERATOR)
target_link_libraries(MazeGenerator ncurses)

add_executable(Snake main.cpp NcursesGameEngine.cpp snake/Snake.cpp)
target_compile_definitions(Snake PRIVATE SNAKE)
target_link_libraries(Snake ncurses)