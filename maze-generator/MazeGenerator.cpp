//
// Created by dominik on 09.07.2019.
//

#include <vector>
#include "MazeGenerator.h"

#define CELL_SIZE 3

MazeGenerator::MazeGenerator(int maze_width, int maze_height)
: NcursesGameEngine(2 * maze_width * (CELL_SIZE + 1), maze_height * (CELL_SIZE + 1)) {
    maze_height_ = maze_height;
    maze_width_  = maze_width;
    cells = new Cell[maze_height_ * maze_width_];
    current_cell_index_ = 0;
    visited_cells_counter_ = 1;
}

MazeGenerator::~MazeGenerator() {
    delete[] cells;
}

bool MazeGenerator::onUserCreate() {
    std::random_device rd;
    random_engine_ = std::default_random_engine(rd());

    for (int i = 0; i < maze_width_ * maze_height_; ++i) {
        cells[i].is_visited_    = false;
        cells[i].is_east_open_  = false;
        cells[i].is_south_open_ = false;
    }
    cells[0].is_visited_ = true;
    current_cell_index_ = 0;
    visited_cells_counter_ = 1;

    backtrack_stack_ = {};

    int fill_color;
    for (int y = 0; y < maze_height_; ++y) {
        for (int x = 0; x < maze_width_; ++x) {
            // Fill Cell
            fill_color = cells[y*maze_width_ + x].is_visited_ ? kColorWhite : kColorBlue;
            for (int screen_y = y*(CELL_SIZE + 1); screen_y < (y+1)*(CELL_SIZE + 1) - 1; ++screen_y) {
                for (int screen_x = 2*x*(CELL_SIZE + 1); screen_x < 2*(x+1)*(CELL_SIZE + 1) - 2; ++screen_x) {
                    screen_buffer_[screen_y*getWindowWidth() + screen_x].ext_color = fill_color;
                }
            }

            // Fill Paths
            fill_color = cells[y*maze_width_ + x].is_south_open_ ? kColorWhite : kColorBlack;
            for (int screen_y = (y+1)*(CELL_SIZE + 1) - 1, screen_x = 2*x*(CELL_SIZE + 1); screen_x < 2*(x+1)*(CELL_SIZE + 1) - 2; ++screen_x) {
                screen_buffer_[screen_y*getWindowWidth() + screen_x].ext_color = fill_color;
            }
            fill_color = cells[y*maze_width_ + x].is_east_open_ ? kColorWhite : kColorBlack;
            for (int screen_x = 2*(x+1)*(CELL_SIZE + 1) - 2, screen_y = y*(CELL_SIZE + 1); screen_y < (y+1)*(CELL_SIZE + 1) - 1; ++screen_y) {
                screen_buffer_[screen_y*getWindowWidth() + screen_x].ext_color = fill_color;
                screen_buffer_[screen_y*getWindowWidth() + screen_x + 1].ext_color = fill_color;
            }

            // Fill right bottom corner
            screen_buffer_[((y+1)*(CELL_SIZE+1) - 1) * getWindowWidth() + 2*(x+1)*(CELL_SIZE+1) - 2].ext_color = kColorBlack;
            screen_buffer_[((y+1)*(CELL_SIZE+1) - 1) * getWindowWidth() + 2*(x+1)*(CELL_SIZE+1) - 1].ext_color = kColorBlack;

        }
    }
    return true;
}

