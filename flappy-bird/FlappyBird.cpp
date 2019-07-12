//
// Created by dominik on 08.07.2019.
//

#include "FlappyBird.h"

FlappyBird::FlappyBird(int window_width, int window_height)
: NcursesGameEngine(window_width, window_height) {
    gap_between_obstacles_ = 30;
    gap_inside_obstacle_ = 12;
    horizontal_cycle_ = 0.02f;
    position_ = (float)(getWindowHeight()) / 2;
    velocity_ = 0.0f;
    acceleration_ = 0.0f;
    game_lost_ = false;
}


bool FlappyBird::onUserCreate() {
    std::random_device rd;
    random_engine_ = std::default_random_engine(rd());
    distribution_ = std::uniform_int_distribution<int> (-gap_between_obstacles_/3, gap_between_obstacles_/3);

    game_lost_ = false;
    for (int i = 0; i < getWindowWidth() * getWindowHeight(); ++i) {
        screen_buffer_[i].ext_color = kColorBlue;
    }
    obstacles_.emplace_back(getWindowWidth(), getWindowHeight()/2 + distribution_(random_engine_));
    for (int i = 1; i < getWindowWidth() / gap_between_obstacles_ + 2; ++i) {
        obstacles_.emplace_back(getWindowWidth() + i * gap_between_obstacles_, (obstacles_[i-1].second + distribution_(random_engine_)) % (getWindowHeight() - gap_inside_obstacle_));
    }
    return true;
}

