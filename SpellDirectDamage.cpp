#include "SpellDirectDamage.hpp"
#include <cmath>
#include <iostream>
#include "SpellEnhancement.hpp"
SpellDirectDamage::SpellDirectDamage() {
    damage = 3;
    standart_range = 4;
    range = 4;
    mana_cost= 5;
}

int SpellDirectDamage::get_damage() const {
    return damage;
}

int SpellDirectDamage::get_range() const {
    return range;
}

int SpellDirectDamage::get_standart_range() const {
    return standart_range;
}

int SpellDirectDamage::get_mana_cost() const {
    return mana_cost;
}

bool SpellDirectDamage::requires_target() const {
    return true;
}

void SpellDirectDamage::set_range(int new_range) {
    range = new_range;
}

bool SpellDirectDamage::cast(Player& player, Board& board, std::vector<Enemy>& enemies,
                      std::vector<Ally>& allies,
                      EnemyAttackTower& attack_tower, EnemyTower& tower,
                      int targetX, int targetY) {

    if (player.get_mana() < mana_cost) {
        return false;
    }

    int current_range = range;
    if (SpellEnhancement::has_enhancement()) {
        current_range = SpellEnhancement::apply_direct_damage_enhancement(range);
    }

    int playerX, playerY;
    player.get_coords(playerX, playerY);

    int distance = std::abs(targetX - playerX) + std::abs(targetY - playerY);
    if (distance > current_range) {
        std::cout << "The target is too far away" << std::endl;
        return false;
    }

    for (auto& enemy : enemies) {
        int enemyX, enemyY;
        enemy.get_coords(enemyX, enemyY);
        if (enemyX == targetX && enemyY == targetY && !enemy.is_dead()) {
            enemy.take_damage(damage);
            player.set_mana(player.get_mana() - mana_cost);
            return true;
        }
    }

    int attackTowerX, attackTowerY;
    attack_tower.get_coords(attackTowerX, attackTowerY);
    if (attackTowerX == targetX && attackTowerY == targetY && !attack_tower.is_death()) {
        attack_tower.take_damage(damage);
        player.set_mana(player.get_mana() - mana_cost);
        return true;
    }

    int spawnTowerX, spawnTowerY;
    tower.get_coords(spawnTowerX, spawnTowerY);
    if (spawnTowerX == targetX && spawnTowerY == targetY) {
        tower.take_damage(damage);
        player.set_mana(player.get_mana() - mana_cost);
        return true;
    }

    std::cout << "No valid target found" << std::endl;
    return false;
}