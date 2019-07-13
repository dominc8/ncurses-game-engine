//
// Created by dominik on 13.07.2019.
//

#include "Snake.h"

Snake::Snake(int level_width, int level_height)
: NcursesGameEngine(level_width > 5 ? level_width * 2 + 4 : 14, level_height > 5 ? level_height + 2 : 7) {
    level_width_ = level_width;
    level_height_ = level_height;
    body_.reserve(3);
    body_.emplace_back(3, 1);
    body_.emplace_back(2, 1);
    body_.emplace_back(1, 1);
    direction_ = Direction::Right;
    speed_ = 5.0f;
    lost_game_ = false;
    accumulated_time_ = 0.0f;
    has_eaten = false;
    move_queue_ = {};
    apple = 4*level_width_ + 2*level_height_;
}

bool Snake::onUserCreate() {
    body_.clear();
    body_.reserve(3);
    body_.emplace_back(3, 1);
    body_.emplace_back(2, 1);
    body_.emplace_back(1, 1);
    direction_ = Direction::Right;
    speed_ = 5.0f;
    lost_game_ = false;
    has_eaten = false;
    accumulated_time_ = 0.0f;
    move_queue_ = {};
    apple = 4*level_width_ + 2*level_height_;
    std::random_device rd;
    random_engine_ = std::default_random_engine(rd());
    distribution_ = std::uniform_int_distribution<int> (0, level_width_*level_height_ - 1);

    for (int i = 0; i < getWindowWidth(); ++i) {
        setcchar(&screen_buffer_[i], MEDIUM_SHADE_BLOCK, A_NORMAL, kColorYellow, nullptr);
        setcchar(&screen_buffer_[getWindowWidth()*getWindowHeight() - i - 1], MEDIUM_SHADE_BLOCK, A_NORMAL, kColorYellow, nullptr);
    }
    for (int i = 1; i < getWindowHeight() - 1; ++i) {
        setcchar(&screen_buffer_[i*getWindowWidth()], MEDIUM_SHADE_BLOCK, A_NORMAL, kColorYellow, nullptr);
        setcchar(&screen_buffer_[i*getWindowWidth() + 1], MEDIUM_SHADE_BLOCK, A_NORMAL, kColorYellow, nullptr);
        setcchar(&screen_buffer_[(i+1)*getWindowWidth() - 2], MEDIUM_SHADE_BLOCK, A_NORMAL, kColorYellow, nullptr);
        setcchar(&screen_buffer_[(i+1)*getWindowWidth() - 1], MEDIUM_SHADE_BLOCK, A_NORMAL, kColorYellow, nullptr);
    }

    for (auto &pair : body_) {
        if (pair.first < 0 || pair.first >= level_width_ || pair.second < 0 || pair.second >= level_height_) {
            lost_game_ = true;
        }
        screen_buffer_[(pair.second + 1)*getWindowWidth() + 2 + 2*pair.first].ext_color = kColorBlue;
        screen_buffer_[(pair.second + 1)*getWindowWidth() + 2 + 2*pair.first + 1].ext_color = kColorBlue;
    }

    return true;
}

bool Snake::onUserUpdate(float elapsed_time) {
    if (key_states_[kKeyUp]) {
        key_states_[kKeyUp] = false;
        if (direction_ != Direction::Down) {
            move_queue_.push(Direction::Up);
        }
    }
    if (key_states_[kKeyDown]) {
        key_states_[kKeyDown] = false;
        if (direction_ != Direction::Up) {
            move_queue_.push(Direction::Down);
        }
    }
    if (key_states_[kKeyLeft]) {
        key_states_[kKeyLeft] = false;
        if (direction_ != Direction::Right) {
            move_queue_.push(Direction::Left);
        }
    }
    if (key_states_[kKeyRight]) {
        key_states_[kKeyRight] = false;
        if (direction_ != Direction::Left) {
            move_queue_.push(Direction::Right);
        }
    }
    if (key_states_[kKeyQ]) {
        key_states_[kKeyQ] = false;
        return false;
    }
    accumulated_time_ += elapsed_time * speed_;
    if (accumulated_time_ >= 0.8f) {
        has_eaten = false;
        accumulated_time_ = 0.0f;

        if (!move_queue_.empty()) {
            direction_ = move_queue_.front();
            move_queue_.pop();
        }

        switch (direction_) {
            case Direction::Up:
                body_.emplace(body_.begin(), body_[0].first, body_[0].second - 1);
                break;
            case Direction::Down:
                body_.emplace(body_.begin(), body_[0].first, body_[0].second + 1);
                break;
            case Direction::Left:
                body_.emplace(body_.begin(), body_[0].first - 1, body_[0].second);
                break;
            case Direction::Right:
                body_.emplace(body_.begin(), body_[0].first + 1, body_[0].second);
                break;
        }

        if (body_[0].second * level_width_ + body_[0].first == apple) {
            has_eaten = true;
            speed_ += 0.4f;
            bool good_location;
            do {
                good_location = true;
                apple = distribution_(random_engine_);
                for (auto pair : body_) {
                    if (pair.second*level_width_ + pair.first == apple) {
                        good_location = false;
                        break;
                    }
                }
            } while (!good_location);
        }

        if (!has_eaten) { // pole nie jest jabłkiem
            screen_buffer_[(body_[body_.size() - 1].second + 1)*getWindowWidth() + 2 + 2*body_[body_.size() - 1].first].ext_color = kColorWhite;
            screen_buffer_[(body_[body_.size() - 1].second + 1)*getWindowWidth() + 2 + 2*body_[body_.size() - 1].first + 1].ext_color = kColorWhite;
            body_.erase(body_.end());
        }

        for (unsigned long i = 1; i < body_.size(); ++i) {
            if((body_[0].first == body_[i].first) && (body_[0].second == body_[i].second)) {
                lost_game_ = true;
            }
        }

        for (auto &pair : body_) {
            if (pair.first < 0 || pair.first >= level_width_ || pair.second < 0 || pair.second >= level_height_) {
                lost_game_ = true;
            }
            screen_buffer_[(pair.second + 1)*getWindowWidth() + 2 + 2*pair.first].ext_color = kColorBlue;
            screen_buffer_[(pair.second + 1)*getWindowWidth() + 2 + 2*pair.first + 1].ext_color = kColorBlue;
        }
        screen_buffer_[(apple/level_width_ + 1)*getWindowWidth() + 2 + 2*(apple%level_width_)].ext_color = kColorRed;
        screen_buffer_[(apple/level_width_ + 1)*getWindowWidth() + 2 + 2*(apple%level_width_) + 1].ext_color = kColorRed;
        mvprintw(0, getWindowWidth(), "Score %d", body_.size() - 3);
    }
    return !lost_game_;
}
