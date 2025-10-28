#pragma once
#include "Spell.hpp"
class SpellSummon : public Spell {
private:
    int range;
    int mana_cost;
public:
    SpellSummon();

    int get_range() const override;
    int get_mana_cost() const override;
    bool requires_target() const override;

    bool cast(Player& player, Board& board, std::vector<Enemy>& enemies,
                      std::vector<Ally>& allies,
                      EnemyAttackTower& attack_tower, EnemyTower& tower,
                      int targetX, int targetY) override;
};