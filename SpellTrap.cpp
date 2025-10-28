#include "SpellTrap.hpp"
#include <iostream>
#include <cmath>
#include "SpellEnhancement.hpp"

int SpellTrap::trap_damage = 3;
SpellTrap::SpellTrap() {
    range = 4;
    mana_cost = 5;
}

int SpellTrap::get_trap_damage() {
    return trap_damage;
}

int SpellTrap::get_range() const {
    return range;
}

int SpellTrap::get_mana_cost() const {
    return mana_cost;
}

bool SpellTrap::requires_target() const {
    return true;
}

std::string SpellTrap::get_name() const {
    return "Trap";
}

bool SpellTrap::cast(Player& player, Board& board, std::vector<Enemy>& enemies,
                      std::vector<Ally>& allies,
                      EnemyAttackTower& attack_tower, EnemyTower& tower,
                      int targetX, int targetY) {
    if (player.get_mana() < mana_cost) {
        std::cout<<"Not enough mana"<<std::endl;
        return false;
    }

    if (SpellEnhancement::has_enhancement()) {
        int enhanced_damage = SpellEnhancement::apply_trap_enhancement(trap_damage);
        trap_damage = enhanced_damage;
    }

    int playerX, playerY;
    player.get_coords(playerX, playerY);
    int distance = std::abs(targetX - playerX) + std::abs(targetY - playerY);
    if (distance > range) {
        std::cout << "Target area is too far away" << std::endl;
        return false;
    }

    if (!board.can_move_to(targetX, targetY)) {
        std::cout << "Cannot place trap on obstacle or out of bounds" << std::endl;
        return false;
    }

    Cell& cell = board.get_cell(targetX, targetY);
    if (cell.is_player_here() || cell.is_enemy_here() || cell.is_trap_here()) {
        std::cout << "Target cell is occupied" << std::endl;
        return false;
    }


    cell.set_trap(true);
    player.set_mana(player.get_mana() - mana_cost);
    std::cout << "Trap placed at (" << targetX << ", " << targetY << ")" << std::endl;

    return true;
}