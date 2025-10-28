#pragma once
#include "Spell.hpp"

class SpellEnhancement: public Spell {
private:
    int range;
    int mana_cost;
    static int enhancement_points;
public:
    SpellEnhancement();

    int get_range() const override;
    int get_mana_cost() const override;
    bool requires_target() const override;
    std::string get_name() const override;
    static int get_enhancement_points();
    static void add_enhancement_point();
    static void reset_enhancement_points();
    static bool has_enhancement();

    static int apply_direct_damage_enhancement(int base_range);
    static int apply_splash_enhancement(int base_area_boost);
    static int apply_trap_enhancement(int base_damage);
    static int apply_summon_enhancement(int base_ally_count);

    bool cast(Player& player, Board& board, std::vector<Enemy>& enemies,
                      std::vector<Ally>& allies,
                      EnemyAttackTower& attack_tower, EnemyTower& tower,
                      int targetX, int targetY) override;
};