#include "Ally.hpp"
#include "EnemyTower.hpp"
#include "EnemyAttackTower.hpp"
#include <iostream>
#include <cmath>

Ally::Ally(int start_x, int start_y) {
    hp = 10;
    damage = 2;
    x = start_x;
    y = start_y;
    can_act = true;
}

void Ally::move(Board& board, std::vector<Enemy>& enemies,
        EnemyTower& tower, EnemyAttackTower& tower_attack) {
    if (is_dead() || !can_act) return;

    TARGET target_type = ENEMY;
    Enemy* closest_enemy = nullptr;
    int min_distance = 1000;
    int target_x = -1, target_y = -1;

    for (auto& enemy : enemies) {
        if (!enemy.is_dead()) {
            int enemy_x, enemy_y;
            enemy.get_coords(enemy_x, enemy_y);
            int distance = std::abs(enemy_x - x) + std::abs(enemy_y - y);
            if (distance < min_distance) {
                min_distance = distance;
                closest_enemy = &enemy;
                target_x = enemy_x;
                target_y = enemy_y;
                target_type = ENEMY;
            }
        }
    }

    int towerX, towerY;
    tower.get_coords(towerX, towerY);
    int distance_tower = std::abs(towerX - x) + std::abs(towerY - y);

    int tower_attackX, tower_attackY;
    tower_attack.get_coords(tower_attackX, tower_attackY);
    int distance_attack_tower = std::abs(tower_attackX - x) + std::abs(tower_attackY - y);

    if (closest_enemy) {
        // выбираем самую близкую цель из доступных
        if (min_distance <= distance_tower && min_distance <= distance_attack_tower) {
            target_type = ENEMY;
        } else if (distance_tower <= distance_attack_tower && !tower.is_death()) {
            target_x = towerX;
            target_y = towerY;
            target_type = TOWER;
        } else if (!tower_attack.is_death()) {
            target_x = tower_attackX;
            target_y = tower_attackY;
            target_type = ATTACK_TOWER;
        } else {
            target_type = ENEMY; // Башни мертвы - атакуем врага
        }
    } else {
        // Нет врагов - атакуем башни
        if (!tower.is_death() && !tower_attack.is_death()) {
            target_x = (distance_tower <= distance_attack_tower) ? towerX : tower_attackX;
            target_y = (distance_tower <= distance_attack_tower) ? towerY : tower_attackY;
            target_type = (distance_tower <= distance_attack_tower) ? TOWER : ATTACK_TOWER;
        } else if (!tower.is_death()) {
            target_x = towerX;
            target_y = towerY;
            target_type = TOWER;
        } else if (!tower_attack.is_death()) {
            target_x = tower_attackX;
            target_y = tower_attackY;
            target_type = ATTACK_TOWER;
        } else {
            std::cout << "All targets destroyed!" << std::endl;
            can_act = false;
            return;
        }
    }

    if (target_x == -1 || target_y == -1) {
        can_act = false;
        return;
    }

    int new_x = x, new_y = y;
    if (x < target_x) new_x++;
    else if (x > target_x) new_x--;
    else if (y < target_y) new_y++;
    else if (y > target_y) new_y--;

    if (new_x >= 0 && new_x < board.get_length() &&
        new_y >= 0 && new_y < board.get_width()) {

        Cell& target_cell = board.get_cell(new_x, new_y);

        // Атака врага на целевой клетке
        if (target_cell.is_enemy_here() && target_type == ENEMY) {
            for (auto& enemy : enemies) {
                int ex, ey;
                enemy.get_coords(ex, ey);
                if (ex == new_x && ey == new_y && !enemy.is_dead()) {
                    attack_enemy(enemy);
                    can_act = false;
                    return;
                }
            }
        }
        else if (new_x == towerX && new_y == towerY && target_type == TOWER) {
            tower.take_damage(damage);
            std::cout << "Ally attacked spawn tower for " << damage << " damage!" << std::endl;
            can_act = false;
            return;
        }
        else if (new_x == tower_attackX && new_y == tower_attackY && target_type == ATTACK_TOWER) {
            tower_attack.take_damage(damage);
            std::cout << "Ally attacked attack tower for " << damage << " damage!" << std::endl;
            can_act = false;
            return;
        }
        else if (board.can_move_to(new_x, new_y) &&
         !target_cell.is_enemy_here() &&
         !target_cell.is_tower_here() &&
         !target_cell.is_attack_tower_here() &&
         !target_cell.is_ally_here() &&
         !target_cell.is_player_here()) {
            board.get_cell(x, y).set_ally(false);
            board.get_cell(new_x, new_y).set_ally(true);
            x = new_x;
            y = new_y;
            can_act = false;
        }
    }
    can_act = false;
}

void Ally::attack_enemy(Enemy& enemy) {
    enemy.take_damage(damage);
    std::cout << "Ally attacked enemy for " << damage << " damage!" << std::endl;
}

void Ally::take_damage(int enemy_damage) {
    int old_hp=hp;
    if (enemy_damage > hp) {
        hp = 0;
    } else {
        hp -= enemy_damage;
    }
    std::cout << "Ally: " << old_hp << " -> " << hp << " (-" << enemy_damage << ")" << std::endl;
}

int Ally::get_hp() const {
    return hp;
}

int Ally::get_damage() const {
    return damage;
}

void Ally::get_coords(int& ally_x, int& ally_y) const {
    ally_x = x;
    ally_y = y;
}

bool Ally::is_dead() const {
    return hp == 0;
}

void Ally::reset_turn() {
    can_act = true;
}