//
// Created by dominik on 12.07.2019.
//

#ifndef NCURSESGAMEENGINE_SIMPLEMOUSEEXAMPLE_H
#define NCURSESGAMEENGINE_SIMPLEMOUSEEXAMPLE_H

#include "../NcursesGameEngine.h"
#include <random>

class SimpleMouseExample : public NcursesGameEngine {
public:
    SimpleMouseExample(int window_width, int window_height);
    ~SimpleMouseExample() override = default;
protected:
    bool onUserCreate() override;

    bool onUserUpdate(float elapsed_time) override;

private:
    std::default_random_engine random_engine_;
    std::random_device random_device_;
    std::uniform_int_distribution<int> distribution_;

};


#endif //NCURSESGAMEENGINE_SIMPLEMOUSEEXAMPLE_H
