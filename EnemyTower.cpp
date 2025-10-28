#include "EnemyTower.hpp"

EnemyTower::EnemyTower(int x_pos, int y_pos) {
    countdown = 10;
    x = x_pos - 1;
    y = y_pos - 1;
    hp=50;
}

bool EnemyTower::spawn_enemy(int& spawn_x, int& spawn_y, Board& board) {
    if (countdown > 0) {
        countdown--;
        return false;
    }

    std::vector<std::pair<int, int>> positions = {
        {x - 1 , y},
        {x, y - 1}
    };

    for (const auto& pos : positions) {
        int pos_x = pos.first;
        int pos_y = pos.second;

        if (board.can_move_to(pos_x, pos_y) &&
            !board.get_cell(pos_x, pos_y).is_enemy_here() &&
            !board.get_cell(pos_x, pos_y).is_player_here()) {

            spawn_x = pos_x;
            spawn_y = pos_y;
            countdown = 10;
            return true;
        }
    }

    countdown = 1;
    return false;
}

void EnemyTower::take_damage(const int player_damage) {
    if (player_damage > hp) {
        hp = 0;
    }
    else {
        hp = hp - player_damage;
    }
}

bool EnemyTower::is_death() const {
    return  hp==0;
}

void EnemyTower::get_coords(int& cur_x, int& cur_y){
    cur_x = x;
    cur_y = y;
}
