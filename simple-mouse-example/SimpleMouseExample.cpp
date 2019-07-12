//
// Created by dominik on 12.07.2019.
//

#include "SimpleMouseExample.h"

SimpleMouseExample::SimpleMouseExample(int window_width, int window_height)
: NcursesGameEngine(window_width, window_height) {
    for(int i = 0; i < window_height * window_width; ++i) {
        switch (i%4) {
            case 0:
                setcchar(&screen_buffer_[i], FULL_BLOCK, A_NORMAL, kColorWhite, nullptr);
                break;
            case 1:
                setcchar(&screen_buffer_[i], DARK_SHADE_BLOCK, A_NORMAL, kColorWhite, nullptr);
                break;
            case 2:
                setcchar(&screen_buffer_[i], MEDIUM_SHADE_BLOCK, A_NORMAL, kColorWhite, nullptr);
                break;
            case 3:
                setcchar(&screen_buffer_[i], LIGHT_SHADE_BLOCK, A_NORMAL, kColorWhite, nullptr);
                break;
        }
    }
}

bool SimpleMouseExample::onUserCreate() {
    random_engine_ = std::default_random_engine(random_device_());
    distribution_ = std::uniform_int_distribution<int>(kColorRed, kColorCyan);
    return true;
}

bool SimpleMouseExample::onUserUpdate(float elapsed_time) {
    static int old_index;
    static int old_color = screen_buffer_[old_index].ext_color;
    bool changed_pos = false;

    if (key_states_[kKeyQ]) {
        key_states_[kKeyQ] = false;
        return false;
    }
    if (mouse_states_[kLMBClicked]) {
        mouse_states_[kLMBClicked] = false;
        changed_pos = true;
    }
    if (mouse_states_[kLMBReleased]) {
        mouse_states_[kLMBReleased] = false;
        if (mouse_states_[kLMBPressed]) {
            mouse_states_[kLMBPressed] = false;
            changed_pos = true;
        }
    }

    if (changed_pos && (mouse_position_.y*getWindowWidth() + mouse_position_.x != old_index)) {
        screen_buffer_[old_index].ext_color = old_color;
        old_index = mouse_position_.y*getWindowWidth() + mouse_position_.x;
        old_color = screen_buffer_[old_index].ext_color;
        screen_buffer_[old_index].ext_color = distribution_(random_engine_);
    }

    return true;
}
