//
// Created by dominik on 08.07.2019.
//

#include "SimpleKeyboardExample.h"

SimpleKeyboardExample::SimpleKeyboardExample(int window_width, int window_height)
        : NcursesGameEngine(window_width, window_height) {}

bool SimpleKeyboardExample::onUserCreate() {

    int buffer_index = 0;
    for (buffer_index = 0; buffer_index < 1*getWindowWidth(); ++buffer_index) {
        screen_buffer_[buffer_index].ext_color = kColorBlue;
    }

    for (; buffer_index < 2*getWindowWidth(); ++buffer_index) {
        screen_buffer_[buffer_index].ext_color = kColorGreen;
    }

    for (; buffer_index < getWindowWidth() * getWindowHeight(); ++buffer_index) {
        screen_buffer_[buffer_index].ext_color = kColorWhite;
    }

    return true;
}

bool SimpleKeyboardExample::onUserUpdate(float elapsed_time) {
    static int x, y;
    static int old_index;
    static int old_color = screen_buffer_[old_index].ext_color;
    if (key_states_[kKeyW]) {
        if (y > 0) --y;
        key_states_[kKeyW] = false;
    }
    if (key_states_[kKeyS]) {
        if (y < getWindowHeight() - 1) ++y;
        key_states_[kKeyS] = false;
    }
    if (key_states_[kKeyA]) {
        if (x > 0) --x;
        key_states_[kKeyA] = false;
    }
    if (key_states_[kKeyD]) {
        if (x < getWindowWidth() - 1) ++x;
        key_states_[kKeyD] = false;
    }
    if (key_states_[kKeyQ]) {
        key_states_[kKeyQ] = false;
        return false;
    }
    if (y*getWindowWidth() + x != old_index) {
        screen_buffer_[old_index].ext_color = old_color;
        old_index = y*getWindowWidth() + x;
        old_color = screen_buffer_[old_index].ext_color;
        screen_buffer_[y*getWindowWidth() + x].ext_color = kColorRed;
    }
    return true;
}
