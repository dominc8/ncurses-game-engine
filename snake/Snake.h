//
// Created by dominik on 13.07.2019.
//

#ifndef NCURSESGAMEENGINE_SNAKE_H
#define NCURSESGAMEENGINE_SNAKE_H

#include "../NcursesGameEngine.h"
#include <vector>
#include <utility>
#include <queue>
#include <random>

enum class Direction {
    Up = 0,
    Left,
    Down,
    Right
};

class Snake : public NcursesGameEngine {
public:
    Snake(int level_width, int level_height);
    ~Snake() override = default;

protected:
    bool onUserCreate() override;
    bool onUserUpdate(float elapsed_time) override;

private:
    int level_width_;
    int level_height_;
    std::vector<std::pair<int, int>> body_;
    Direction direction_;
    float speed_;
    bool lost_game_;
    float accumulated_time_;
    bool has_eaten;
    std::queue<Direction> move_queue_;
    int apple;
    std::default_random_engine random_engine_;
    std::uniform_int_distribution<int> distribution_;
};


#endif //NCURSESGAMEENGINE_SNAKE_H
