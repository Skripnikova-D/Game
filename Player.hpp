#pragma once
#include <vector>

class Board;
class Enemy;
class EnemyTower;
class EnemyAttackTower;

enum class fight_mode {
    MELEE, //5
    RANGE  //4
};

class Player {
private:
    int hp;
    int damage;
    int x, y;
    fight_mode mode;
    bool can_act;
    bool is_slowed;
    int mana;
    int max_mana;
public:
    Player();

    int get_hp() const;
    int get_damage() const;
    void get_coords(int& player_x, int& player_y) const;
    fight_mode get_mode() const;
    bool get_can_act() const;
    bool get_is_slowed() const;
    void apply_slow();
    void set_position(const int& new_x, const int& new_y);
    void move(char movement_button, Board& board, std::vector<Enemy>& enemies,
              EnemyTower& spawn_tower, EnemyAttackTower& attack_tower);
    void melee_attack(Enemy& enemy);
    void reset_step();
    void change_attack_mode();
    void take_damage(int enemy_damage);
    bool is_dead() const;
    int get_mana() const;
    int get_max_mana() const;
    void set_mana(int new_mana);
};