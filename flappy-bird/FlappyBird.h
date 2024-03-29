//
// Created by dominik on 08.07.2019.
//

#ifndef NCURSESGAMEENGINE_FLAPPYBIRD_H
#define NCURSESGAMEENGINE_FLAPPYBIRD_H

#include <utility>
#include <vector>
#include <random>
#include "../NcursesGameEngine.h"

class FlappyBird : public NcursesGameEngine{
public:
    FlappyBird(int window_width, int window_height);
    ~FlappyBird() override = default;

    bool onUserCreate() override;
    bool onUserUpdate(float elapsed_time) override;

private:
    float position_;
    float velocity_;
    float acceleration_;
    const float kGravity_ = 50.0f;
    std::vector<std::pair<int, int>> obstacles_;
    int gap_between_obstacles_;
    int gap_inside_obstacle_;
    float horizontal_cycle_;
    bool game_lost_;
    std::default_random_engine random_engine_;
    std::uniform_int_distribution<int> distribution_;
};


#endif //NCURSESGAMEENGINE_FLAPPYBIRD_H
