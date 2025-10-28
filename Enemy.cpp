#include "Enemy.hpp"
#include "Player.hpp"
#include "Ally.hpp"
#include "SpellTrap.hpp"
#include <iostream>

Enemy::Enemy(int start_x, int start_y) {
    hp = 10;
    damage = 2;
    x = start_x;
    y = start_y;
    can_act = true;
    is_slowed = false;
}

void Enemy::move(Player& player, Board& board, std::vector<Ally>& allies) {
    if (is_dead() || !can_act) return;

    int player_x, player_y;
    player.get_coords(player_x, player_y);

    int new_x = x, new_y = y;

    if (x < player_x) new_x++;
    else if (x > player_x) new_x--;
    else if (y < player_y) new_y++;
    else if (y > player_y) new_y--;

    if (new_x >= 0 && new_x < board.get_length() &&
        new_y >= 0 && new_y < board.get_width()) {

        Cell& target_cell = board.get_cell(new_x, new_y);

        if (target_cell.is_trap_here()) {
            take_damage(SpellTrap::get_trap_damage());
            target_cell.set_trap(false);
            std::cout << "Enemy triggered a trap!" << std::endl;
        }

        if (target_cell.is_ally_here()) {
            for (auto& ally : allies) {
                int ally_x, ally_y;
                ally.get_coords(ally_x, ally_y);
                if (ally_x == new_x && ally_y == new_y && !ally.is_dead()) {
                    attack_ally(ally);
                    std::cout << "Enemy attacked ally! Ally HP: " << ally.get_hp() << std::endl;
                    can_act = false;
                    return;
                }
            }
        }

        if (target_cell.is_player_here()) {
            attack_player(player);
            std::cout << "Enemy attacked player! Player HP: " << player.get_hp() << std::endl;
            can_act = false;
        }

        else if (board.can_move_to(new_x, new_y) &&
         !target_cell.is_enemy_here() &&
         !target_cell.is_tower_here() &&
         !target_cell.is_attack_tower_here()) {
            board.get_cell(x, y).set_enemy(false);
            board.get_cell(new_x, new_y).set_enemy(true);

            x = new_x;
            y = new_y;
            can_act = false;

            if (board.get_cell(x, y).is_slow_here()) {
                apply_slow();
            }
        }
    }
}

void Enemy::apply_slow() {
    is_slowed = true;
}

void Enemy::reset_turn() {
    can_act = !is_slowed;
    is_slowed = false;
}

void Enemy::attack_player(Player& player) {
    int player_x, player_y;
    player.get_coords(player_x, player_y);

    std::cout << "Enemy at (" << x << "," << y << ") attacking player at (" << player_x << "," << player_y << ")" << std::endl;

    player.take_damage(damage);
    std::cout << "Enemy deals " << damage << " damage to player!" << std::endl;
    std::cout << "Player HP after attack: " << player.get_hp() << std::endl;

    can_act = false;
}

void Enemy::attack_ally(Ally &ally) {
    int ally_x, ally_y;
    ally.get_coords(ally_x, ally_y);

    std::cout << "Enemy at (" << x << "," << y << ") attacking ally at (" << ally_x << "," << ally_y << ")" << std::endl;
    ally.take_damage(damage);
    std::cout << "Enemy deals " << damage << " damage to ally!" << std::endl;
    std::cout << "Ally HP after attack: " << ally.get_hp() << std::endl;
    can_act = false;
}

void Enemy::take_damage(int player_damage) {
    int old_hp = hp;
    if (player_damage > hp) {
        hp = 0;
    }
    else {
        hp = hp - player_damage;
    }
    std::cout << "Enemy: " << old_hp << " -> " << hp << " (-" << player_damage << ")" << std::endl;
}

int Enemy::get_hp() const {
    return hp;
}

int Enemy::get_damage() const {
    return damage;
}

void Enemy::get_coords(int& enemy_x, int& enemy_y) const {
    enemy_x = x;
    enemy_y = y;
}

bool Enemy::is_dead() const {
    return hp == 0;
}