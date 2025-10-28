#pragma once
#include "Spell.hpp"

class SpellSplash : public Spell {
private:
    int damage;
    int range;
    int mana_cost;
    int area_size;
public:
    SpellSplash();

    int get_damage() const;
    int get_range() const override;
    int get_mana_cost() const override;
    bool requires_target() const override;
    std::string get_name() const override;

    bool cast(Player& player, Board& board, std::vector<Enemy>& enemies,
                      std::vector<Ally>& allies,
                      EnemyAttackTower& attack_tower, EnemyTower& tower,
                      int targetX, int targetY) override;
};