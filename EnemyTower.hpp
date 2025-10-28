#pragma once

#include "Board.hpp"

class EnemyTower {
private:
    int countdown;
    int x, y;
    int hp;
public:
    EnemyTower(int x_pos, int y_pos);
    bool spawn_enemy(int& spawn_x, int& spawn_y, Board& board);
    void take_damage(const int player_damage) ;
    bool is_death() const;
    void get_coords(int& cur_x, int& cur_y);
};