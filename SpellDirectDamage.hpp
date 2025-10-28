#pragma once
#include "Spell.hpp"

class SpellDirectDamage : public Spell {
private:
    int damage;
    int standart_range;
    int range;
    int mana_cost;

public:
    SpellDirectDamage();

    int get_damage() const;
    int get_range() const override;
    int get_mana_cost() const override;
    bool requires_target() const override;
    std::string get_name() const override;

    void set_range(int new_range);
    int get_standart_range() const;
    bool cast(Player& player, Board& board, std::vector<Enemy>& enemies,
                      std::vector<Ally>& allies,
                      EnemyAttackTower& attack_tower, EnemyTower& tower,
                      int targetX, int targetY) override;
};