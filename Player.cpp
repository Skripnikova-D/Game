#include "Player.hpp"
#include "Board.hpp"
#include "Enemy.hpp"
#include "EnemyTower.hpp"
#include "EnemyAttackTower.hpp"
#include <iostream>
Player::Player() {
    hp = 20;
    damage = 2;
    x = 0;
    y = 0;
    mode = fight_mode::MELEE;
    can_act = true;
    is_slowed = false;
    mana=0;
    max_mana=25;
}

int Player::get_hp() const {
    return hp;
}

int Player::get_damage() const {
    return damage;
}

void Player::get_coords(int& player_x, int& player_y) const {
    player_x = x;
    player_y = y;
}

fight_mode Player::get_mode() const {
    return mode;
}

bool Player::get_can_act()const{
    return can_act;
}

bool Player::get_is_slowed() const{
    return is_slowed;
}

void Player::apply_slow() {
    is_slowed = true;
}

void Player::set_position(const int& new_x, const int& new_y) {
    x = new_x;
    y = new_y;
}

void Player::move(char movement_button, Board& board, std::vector<Enemy>& enemies,
                  EnemyTower& spawn_tower, EnemyAttackTower& attack_tower) {
    if (can_act) {
        int new_x = x, new_y = y;

        switch (movement_button) {
        case 'W': new_y--; break;
        case 'A': new_x--; break;
        case 'S': new_y++; break;
        case 'D': new_x++; break;
        default: return;
        }

        if (new_x >= 0 && new_x < board.get_length() &&
            new_y >= 0 && new_y < board.get_width()) {

            Cell& target_cell = board.get_cell(new_x, new_y);

            if (target_cell.is_enemy_here() && mode == fight_mode::MELEE) {
                for (auto& enemy : enemies) {
                    int enemy_x, enemy_y;
                    enemy.get_coords(enemy_x, enemy_y);
                    if (enemy_x == new_x && enemy_y == new_y && !enemy.is_dead()) {
                        melee_attack(enemy);
                        std::cout << "Player attacked enemy! Enemy HP: " << enemy.get_hp() << std::endl;
                        if (enemy.is_dead()) {
                            target_cell.set_enemy(false);
                            std::cout << "Enemy defeated!" << std::endl;
                        }
                        can_act = false;
                        return;
                    }
                }
            }
            else if (target_cell.is_tower_here() && mode == fight_mode::MELEE) {
                int tower_x, tower_y;
                spawn_tower.get_coords(tower_x, tower_y);
                std::cout<<"Try attack "<<std::endl;
                std::cout << "Player position: " << new_x << ", " << new_y << std::endl;
                std::cout << "Tower position: " << tower_x << ", " << tower_y << std::endl;
                if (tower_x == new_x && tower_y == new_y) {
                    spawn_tower.take_damage(damage);
                    std::cout << "Player attacked spawn tower for " << damage << " damage!" << std::endl;
                    if (spawn_tower.is_death()) {
                        target_cell.set_tower(false);
                        std::cout << "Spawn tower destroyed!" << std::endl;
                    }
                    can_act = false;
                    return;
                }
            }
            else if (target_cell.is_attack_tower_here() && mode == fight_mode::MELEE) {
                int attack_tower_x, attack_tower_y;
                attack_tower.get_coords(attack_tower_x, attack_tower_y);
                if (attack_tower_x == new_x && attack_tower_y == new_y) {
                    attack_tower.take_damage(damage);
                    std::cout << "Player attacked attack tower for " << damage << " damage!" << std::endl;
                    if (attack_tower.is_death()) {
                        target_cell.set_attack_tower(false);
                        std::cout << "Attack tower destroyed!" << std::endl;
                    }
                    can_act = false;
                    return;
                }
            }
            else if (board.can_move_to(new_x, new_y)) {
                if (!target_cell.is_tower_here() && !target_cell.is_attack_tower_here() &&
                    !target_cell.is_ally_here()){
                    board.get_cell(x, y).set_player(false);
                    board.get_cell(new_x, new_y).set_player(true);

                    x = new_x;
                    y = new_y;
                    can_act = false;

                    if (board.get_cell(x, y).is_slow_here()) {
                        apply_slow();
                        std::cout << "Stepped on slow cell! Will skip next turn." << std::endl;
                    }
                } else {
                    std::cout << "Cannot move into taken cell!" << std::endl;
                }
            }
        }
    }
    else {
        std::cout << "Sorry, honey, but you've already wasted your step." << std::endl;
    }
}

void Player::reset_step() {
    can_act = !is_slowed;
    is_slowed = false;
}

void Player::melee_attack(Enemy& enemy)  {
    enemy.take_damage(damage);
}

void Player::change_attack_mode() {

    if (can_act) {
        mode = (mode == fight_mode::MELEE) ? fight_mode::RANGE : fight_mode::MELEE;
        damage = (mode == fight_mode::MELEE) ? 5 : 4;
        can_act = false;
    }
    else {
        std::cout << "Sorry, honey, but you've already wasted your step." << std::endl;
    }
}

void Player::take_damage(int enemy_damage) {
    std::cout << "Player taking " << enemy_damage << " damage. Current HP: " << hp << std::endl;
    if (enemy_damage > hp) {
        hp = 0;
    }
    else {
        hp = hp - enemy_damage;
    }
    std::cout << "Player HP after damage: " << hp << std::endl;
}

bool Player::is_dead() const {
    return hp == 0;
}
int Player::get_mana() const {
    return mana;
}

int Player::get_max_mana() const {
    return max_mana;
}
void Player::set_mana(int new_mana) {
    mana = new_mana;
}

//добавить ренжовую атаку крестиком
