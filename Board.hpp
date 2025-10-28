#pragma once
#include <vector>
#include "Cell.hpp"

class Board {
private:
    int length;
    int width;
    std::vector<std::vector<Cell>> grid;

    void generate_obstacles();
    void generate_slow_cells();

public:
    Board(int set_length, int set_width);
    Board(const Board& other);
    Board(Board&& moved);
    Board& operator=(const Board& other);
    Board& operator=(Board&& moved);
    bool is_cell_occupied(int x, int y) const;
    int get_length() const;
    int get_width() const;
    Cell& get_cell(int x, int y);
    bool can_move_to(int x, int y) const;
    bool move_player(int from_x, int from_y, int to_x, int to_y);
    bool move_enemy(int from_x, int from_y, int to_x, int to_y);
};