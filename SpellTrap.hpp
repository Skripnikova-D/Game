#pragma once
#include "Spell.hpp"
class SpellTrap : public Spell {
private:
    int range;
    int mana_cost;
    static int trap_damage;
public:
    SpellTrap();

    static int get_trap_damage();
    int get_range() const override;
    int get_mana_cost() const override;
    bool requires_target() const override;

    bool cast(Player& player, Board& board, std::vector<Enemy>& enemies,
                      std::vector<Ally>& allies,
                      EnemyAttackTower& attack_tower, EnemyTower& tower,
                      int targetX, int targetY) override;
};


