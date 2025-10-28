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

std::string SpellSummon::get_name() const {
    return "Summon Ally";
}

bool SpellSummon::cast(Player& player, Board& board, std::vector<Enemy>& enemies,
                      std::vector<Ally>& allies, EnemyAttackTower& attack_tower,
                      EnemyTower& spawn_tower, int target_x, int target_y) {

    int player_x, player_y;
    player.get_coords(player_x, player_y);
    std::vector<std::pair<int, int>> positions = {
        {player_x - 1, player_y},
        {player_x + 1, player_y},
        {player_x, player_y - 1},
        {player_x, player_y + 1}
    };

    for (const auto& pos : positions) {
        int pos_x = pos.first;
        int pos_y = pos.second;
        if (pos_x >= 0 && pos_x < board.get_length() &&
            pos_y >= 0 && pos_y < board.get_width()) {

            Cell& cell = board.get_cell(pos_x, pos_y);

            if (!cell.is_obstacle_here() && !cell.is_enemy_here() &&
                !cell.is_player_here() && !cell.is_ally_here() &&
                !cell.is_tower_here() && !cell.is_attack_tower_here()) {
                allies.emplace_back(pos_x, pos_y);
                cell.set_ally(true);
                return true;
            } else {
                std::cout << "NOT SUITABLE" << std::endl;
            }
        } else {
            std::cout << "OUT OF BOUNDS" << std::endl;
        }
    }

    std::cout << "No suitable position found for summoning" << std::endl;
    std::cout << "=== END SUMMON DEBUG ===" << std::endl;
    return false;
}