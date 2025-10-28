#pragma once
#include "Player.hpp"

class Ally;

class EnemyAttackTower {
private:
    int x, y;
    int damage;
    int hp;
    int countdown;
    int radius;

public:
    EnemyAttackTower(int cur_x, int cur_y);

    void get_coords(int& tower_x, int& tower_y) const;
    int get_damage() const;
    int get_hp() const;
    int get_countdown() const;

    void attack_player(Player& player);
    void attack_ally(Ally& ally);
    void attack_in_range(Player& player, std::vector<Ally>& allies);
    void take_damage(int player_damage);
    bool is_death() const;
};