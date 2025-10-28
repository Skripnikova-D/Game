#pragma once
#include "Board.hpp"

class Player;
class Ally;

class Enemy {
private:
    int hp;
    int damage;
    int x, y;
    bool can_act;
    bool is_slowed;

public:
    Enemy(int start_x, int start_y);

    void move(Player& player, Board& board, std::vector<Ally>& allies);
    void apply_slow();
    void reset_turn();
    void attack_player(Player& player);
    void attack_ally(Ally& ally);
    void take_damage(int player_damage);
    int get_hp() const;
    int get_damage() const;
    void get_coords(int& enemy_x, int& enemy_y) const;
    bool is_dead() const;
};