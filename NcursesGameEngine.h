//
// Created by dominik on 07.07.2019.
//

#ifndef NCURSESGAMEENGINE_NCURSESGAMEENGINE_H
#define NCURSESGAMEENGINE_NCURSESGAMEENGINE_H

#include <ncurses.h>

#define FULL_BLOCK L"\u2588"
#define DARK_SHADE_BLOCK L"\u2593"
#define MEDIUM_SHADE_BLOCK L"\u2592"
#define LIGHT_SHADE_BLOCK L"\u2591"

enum KeyStates {
    kKeyW  = 0,
    kKeyS,
    kKeyA,
    kKeyD,
    kKeySpace,
    kKeyQ,
    kKeyUp,
    kKeyDown,
    kKeyLeft,
    kKeyRight,
    kKeyStatesCount
};

enum MouseStates {
    kLMBPressed = 0,
    kLMBClicked,
    kLMBReleased,
    kRMBPressed,
    kRMBClicked,
    kRMBReleased,
    kMouseStatesCount
};

enum Colors {
    kColorRed = 1,
    kColorGreen,
    kColorYellow,
    kColorBlue,
    kColorMagenta,
    kColorCyan,
    kColorWhite,
    kColorBlack
};

class NcursesGameEngine {
public:
    NcursesGameEngine(int window_width, int window_height);
    virtual ~NcursesGameEngine();
    void start();

protected:
    virtual bool onUserCreate() = 0;
    virtual bool onUserUpdate(float elapsed_time) = 0;

    inline int getWindowWidth() const { return window_width_; };
    inline int getWindowHeight() const { return window_height_; };

    cchar_t *screen_buffer_;
    bool key_states_[kKeyStatesCount];
    bool mouse_states_[kMouseStatesCount];
    struct MousePosition{
        int x, y, z;
    } mouse_position_;


private:
    int window_width_;
    int window_height_;
};


#endif //NCURSESGAMEENGINE_NCURSESGAMEENGINE_H
