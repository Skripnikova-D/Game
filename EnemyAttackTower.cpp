#include "EnemyAttackTower.hpp"
#include "Player.hpp"
#include "Ally.hpp"
#include <iostream>
EnemyAttackTower::EnemyAttackTower(int cur_x, int cur_y) {
    x = cur_x;
    y = cur_y;
    damage = 2;
    hp = 30;
    countdown = 5;
    radius = 3;
}

void EnemyAttackTower::get_coords(int& tower_x, int& tower_y) const {
    tower_x = x;
    tower_y = y;
}

int EnemyAttackTower::get_damage() const {
    return damage;
}

int EnemyAttackTower::get_hp() const {
    return hp;
}

int EnemyAttackTower::get_countdown() const {
    return countdown;
}

void EnemyAttackTower::attack_in_range(Player& player, std::vector<Ally>& allies) {
    if (countdown <= 0) {
        bool attacked = false;

        int player_x, player_y;
        player.get_coords(player_x, player_y);
        int distance_to_player = (player_x - x) * (player_x - x) + (player_y - y) * (player_y - y);

        if (distance_to_player <= radius * radius && !player.is_dead()) {
            attack_player(player);
            attacked = true;
        }

        if (!attacked) {
            for (auto& ally : allies) {
                if (ally.is_dead()) continue;

                int ally_x, ally_y;
                ally.get_coords(ally_x, ally_y);
                int distance_to_ally = (ally_x - x) * (ally_x - x) + (ally_y - y) * (ally_y - y);

                if (distance_to_ally <= radius * radius) {
                    attack_ally(ally);
                    attacked = true;
                    break;
                }
            }
        }

        if (attacked) {
            countdown = 5;
        } else {
            countdown = 0;
        }
    } else {
        countdown--;
    }
}

void EnemyAttackTower::attack_player(Player& player) {
    int player_x, player_y;
    player.get_coords(player_x, player_y);
    player.take_damage(damage);
    std::cout << "Attack tower deals " << damage << " damage to player!" << std::endl;
}

void EnemyAttackTower::attack_ally(Ally& ally) {
    int ally_x, ally_y;
    ally.get_coords(ally_x, ally_y);
    ally.take_damage(damage);
    std::cout << "Attack tower deals " << damage << " damage to ally!" << std::endl;
}

void EnemyAttackTower::take_damage(int player_damage) {
    int old_hp = hp;
    if (player_damage > hp) {
        hp = 0;
    } else {
        hp = hp - player_damage;
    }
    std::cout << "Attack Tower: " << old_hp << " -> " << hp << " (-" << player_damage << ")" << std::endl;
}

bool EnemyAttackTower::is_death() const {
    return hp == 0;
}