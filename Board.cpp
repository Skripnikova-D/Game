#include "Board.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

void Board::generate_obstacles() {
    int cells = length * width;
    int obstacle_count = cells * 0.1;
    for (int i = 0; i < obstacle_count; i++) {
        int x = std::rand() % length;
        int y = std::rand() % width;
        if (x != 0 || y != 0) {
            grid[y][x].set_obstacle(true);
        }
    }
}

void Board::generate_slow_cells() {
    int cells = length * width;
    int slow_count = cells * 0.05;
    for (int i = 0; i < slow_count; i++) {
        int x = std::rand() % length;
        int y = std::rand() % width;

        if ((x != 0 || y != 0) && !grid[y][x].is_obstacle_here()) {
            grid[y][x].set_slow(true);
        }
    }
}

Board::Board(int set_length, int set_width) {
    if (set_length < 10 || set_length > 25 ||
        set_width < 10 || set_width > 25) {
        set_length = 10;
        set_width = 10;
    }

    length = set_length;
    width = set_width;

    grid.resize(width);
    for (int i = 0; i < width; i++) {
        grid[i].resize(length);
    }
    std::srand(std::time(0));
    generate_obstacles();
    generate_slow_cells();
}

Board::Board(const Board& other) {
    length = other.length;
    width = other.width;
    grid = other.grid;
}

Board::Board(Board&& moved) {
    length = moved.length;
    width = moved.width;
    grid = std::move(moved.grid);

    moved.length = 0;
    moved.width = 0;
}

Board& Board::operator=(const Board& other) {
    if (this != &other) {
        length = other.length;
        width = other.width;
        grid = other.grid;
    }
    return *this;
}

Board& Board::operator=(Board&& moved) {
    if (&moved != this) {
        length = moved.length;
        width = moved.width;
        grid = std::move(moved.grid);

        moved.length = 0;
        moved.width = 0;
    }
    return *this;
}

bool Board::is_cell_occupied(int x, int y) const {
    if (x < 0 || x >= length || y < 0 || y >= width) {
        return true;
    }
    const Cell& cell = grid[y][x];
    return cell.is_player_here() || cell.is_enemy_here() || cell.is_obstacle_here();
}

int Board::get_length() const {
    return length;
}

int Board::get_width() const {
    return width;
}

Cell& Board::get_cell(int x, int y) {
    if (x < 0 || x >= length || y < 0 || y >= width) {
        throw std::out_of_range("Coordinates out of board");
    }
    return grid[y][x];
}

bool Board::can_move_to(int x, int y) const {
    if (x < 0 || x >= length || y < 0 || y >= width) {
        return false;
    }
    const Cell& cell = grid[y][x];
    return !cell.is_obstacle_here();
}

bool Board::move_player(int from_x, int from_y, int to_x, int to_y) {
    if (!can_move_to(to_x, to_y)) {
        return false;
    }
    get_cell(from_x, from_y).set_player(false);
    get_cell(to_x, to_y).set_player(true);
    return true;
}

bool Board::move_enemy(int from_x, int from_y, int to_x, int to_y) {
    if (!can_move_to(to_x, to_y)) {
        return false;
    }
    get_cell(from_x, from_y).set_enemy(false);
    get_cell(to_x, to_y).set_enemy(true);
    return true;
}