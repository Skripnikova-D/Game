#pragma once

class Cell {
private:
    bool is_enemy;
    bool is_player;
    bool is_obstacle;
    bool is_slow;
    bool is_trap;
    bool is_ally;
public:
    Cell();

    bool is_enemy_here() const;
    bool is_player_here() const;
    bool is_obstacle_here() const;
    bool is_slow_here() const;
    bool is_trap_here() const;
    bool is_ally_here() const;

    void set_player(bool condition_now);
    void set_enemy(bool condition_now);
    void set_obstacle(bool condition_now);
    void set_slow(bool condition_now);
    void set_trap(bool condition_now);
    void set_ally(bool condition_now);
};