bool FlappyBird::onUserUpdate(float elapsed_time) {
    static auto bufferPlayerPosition = [&](float pos) { return (int)pos * getWindowWidth() + getWindowWidth() / 3; };
    static float sum_time = 0.0f;

    if (game_lost_) {
        return false;
    }

    sum_time += elapsed_time;

    if (key_states_[kKeySpace] && velocity_ >= kGravity_ / 80.0f)
    {
        acceleration_ = -kGravity_/6;
        velocity_ = -kGravity_ / 3.0f;}
    else
        acceleration_ += kGravity_ * elapsed_time * 4;
    if (key_states_[kKeySpace])    key_states_[kKeySpace] = false;


    if (acceleration_ >= kGravity_)
        acceleration_ = kGravity_;

    float old_position = position_;
    velocity_ += acceleration_ * elapsed_time;
    position_ += velocity_ * elapsed_time;
    if (position_ < 1)  {
        position_ = 1;
    }
    if (position_ > (float)(getWindowHeight() - 2))  {
        position_ = (float)(getWindowHeight() - 2);
        game_lost_ = true;
    }

    int buffer_player_position = bufferPlayerPosition(old_position);


    if (screen_buffer_[buffer_player_position - getWindowWidth()].ext_color         == kColorGreen ||
            screen_buffer_[buffer_player_position - getWindowWidth() + 1].ext_color == kColorGreen ||
            screen_buffer_[buffer_player_position - 1].ext_color                    == kColorGreen ||
            screen_buffer_[buffer_player_position].ext_color                        == kColorGreen ||
            screen_buffer_[buffer_player_position + 1].ext_color                    == kColorGreen ||
            screen_buffer_[buffer_player_position + 2].ext_color                    == kColorGreen ||
            screen_buffer_[buffer_player_position + getWindowWidth() - 1].ext_color == kColorGreen ||
            screen_buffer_[buffer_player_position + getWindowWidth()].ext_color     == kColorGreen) {
        game_lost_ = true;
    }

    screen_buffer_[buffer_player_position - getWindowWidth()].ext_color     = kColorBlue;
    screen_buffer_[buffer_player_position - getWindowWidth() + 1].ext_color = kColorBlue;
    screen_buffer_[buffer_player_position - 1].ext_color                    = kColorBlue;
    screen_buffer_[buffer_player_position].ext_color                        = kColorBlue;
    screen_buffer_[buffer_player_position + 1].ext_color                    = kColorBlue;
    screen_buffer_[buffer_player_position + 2].ext_color                    = kColorBlue;
    screen_buffer_[buffer_player_position + getWindowWidth() - 1].ext_color = kColorBlue;
    screen_buffer_[buffer_player_position + getWindowWidth()].ext_color     = kColorBlue;

    buffer_player_position = bufferPlayerPosition(position_);

    if (screen_buffer_[buffer_player_position - getWindowWidth()].ext_color     == kColorGreen ||
        screen_buffer_[buffer_player_position - getWindowWidth() + 1].ext_color == kColorGreen ||
        screen_buffer_[buffer_player_position - 1].ext_color                    == kColorGreen ||
        screen_buffer_[buffer_player_position].ext_color                        == kColorGreen ||
        screen_buffer_[buffer_player_position + 1].ext_color                    == kColorGreen ||
        screen_buffer_[buffer_player_position + 2].ext_color                    == kColorGreen ||
        screen_buffer_[buffer_player_position + getWindowWidth() - 1].ext_color == kColorGreen ||
        screen_buffer_[buffer_player_position + getWindowWidth()].ext_color     == kColorGreen) {
        game_lost_ = true;
    }

    screen_buffer_[buffer_player_position - getWindowWidth()].ext_color     = kColorRed;
    screen_buffer_[buffer_player_position - getWindowWidth() + 1].ext_color = kColorBlack;
    screen_buffer_[buffer_player_position - 1].ext_color                    = kColorRed;
    screen_buffer_[buffer_player_position].ext_color                        = kColorRed;
    screen_buffer_[buffer_player_position + 1].ext_color                    = kColorRed;
    screen_buffer_[buffer_player_position + 2].ext_color                    = kColorRed;
    screen_buffer_[buffer_player_position + getWindowWidth() - 1].ext_color = kColorRed;
    screen_buffer_[buffer_player_position + getWindowWidth()].ext_color     = kColorRed;

    mvprintw(getWindowHeight() + 2, 0, "a = %f", acceleration_);
    mvprintw(getWindowHeight() + 3, 0, "v = %f", velocity_);
    mvprintw(getWindowHeight() + 4, 0, "y = %f", position_);

    if (key_states_[kKeyQ]) {
        key_states_[kKeyQ] = false;
        return false;
    }

    // draw obstacles_
    if (sum_time >= horizontal_cycle_) {
        sum_time = 0;
        if (obstacles_[0].first == 0) {
            for (int row = 0; row < getWindowHeight(); ++row) {
                screen_buffer_[row*getWindowWidth()].ext_color     = kColorBlue;
                screen_buffer_[row*getWindowWidth() + 1].ext_color = kColorBlue;
                screen_buffer_[row*getWindowWidth() + 2].ext_color = kColorBlue;
            }
            obstacles_.erase(obstacles_.begin());
            obstacles_.emplace_back(obstacles_.end()->first + gap_between_obstacles_, (obstacles_.end()->second + distribution_(random_engine_)) % (getWindowHeight() - gap_inside_obstacle_));
        }

        for (auto &obstacle : obstacles_) {
            obstacle.first -= 1;
            if (obstacle.first < getWindowWidth() + 2) {
                int row;
                for (row = 0; row < obstacle.second; ++row) {


                    if (obstacle.first < getWindowWidth() + 2 && obstacle.first > 1) {
                        screen_buffer_[row*getWindowWidth() + obstacle.first - 2].ext_color = kColorGreen;
                    }
                    if (obstacle.first < getWindowWidth() - 1) {
                        screen_buffer_[row*getWindowWidth() + obstacle.first + 1].ext_color = kColorBlue;
                    }


                }
                for (row += gap_inside_obstacle_; row < getWindowHeight(); ++row) {
                    if (obstacle.first < getWindowWidth() + 2 && obstacle.first > 1) {
                        screen_buffer_[row*getWindowWidth() + obstacle.first - 2].ext_color = kColorGreen;
                    }
                    if (obstacle.first < getWindowWidth() - 1) {
                        screen_buffer_[row*getWindowWidth() + obstacle.first + 1].ext_color = kColorBlue;
                    }
                }
            }
        }
    }

    return true;
}