bool MazeGenerator::onUserUpdate(float elapsed_time) {
    static float tick = 0.0f;
    tick += elapsed_time;
    if (tick > 0.05f) {
        tick = 0.0f;

        if (visited_cells_counter_ < maze_width_ * maze_height_) {

            std::vector<Neighbour> possible_neighbours;
            possible_neighbours.reserve(4);

            if ((current_cell_index_ % maze_width_ > 0) && !cells[current_cell_index_ - 1].is_visited_) {
                possible_neighbours.push_back(Neighbour::WEST);
            }
            if ((current_cell_index_ % maze_width_ < (maze_width_ - 1)) && !cells[current_cell_index_ + 1].is_visited_) {
                possible_neighbours.push_back(Neighbour::EAST);
            }
            if ((current_cell_index_ - maze_width_ >= 0) && !cells[current_cell_index_ - maze_width_].is_visited_) {
                possible_neighbours.push_back(Neighbour::NORTH);
            }
            if ((current_cell_index_ + maze_width_ < maze_width_*maze_height_) && !cells[current_cell_index_ + maze_width_].is_visited_) {
                possible_neighbours.push_back(Neighbour::SOUTH);
            }

            if (!possible_neighbours.empty()) {
                distribution_ = std::uniform_int_distribution<int> (0, possible_neighbours.size() - 1);
                auto neighbour = possible_neighbours[distribution_(random_engine_)];
                backtrack_stack_.push(current_cell_index_);
                visited_cells_counter_++;
                switch (neighbour) {
                    case Neighbour::NORTH:
                        cells[current_cell_index_ - maze_width_].is_visited_ = true;
                        cells[current_cell_index_ - maze_width_].is_south_open_ = true;
                        current_cell_index_ -= maze_width_;
                        break;
                    case Neighbour::EAST:
                        cells[current_cell_index_ + 1].is_visited_ = true;
                        cells[current_cell_index_].is_east_open_ = true;
                        current_cell_index_ += 1;
                        break;
                    case Neighbour::SOUTH:
                        cells[current_cell_index_ + maze_width_].is_visited_ = true;
                        cells[current_cell_index_].is_south_open_ = true;
                        current_cell_index_ += maze_width_;
                        break;
                    case Neighbour::WEST:
                        cells[current_cell_index_ - 1].is_visited_ = true;
                        cells[current_cell_index_ - 1].is_east_open_ = true;
                        current_cell_index_ -= 1;
                        break;
                }
            } else {
                current_cell_index_ = backtrack_stack_.top();
                backtrack_stack_.pop();
            }

            // Draw Maze
            int fill_color;
            for (int y = 0; y < maze_height_; ++y) {
                for (int x = 0; x < maze_width_; ++x) {
                    // Fill Cell
                    if ((y*maze_width_ + x) == current_cell_index_) {
                        fill_color = kColorGreen;
                    } else {
                        fill_color = cells[y*maze_width_ + x].is_visited_ ? kColorWhite : kColorBlue;
                    }
                    for (int screen_y = y*(CELL_SIZE + 1); screen_y < (y+1)*(CELL_SIZE + 1) - 1; ++screen_y) {
                        for (int screen_x = 2*x*(CELL_SIZE + 1); screen_x < 2*(x+1)*(CELL_SIZE + 1) - 2; ++screen_x) {
                            screen_buffer_[screen_y*getWindowWidth() + screen_x].ext_color = fill_color;
                        }
                    }

                    // Fill Paths
                    fill_color = cells[y*maze_width_ + x].is_south_open_ ? kColorWhite : kColorBlack;
                    for (int screen_y = (y+1)*(CELL_SIZE + 1) - 1, screen_x = 2*x*(CELL_SIZE + 1); screen_x < 2*(x+1)*(CELL_SIZE + 1) - 2; ++screen_x) {
                        screen_buffer_[screen_y*getWindowWidth() + screen_x].ext_color = fill_color;
                    }
                    fill_color = cells[y*maze_width_ + x].is_east_open_ ? kColorWhite : kColorBlack;
                    for (int screen_x = 2*(x+1)*(CELL_SIZE + 1) - 2, screen_y = y*(CELL_SIZE + 1); screen_y < (y+1)*(CELL_SIZE + 1) - 1; ++screen_y) {
                        screen_buffer_[screen_y*getWindowWidth() + screen_x].ext_color = fill_color;
                        screen_buffer_[screen_y*getWindowWidth() + screen_x + 1].ext_color = fill_color;
                    }
                }
            }
        }
    }

    if (key_states_[kKeyQ]) {
        key_states_[kKeyQ] = false;
        return false;
    }
    return true;
}
