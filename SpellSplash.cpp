#include "SpellSplash.hpp"
#include <iostream>
#include <cmath>
#include "SpellEnhancement.hpp"
SpellSplash::SpellSplash() {
    range = 4;
    mana_cost = 10;
    damage = 5;
    area_size=1;
}

int SpellSplash::get_damage() const {
    return damage;
}

int SpellSplash::get_range() const {
    return range;
}

int SpellSplash::get_mana_cost() const {
    return mana_cost;
}

bool SpellSplash::requires_target() const {
    return true;
}

std::string SpellSplash::get_name() const {
    return "Splash Damage";
}

bool SpellSplash::cast(Player& player, Board& board, std::vector<Enemy>& enemies,
                      std::vector<Ally>& allies,
                      EnemyAttackTower& attack_tower, EnemyTower& tower,
                      int targetX, int targetY) {

    if (player.get_mana() < mana_cost) {
        return false;
    }

    int current_area = area_size;
    if (SpellEnhancement::has_enhancement()) {
        current_area = SpellEnhancement::apply_splash_enhancement(area_size);
    }

    int playerX, playerY;
    player.get_coords(playerX, playerY);
    int distance = std::abs(targetX - playerX) + std::abs(targetY - playerY);
    if (distance > range) {
        std::cout << "Target area is too far away" << std::endl;
        return false;
    }

    player.set_mana(player.get_mana() - mana_cost);

    int length = board.get_length();
    int width = board.get_width();
    bool hit_anything = false;

    for (int x = targetX; x <= targetX + current_area; x++) {
        for (int y = targetY; y <= targetY + current_area; y++) {
            if (x >= 0 && y >= 0 && x < length && y < width) {
                for (auto& enemy : enemies) {
                    int enemyX, enemyY;
                    enemy.get_coords(enemyX, enemyY);
                    if (enemyX == x && enemyY == y && !enemy.is_dead()) {
                        enemy.take_damage(damage);
                        hit_anything = true;
                        std::cout << "Splash hit enemy at (" << x << ", " << y << ")" << std::endl;
                    }
                }

                int attackTowerX, attackTowerY;
                attack_tower.get_coords(attackTowerX, attackTowerY);
                if (attackTowerX == x && attackTowerY == y && !attack_tower.is_death()) {
                    attack_tower.take_damage(damage);
                    hit_anything = true;
                    std::cout << "Splash hit attack tower at (" << x << ", " << y << ")" << std::endl;
                }

                int spawnTowerX, spawnTowerY;
                tower.get_coords(spawnTowerX, spawnTowerY);
                if (spawnTowerX == x && spawnTowerY == y) {
                    tower.take_damage(damage);
                    hit_anything = true;
                    std::cout << "Splash hit spawn tower at (" << x << ", " << y << ")" << std::endl;
                }
            }
        }
    }
    if (!hit_anything) {
        std::cout << "Splash spell cast but hit nothing in the area" << std::endl;
    }
    return true;
}