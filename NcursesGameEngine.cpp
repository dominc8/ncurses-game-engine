//
// Created by dominik on 07.07.2019.
//

#include "NcursesGameEngine.h"
#include "clocale"
#include <chrono>
#include <string>

NcursesGameEngine::NcursesGameEngine
(int window_width, int window_height) :
window_width_(window_width), window_height_(window_height), mouse_position_{0, 0, 0} {
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    nodelay(stdscr, true);
    noecho();
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    mouse_position_ = {0,0, 0};
    keypad(stdscr, true);
    refresh();

    screen_buffer_ = new cchar_t[window_height * window_width];

    for (int i = 0; i < window_height * window_width; ++i) {
        setcchar(&screen_buffer_[i], FULL_BLOCK, A_NORMAL, kColorWhite, nullptr);
    }

    start_color();
    init_pair(kColorRed    , COLOR_RED    , COLOR_BLACK);     // 1
    init_pair(kColorGreen  , COLOR_GREEN  , COLOR_BLACK);     // 2
    init_pair(kColorYellow , COLOR_YELLOW , COLOR_BLACK);     // 3
    init_pair(kColorBlue   , COLOR_BLUE   , COLOR_BLACK);     // 4
    init_pair(kColorMagenta, COLOR_MAGENTA, COLOR_BLACK);     // 5
    init_pair(kColorCyan   , COLOR_CYAN   , COLOR_BLACK);     // 6
    init_pair(kColorWhite  , COLOR_WHITE  , COLOR_BLACK);     // 7
    init_pair(kColorBlack  , COLOR_BLACK  , COLOR_BLACK);     // 8

    for (bool & key_state : key_states_) {
        key_state = false;
    }
    for (bool & mouse_state : mouse_states_) {
        mouse_state = false;
    }
}

NcursesGameEngine::~NcursesGameEngine() {
    delete[] screen_buffer_;
    endwin();
}

void NcursesGameEngine::start() {
    if (!onUserCreate()) {
        return;
    }
    for(int i = 0; i < getWindowHeight(); ++i) {
        mvadd_wchnstr(i,0, &screen_buffer_[i*getWindowWidth()], getWindowWidth());
    }
    move(getWindowHeight(), 0);
    refresh();
    std::chrono::system_clock::time_point time_point1, time_point2;
    time_point1 = std::chrono::system_clock::now();
    float elapsed_time;
    int key_input;

    while(true) {
        time_point2 = std::chrono::system_clock::now();
        elapsed_time = std::chrono::duration<float>(time_point2 - time_point1).count();
        time_point1 = time_point2;
        if ((key_input = getch()) != ERR) {
            switch (key_input) {
                case 'w':
                    key_states_[kKeyW] = true;
                    break;
                case 's':
                    key_states_[kKeyS] = true;
                    break;
                case 'a':
                    key_states_[kKeyA] = true;
                    break;
                case 'd':
                    key_states_[kKeyD] = true;
                    break;
                case ' ':
                    key_states_[kKeySpace] = true;
                    break;
                case 'q':
                    key_states_[kKeyQ] = true;
                    break;
                case KEY_UP:
                    key_states_[kKeyUp] = true;
                    break;
                case KEY_DOWN:
                    key_states_[kKeyDown] = true;
                    break;
                case KEY_LEFT:
                    key_states_[kKeyLeft] = true;
                    break;
                case KEY_RIGHT:
                    key_states_[kKeyRight] = true;
                    break;
                case KEY_MOUSE:
                    MEVENT mouse_event;
                    if(getmouse(&mouse_event) == OK) {
                        if(mouse_event.bstate & BUTTON1_PRESSED) {
                            mouse_states_[kLMBPressed] = true;
                        }
                        else if(mouse_event.bstate & BUTTON1_CLICKED) {
                            mouse_states_[kLMBClicked] = true;
                        }
                        else if(mouse_event.bstate & BUTTON1_RELEASED) {
                            mouse_states_[kLMBReleased] = true;
                        }
                        else if(mouse_event.bstate & BUTTON3_PRESSED) {
                            mouse_states_[kRMBPressed] = true;
                        }
                        else if(mouse_event.bstate & BUTTON3_CLICKED) {
                            mouse_states_[kRMBClicked] = true;
                        }
                        else if(mouse_event.bstate & BUTTON3_RELEASED) {
                            mouse_states_[kRMBReleased] = true;
                        }
                        mouse_position_ = {mouse_event.x, mouse_event.y, mouse_event.z};
                        }
                    break;
            }
        }
        if (onUserUpdate(elapsed_time)) {
            move(0,0);
            for(int i = 0; i < getWindowHeight(); ++i) {
                mvadd_wchnstr(i,0, &screen_buffer_[i*getWindowWidth()], getWindowWidth());
            }
            move(getWindowHeight(), 0);
            printw("%.0f FPS\n", 1/elapsed_time);
            refresh();
        } else {
            break;
        }
    }
}
