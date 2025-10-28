#include "Cell.hpp"

Cell::Cell() {
    is_enemy = false;
    is_player = false;
    is_obstacle = false;
    is_slow = false;
    is_trap = false;
    is_ally = false;
}

bool Cell::is_enemy_here() const {
    return is_enemy;
}

bool Cell::is_player_here() const {
    return is_player;
}

bool Cell::is_obstacle_here() const {
    return is_obstacle;
}

bool Cell::is_slow_here() const {
    return is_slow;
}
bool Cell::is_ally_here() const {
    return is_ally;
}

void Cell::set_player(bool condition_now) {
    is_player = condition_now;
}

void Cell::set_enemy(bool condition_now) {
    is_enemy = condition_now;
}

void Cell::set_obstacle(bool condition_now) {
    is_obstacle = condition_now;
}

void Cell::set_slow(bool condition_now) {
    is_slow = condition_now;
}

bool Cell::is_trap_here() const {
    return is_trap;
}

void Cell::set_trap(bool condition_now) {
    is_trap = condition_now;
}

void Cell::set_ally(bool condition_now) {
    is_ally = condition_now;
}