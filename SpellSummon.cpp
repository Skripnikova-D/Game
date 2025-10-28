#include "SpellSummon.hpp"
#include "Ally.hpp"
#include <iostream>
#include <vector>
#include "SpellEnhancement.hpp"

SpellSummon::SpellSummon() {
    mana_cost= 5;
    range=1;
}

int SpellSummon::get_range() const {
    return range;
}

int SpellSummon::get_mana_cost() const {
    return mana_cost;
}

bool SpellSummon::requires_target() const {
    return false;
}

bool SpellSummon::cast(Player& player, Board& board, std::vector<Enemy>& enemies,
                      std::vector<Ally>& allies,
                      EnemyAttackTower& attack_tower, EnemyTower& tower,
                      int targetX, int targetY) {

    if (player.get_mana() < mana_cost) {
        std::cout << "Not enough mana to summon ally" << std::endl;
        return false;
    }

    int ally_count = 1;
    if (SpellEnhancement::has_enhancement()) {
        ally_count = SpellEnhancement::apply_summon_enhancement(ally_count);
    }

    int playerX, playerY;
    player.get_coords(playerX, playerY);

    std::vector<std::pair<int, int>> possible_positions = {
        {playerX + 1, playerY}, {playerX - 1, playerY},
        {playerX, playerY + 1}, {playerX, playerY - 1},
        {playerX + 1, playerY + 1}, {playerX + 1, playerY - 1},
        {playerX - 1, playerY + 1}, {playerX - 1, playerY - 1}
    };

    int summoned_count = 0;
    for (const auto& pos : possible_positions) {
        if (summoned_count >= ally_count) break;

        int spawnX = pos.first;
        int spawnY = pos.second;

        if (spawnX >= 0 && spawnX < board.get_length() &&
            spawnY >= 0 && spawnY < board.get_width() &&
            board.can_move_to(spawnX, spawnY)) {

            Cell& cell = board.get_cell(spawnX, spawnY);
            if (!cell.is_player_here() && !cell.is_enemy_here() &&
                !cell.is_ally_here() && !cell.is_trap_here()) {

                allies.emplace_back(spawnX, spawnY);
                cell.set_ally(true);
                summoned_count++;

                }
            }
    }
    if (summoned_count > 0) {
        player.set_mana(player.get_mana() - mana_cost);
        return true;
    }

    std::cout << "No space to summon ally nearby" << std::endl;
    return false;
}