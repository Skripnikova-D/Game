#include "SpellEnhancement.hpp"
#include <iostream>

int SpellEnhancement::enhancement_points = 0;

SpellEnhancement::SpellEnhancement() {
    range = 0;
    mana_cost = 10;
}

int SpellEnhancement::get_range() const {
    return range;
}

int SpellEnhancement::get_mana_cost() const {
    return mana_cost;
}

bool SpellEnhancement::requires_target() const {
    return false;
}

int SpellEnhancement::get_enhancement_points() {
    return enhancement_points;
}

void SpellEnhancement::add_enhancement_point() {
    enhancement_points++;
}

void SpellEnhancement::reset_enhancement_points() {
    enhancement_points = 0;
}
bool SpellEnhancement::has_enhancement() {
    return enhancement_points > 0;
}

int SpellEnhancement::apply_direct_damage_enhancement(int base_range) {
    if (enhancement_points > 0) {
        int enhanced_range = base_range + enhancement_points*2;
        reset_enhancement_points();
        return enhanced_range;
    }
    return base_range;
}

int SpellEnhancement::apply_splash_enhancement(int base_area) {
    if (enhancement_points > 0) {
        int enhanced_area = base_area + enhancement_points;
        reset_enhancement_points();
        return enhanced_area;
    }
    return base_area;
}

int SpellEnhancement::apply_trap_enhancement(int base_damage) {
    if (enhancement_points > 0) {
        int enhanced_damage = base_damage + enhancement_points;
        reset_enhancement_points();
        return enhanced_damage;
    }
    return base_damage;
}

int SpellEnhancement::apply_summon_enhancement(int base_ally_count) {
    if (enhancement_points > 0) {
        int enhanced_count = base_ally_count + enhancement_points;
        reset_enhancement_points();
        return enhanced_count;
    }
    return base_ally_count;
}

bool SpellEnhancement::cast(Player& player, Board& board, std::vector<Enemy>& enemies,
                      std::vector<Ally>& allies,
                      EnemyAttackTower& attack_tower, EnemyTower& tower,
                      int targetX, int targetY) {
    if (player.get_mana() < mana_cost) {
        std::cout << "Not enough mana for enhancement spell! Need: "
                  << mana_cost << ", Have: " << player.get_mana() << std::endl;
        return false;
    }
    add_enhancement_point();
    player.set_mana(player.get_mana() - mana_cost);
    return true;
}