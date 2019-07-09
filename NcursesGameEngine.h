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
    kKeyStatesCount
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

    inline const int getWindowWidth() const { return window_width_; };
    inline const int getWindowHeight() const { return window_height_; };

    cchar_t *screen_buffer_;
    bool key_states_[kKeyStatesCount];

private:
    int window_width_;
    int window_height_;
};


#endif //NCURSESGAMEENGINE_NCURSESGAMEENGINE_H
