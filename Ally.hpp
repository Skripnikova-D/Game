#pragma once
#include "Board.hpp"
#include "Enemy.hpp"

class EnemyTower;
class EnemyAttackTower;

enum TARGET{
    ENEMY,
    TOWER,
    ATTACK_TOWER
};
class Ally {
private:
    int x,y;
    int hp;
    int damage;
    bool can_act;
public:
    Ally(int start_x, int start_y);
    void move(Board& board, std::vector<Enemy>& enemies,
        EnemyTower& tower, EnemyAttackTower& attack_tower);
    void attack_enemy(Enemy& enemy);
    void take_damage(int enemy_damage);

    int get_hp() const;
    int get_damage() const;
    void get_coords(int& ally_x, int& ally_y) const;
    bool is_dead() const;
    void reset_turn();

};


