cmake_minimum_required(VERSION 3.15)
project(OOP)

set(CMAKE_CXX_STANDARD 14)

# Ищем SFML
find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

include_directories(.)
include_directories(assets)

add_executable(OOP
        assets/ArialRegular.ttf
        assets/empty_cell.png
        assets/enemy.png
        assets/obstacle_cell.png
        assets/player.png
        assets/slow_cell.png
        assets/tower.png
        Board.cpp Board.hpp
        Cell.cpp Cell.hpp
        Enemy.cpp Enemy.hpp
        EnemyTower.cpp EnemyTower.hpp
        Game1.cpp
        Player.cpp Player.hpp
        SpellDirectDamage.cpp
        EnemyAttackTower.cpp
        EnemyAttackTower.hpp
        Spell.hpp
        SpellDirectDamage.hpp
        SpellSplash.cpp
        SpellSplash.hpp
        SpellTrap.cpp
        SpellTrap.hpp
        SpellSummon.cpp
        SpellSummon.hpp
        Ally.cpp
        Ally.hpp
        SpellEnhancement.cpp
        SpellEnhancement.hpp
        PlayersHand.cpp
        PlayersHand.hpp)

target_link_libraries(OOP SFML::Graphics SFML::Window SFML::System)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})