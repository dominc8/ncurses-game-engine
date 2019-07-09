//
// Created by dominik on 08.07.2019.
//

#ifndef NCURSESGAMEENGINE_SIMPLEKEYBOARDEXAMPLE_H
#define NCURSESGAMEENGINE_SIMPLEKEYBOARDEXAMPLE_H

#include "../NcursesGameEngine.h"

class SimpleKeyboardExample : public NcursesGameEngine {
public:
    SimpleKeyboardExample(int window_width, int window_height);
    ~SimpleKeyboardExample() override {};

    bool onUserCreate() override;
    bool onUserUpdate(float elapsed_time) override;
};


#endif //NCURSESGAMEENGINE_SIMPLEKEYBOARDEXAMPLE_H
