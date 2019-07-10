//
// Created by dominik on 09.07.2019.
//

#ifndef NCURSESGAMEENGINE_MAZEGENERATOR_H
#define NCURSESGAMEENGINE_MAZEGENERATOR_H

#include "../NcursesGameEngine.h"
#include <stack>

enum Neighbour {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
};

class MazeGenerator : public NcursesGameEngine{
public:
    MazeGenerator(int maze_width, int maze_height);
    virtual ~MazeGenerator() override;

protected:
    bool onUserCreate() override;
    bool onUserUpdate(float elapsed_time) override;

private:
    int maze_width_;
    int maze_height_;
    struct Cell {
        bool is_visited_    = false;
        bool is_east_open_  = false;
        bool is_south_open_ = false;
    };
    Cell *cells;
    int current_cell_index_;
    int visited_cells_counter_;
    std::stack<int> backtrack_stack_;
};


#endif //NCURSESGAMEENGINE_MAZEGENERATOR_H
