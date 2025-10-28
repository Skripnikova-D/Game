#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

#include "Cell.hpp"
#include "Board.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "EnemyTower.hpp"
#include "EnemyAttackTower.hpp"
#include "Ally.hpp"
#include "PlayersHand.hpp"


int main() {
    int length, width = 0;
    std::cout << "Please enter the board size. "
                 "\nThe width and length must not be less than 10 and more than 25, \n otherwise a 10x10 board will be created." << std::endl;
    std::cin >> length >> width;
    Board board(length, width);
    length = board.get_length();
    width = board.get_width();

    EnemyTower tower(length - 1, width - 1);
    board.get_cell(length - 1, width - 1).set_tower(true);

    int a_towerX, a_towerY;
    std::srand(std::time(0));
    int attempts = 0;
    bool position_found = false;

    while (attempts < length * width && !position_found) {
        a_towerX = std::rand() % length;
        a_towerY = std::rand() % width;

        // Проверяем условия:
        if ((a_towerX != 0 || a_towerY != 0) &&
            (a_towerX != length - 1 || a_towerY != width - 1) &&
            board.can_move_to(a_towerX, a_towerY) &&
            !board.get_cell(a_towerX, a_towerY).is_obstacle_here() &&
            !board.get_cell(a_towerX, a_towerY).is_player_here() &&
            !board.get_cell(a_towerX, a_towerY).is_enemy_here()) {

            position_found = true;
            }
        attempts++;
    }

    EnemyAttackTower attack_tower(a_towerX,a_towerY);
    board.get_cell(a_towerX, a_towerY).set_attack_tower(true);

    Player player;
    board.get_cell(0, 0).set_player(true);
    player.set_position(0, 0);

    PlayersHand player_hand;
    player_hand.AddRandomSpell();

    std::vector<Ally> allies;
    std::vector<Enemy> enemies;

    std::vector<std::pair<int, int>> spawn_positions = {
        {length - 2, width - 1},
        {length - 1, width - 2},
        {length - 2, width - 2}
    };

    for (const auto& pos : spawn_positions) {
        if (!board.get_cell(pos.first, pos.second).is_obstacle_here() &&
            !board.get_cell(pos.first, pos.second).is_player_here()) {
            Enemy enemy(pos.first, pos.second);
            enemies.push_back(enemy);
            board.get_cell(pos.first, pos.second).set_enemy(true);
            break; // Только одного врага
            }
    }

    bool player_turn = true;
    bool game_over = false;
    bool casting_spell = false;
    int selected_spell_index = -1;
    bool victory = false;

    sf::RenderWindow window(sf::VideoMode({ 1000, 800 }), "OOP_GAME", sf::Style::Default);

    sf::Texture texture_player, texture_enemy, texture_ally;
    sf::Texture texture_empty_cell, texture_obstacle_cell, texture_slow_cell, texture_trap_cell;
    sf::Texture texture_tower, texture_attack_tower;
    //добавить текстуры для 5 карточек заклинаний

    if (!texture_player.loadFromFile("assets/player.png")) {
        std::cout << "Failed to load player.png" << std::endl;
        return -1;
    }else {
        std::cout<<"Success load player.png"<<std::endl;
    }

    if (!texture_enemy.loadFromFile("assets/enemy.png")) {
        std::cout << "Failed to load enemy.png" << std::endl;
        return -1;
    }else {
        std::cout<<"Success load enemy.png"<<std::endl;
    }

    if (!texture_ally.loadFromFile("assets/Ally.png")) {
        std::cout << "Failed to load Ally.png" << std::endl;
        return -1;
    }else {
        std::cout<<"Success load Ally.png"<<std::endl;
    }

    if (!texture_empty_cell.loadFromFile("assets/empty_cell.png")) {
        std::cout << "Failed to load empty_cell.png" << std::endl;
        return -1;
    }else {
        std::cout<<"Success load empty_cell.png"<<std::endl;
    }

    if (!texture_slow_cell.loadFromFile("assets/slow_cell.png")) {
        std::cout << "Failed to load slow_cell.png" << std::endl;
        return -1;
    }else {
        std::cout<<"Success load slow_cell.png"<<std::endl;
    }

    if (!texture_obstacle_cell.loadFromFile("assets/obstacle_cell.png")) {
        std::cout << "Failed to load obstacle_cell.png" << std::endl;
        return -1;
    } else {
        std::cout << "Success load obstacle_cell.png" << std::endl;
    }

    if (!texture_trap_cell.loadFromFile("assets/trap_cell.png")) {
        std::cout << "Failed to load trap_cell.png" << std::endl;
        return -1;
    } else {
        std::cout << "Success load trap_cell.png" << std::endl;
    }

    if (!texture_tower.loadFromFile("assets/tower.png")) {
        std::cout << "Failed to load tower.png" << std::endl;
        return -1;
    }else {
        std::cout<<"Success load tower.png"<<std::endl;
    }

    if (!texture_attack_tower.loadFromFile("assets/attack_tower.png")) {
        std::cout << "Failed to load attack_tower.png" << std::endl;
        return -1;
    }else {
        std::cout<<"Success load attack_tower.png"<<std::endl;
    }

    sf::Font font;
    sf::FileInputStream fontStream;
    if (!fontStream.open("assets/ArialRegular.ttf")) {
        std::cout << "Failed to open arial.ttf file" << std::endl;
        return -1;
    }
    if (!font.openFromStream(fontStream)) {
        std::cout << "Failed to load arial.ttf font" << std::endl;
        return -1;
    }

    sf::Sprite sprite_player(texture_player);
    sf::Sprite sprite_enemy(texture_enemy);
    sf::Sprite sprite_empty_cell(texture_empty_cell);
    sf::Sprite sprite_slow_cell(texture_slow_cell);
    sf::Sprite sprite_trap_cell(texture_trap_cell);
    sf::Sprite sprite_tower(texture_tower);
    sf::Sprite sprite_attack_tower(texture_attack_tower);
    sf::Sprite sprite_obstacle_cell(texture_obstacle_cell);
    sf::Sprite sprite_ally(texture_ally);

    //text
    sf::Text game_over_text(font);
    game_over_text.setString("GAME OVER");
    game_over_text.setCharacterSize(50);
    game_over_text.setFillColor(sf::Color::Red);
    game_over_text.setPosition({ 400.0f, 350.0f });

    sf::Text victory_text(font);
    victory_text.setString("VICTORY!");
    victory_text.setCharacterSize(50);
    victory_text.setFillColor(sf::Color::Red);
    victory_text.setPosition({ 400.0f, 350.0f });

    sf::Text hp_text(font);
    hp_text.setString("HP: " + std::to_string(player.get_hp()));
    hp_text.setCharacterSize(20);
    hp_text.setFillColor(sf::Color::White);
    hp_text.setPosition({ 50.0f, 20.0f });

    sf::Text mode_text(font);
    mode_text.setString("Mode: " + std::string(player.get_mode() == fight_mode::MELEE ? "MELEE" : "RANGE"));
    mode_text.setCharacterSize(20);
    mode_text.setFillColor(sf::Color::White);
    mode_text.setPosition({ 50.0f, 45.0f });

    sf::Text mana_text(font);
    mana_text.setString("Mana: " + std::to_string(player.get_mana()) + "/" + std::to_string(player.get_max_mana()));
    mana_text.setCharacterSize(20);
    mana_text.setFillColor(sf::Color::Blue);
    mana_text.setPosition({ 50.0f, 70.0f });

    sf::Text turn_text(font);
    turn_text.setString("Turn: Player");
    turn_text.setCharacterSize(20);
    turn_text.setFillColor(sf::Color::Green);
    turn_text.setPosition({ 50.0f, 95.0f });

    sf::Text spell_text(font);
    spell_text.setCharacterSize(16);
    spell_text.setFillColor(sf::Color::White);
    spell_text.setPosition({ 300.0f, 20.0f });

    // Полоска HP
    sf::RectangleShape hp_bar;
    hp_bar.setSize({ 200.0f, 20.0f });
    hp_bar.setFillColor(sf::Color::Green);

    // Полоска маны
    sf::RectangleShape mana_bar;
    mana_bar.setSize({ 200.0f, 15.0f });
    mana_bar.setFillColor(sf::Color::Blue);

    // Индикатор режима атаки
    sf::RectangleShape mode_indicator;
    mode_indicator.setSize({ 30.0f, 30.0f });

    // Индикатор хода
    sf::RectangleShape turn_indicator;
    turn_indicator.setSize({ 30.0f, 30.0f });

    // Текст для режима заклинания
    sf::Text casting_text(font);
    casting_text.setString("Select target for spell");
    casting_text.setCharacterSize(20);
    casting_text.setFillColor(sf::Color::Yellow);
    casting_text.setPosition({ 400.0f, 20.0f });

    // Размеры клеток для отрисовки
    float cell_size = std::min(800.0f / length, 600.0f / width);
    float offset_x = (1000 - length * cell_size) / 2;
    float offset_y = (800 - width * cell_size) / 2;

    // Форма для клеток
    sf::RectangleShape cell_shape;
    cell_shape.setSize({ cell_size - 2, cell_size - 2 });
    cell_shape.setOutlineThickness(1);
    cell_shape.setOutlineColor(sf::Color::Black);

    std::cout << "Game started! Use WASD to move, Q to change attack mode, SPACE to skip your step" << std::endl;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                }
                else if (player_turn && !game_over) {
                    int old_x, old_y;
                    player.get_coords(old_x, old_y);

                    // Обработка хода игрока
                    switch (keyPressed->scancode) {
                        case sf::Keyboard::Scancode::W:
                            player.move('W', board, enemies, tower, attack_tower);
                            break;
                        case sf::Keyboard::Scancode::A:
                            player.move('A', board, enemies, tower, attack_tower);
                            break;
                        case sf::Keyboard::Scancode::S:
                            player.move('S', board, enemies, tower, attack_tower);
                            break;
                        case sf::Keyboard::Scancode::D:
                            player.move('D', board, enemies, tower, attack_tower);
                            break;
                        case sf::Keyboard::Scancode::Q:
                            player.change_attack_mode();
                            std::cout << "Attack mode changed to: "
                                << (player.get_mode() == fight_mode::MELEE ? "MELEE" : "RANGE")
                                << std::endl;
                            break;
                        case sf::Keyboard::Scancode::Space:
                            player_turn = false;
                            std::cout << "Turn skipped" << std::endl;
                            break;
                        case sf::Keyboard::Scancode::Num1:
                        case sf::Keyboard::Scancode::Num2:
                        case sf::Keyboard::Scancode::Num3:
                        case sf::Keyboard::Scancode::Num4:
                        case sf::Keyboard::Scancode::Num5:
                        {
                            int spell_index = static_cast<int>(keyPressed->scancode) -
                                             static_cast<int>(sf::Keyboard::Scancode::Num1);

                            if (spell_index < player_hand.getSpellCount()) {
                                Spell* spell = player_hand.getSpell(spell_index);

                                if (player.get_mana() >= spell->get_mana_cost()) {

                                    if (spell->requires_target()) {
                                        // Заклинание требует выбора цели
                                        casting_spell = true;
                                        selected_spell_index = spell_index;
                                        std::cout << "Select target for " << spell->get_name()
                                        << " (Cost: " << spell->get_mana_cost() << " mana)" << std::endl;
                                    } else {
                                        // Мгновенное заклинание
                                        if (spell->cast(player, board, enemies, allies, attack_tower, tower, 0, 0)) {
                                            std::cout << "Spell cast successfully!" << std::endl;
                                            player_turn = false;
                                            player_hand.removeSpell(spell_index);
                                        } else {
                                            std::cout << "Spell cast failed!" << std::endl;
                                        }
                                    }
                                } else {
                                    std::cout << "Not enough mana! Need: " << spell->get_mana_cost()
                                              << ", Have: " << player.get_mana() << std::endl;
                                }
                            } else {
                                std::cout << "No spell in slot " << (spell_index + 1) << std::endl;
                            }
                            break;
                        }
                        default:
                            break;
                    }

                    // Обновление позиции на доске
                    int new_x, new_y;
                    player.get_coords(new_x, new_y);
                    if (old_x != new_x || old_y != new_y) {
                        board.get_cell(old_x, old_y).set_player(false);
                        board.get_cell(new_x, new_y).set_player(true);
                        std::cout << "Player moved to: " << new_x << ", " << new_y << std::endl;
                    }

                    if (!player.get_can_act()) {
                        player_turn = false;
                        std::cout << "Player turn ended" << std::endl;
                    }
                }
            }
            else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button == sf::Mouse::Button::Left && casting_spell) {
                    // Обработка выбора цели для заклинания
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    int target_x = static_cast<int>((mousePos.x - offset_x) / cell_size);
                    int target_y = static_cast<int>((mousePos.y - offset_y) / cell_size);

                    if (target_x >= 0 && target_x < length && target_y >= 0 && target_y < width) {
                        Spell* spell = player_hand.getSpell(selected_spell_index);

                        std::cout << "Casting spell: " << spell->get_name() << " at (" << target_x << ", " << target_y << ")" << std::endl;

                        if (spell && spell->cast(player, board, enemies, allies, attack_tower, tower, target_x, target_y)) {
                            player_turn = false;
                            std::cout << "Spell cast at (" << target_x << ", " << target_y << ")!" << std::endl;
                            player_hand.removeSpell(selected_spell_index);
                        } else {
                            std::cout << "Spell cast failed!" << std::endl;
                        }
                        casting_spell = false;
                        selected_spell_index = -1;
                    }
                }
            }
        }

        if (!player_turn && !game_over) {
            std::cout << "Enemy turn started" << std::endl;

            // Атака атакующей башни
            if (!attack_tower.is_death()) {
                attack_tower.attack_in_range(player, allies);
            }

            // Спавн новых врагов
            if (!tower.is_death()) {
                int spawn_x, spawn_y;
                if (tower.spawn_enemy(spawn_x, spawn_y, board)) {
                    enemies.emplace_back(spawn_x, spawn_y);
                    board.get_cell(spawn_x, spawn_y).set_enemy(true);
                    std::cout << "New enemy spawned at: " << spawn_x << ", " << spawn_y << std::endl;
                }
            }

            // Движение врагов
            for (auto& enemy : enemies) {
                if (!enemy.is_dead()) {
                    int old_x, old_y;
                    enemy.get_coords(old_x, old_y);

                    enemy.move(player, board, allies);

                    int new_x, new_y;
                    enemy.get_coords(new_x, new_y);

                    // Обновление позиции на доске
                    if (old_x != new_x || old_y != new_y) {
                        board.get_cell(old_x, old_y).set_enemy(false);
                        board.get_cell(new_x, new_y).set_enemy(true);
                        std::cout << "Enemy moved to: " << new_x << ", " << new_y << std::endl;
                    }
                }
            }

            // Движение союзников
            for (auto& ally : allies) {
                if (!ally.is_dead()) {
                    int old_x, old_y;
                    ally.get_coords(old_x, old_y);

                    ally.move(board, enemies, tower, attack_tower);

                    int new_x, new_y;
                    ally.get_coords(new_x, new_y);

                    // Обновление позиции на доске
                    if (old_x != new_x || old_y != new_y) {
                        board.get_cell(old_x, old_y).set_ally(false);
                        board.get_cell(new_x, new_y).set_ally(true);
                    }
                }
            }

            // Удаление мертвых врагов и союзников
            // УДАЛЕНИЕ ВРАГОВ
            auto enemy_it = enemies.begin();
            while (enemy_it != enemies.end()) {
                if (enemy_it->is_dead()) {
                    int x, y;
                    enemy_it->get_coords(x, y);
                    board.get_cell(x, y).set_enemy(false);  // СБРАСЫВАЕМ ФЛАГ
                    enemy_it = enemies.erase(enemy_it);
                } else {
                    ++enemy_it;
                }
            }

            // УДАЛЕНИЕ СОЮЗНИКОВ
            auto ally_it = allies.begin();
            while (ally_it != allies.end()) {
                if (ally_it->is_dead()) {
                    int x, y;
                    ally_it->get_coords(x, y);
                    board.get_cell(x, y).set_ally(false);  // СБРАСЫВАЕМ ФЛАГ
                    ally_it = allies.erase(ally_it);
                } else {
                    ++ally_it;
                }
            }
            // Восстановление маны
            player.set_mana(std::min(player.get_mana() + 2, player.get_max_mana()));

            player_hand.AddRandomSpell();

            // Сброс состояний для следующего хода
            player.reset_step();
            for (auto& enemy : enemies) {
                enemy.reset_turn();
            }
            for (auto& ally : allies) {
                ally.reset_turn();
            }

            player_turn = true;
            std::cout << "Enemy turn ended. Player HP: " << player.get_hp()
                      << ", Mana: " << player.get_mana() << std::endl;
        }
        if (player.is_dead()) {
            game_over = true;
            victory = false;
            std::cout << "GAME OVER! Player died." << std::endl;
        }
        if (tower.is_death() && attack_tower.is_death()) {
            game_over = true;
            victory = true;
            std::cout << "VICTORY! All enemy towers destroyed!" << std::endl;
        }

        window.clear(sf::Color(50, 50, 50));
        // Отрисовка клеток поля
        for (int y = 0; y < width; y++) {
            for (int x = 0; x < length; x++) {
                Cell& cell = board.get_cell(x, y);

                float pos_x = offset_x + x * cell_size;
                float pos_y = offset_y + y * cell_size;

                // Отрисовка фона клетки
                if (cell.is_obstacle_here()) {
                    sprite_obstacle_cell.setPosition({pos_x, pos_y});
                    float scale_x = cell_size / texture_obstacle_cell.getSize().x;
                    float scale_y = cell_size / texture_obstacle_cell.getSize().y;
                    sprite_obstacle_cell.setScale({scale_x, scale_y});
                    window.draw(sprite_obstacle_cell);
                }
                else if (cell.is_slow_here()) {
                    sprite_slow_cell.setPosition({pos_x, pos_y});
                    float scale_x = cell_size / texture_slow_cell.getSize().x;
                    float scale_y = cell_size / texture_slow_cell.getSize().y;
                    sprite_slow_cell.setScale({scale_x, scale_y});
                    window.draw(sprite_slow_cell);
                }
                else {
                    sprite_empty_cell.setPosition({pos_x, pos_y});
                    float scale_x = cell_size / texture_empty_cell.getSize().x;
                    float scale_y = cell_size / texture_empty_cell.getSize().y;
                    sprite_empty_cell.setScale({scale_x, scale_y});
                    window.draw(sprite_empty_cell);
                }

                // Отрисовка ловушек
                if (cell.is_trap_here()) {
                    sprite_trap_cell.setPosition({pos_x, pos_y});
                    float scale_x = cell_size / texture_trap_cell.getSize().x;
                    float scale_y = cell_size / texture_trap_cell.getSize().y;
                    sprite_trap_cell.setScale({scale_x, scale_y});
                    window.draw(sprite_trap_cell);
                }

                // Отрисовка игрока
                if (cell.is_player_here()) {
                    sprite_player.setPosition({pos_x, pos_y});
                    float scale_x = cell_size / texture_player.getSize().x;
                    float scale_y = cell_size / texture_player.getSize().y;
                    sprite_player.setScale({scale_x, scale_y});
                    window.draw(sprite_player);
                }

                // Отрисовка врагов
                if (cell.is_enemy_here()) {
                    sprite_enemy.setPosition({pos_x, pos_y});
                    float scale_x = cell_size / texture_enemy.getSize().x;
                    float scale_y = cell_size / texture_enemy.getSize().y;
                    sprite_enemy.setScale({scale_x, scale_y});
                    window.draw(sprite_enemy);
                }

                // Отрисовка союзников
                if (cell.is_ally_here()) {
                    sprite_ally.setPosition({pos_x, pos_y});
                    float scale_x = cell_size / texture_ally.getSize().x;
                    float scale_y = cell_size / texture_ally.getSize().y;
                    sprite_ally.setScale({scale_x, scale_y});
                    window.draw(sprite_ally);
                }

                // Отрисовка башни-спавнера
                if (cell.is_tower_here() && !tower.is_death()) {
                    sprite_tower.setPosition({pos_x, pos_y});
                    float scale_x = cell_size / texture_tower.getSize().x;
                    float scale_y = cell_size / texture_tower.getSize().y;
                    sprite_tower.setScale({scale_x, scale_y});
                    window.draw(sprite_tower);
                }

                // Отрисовка атакующей башни
                if (cell.is_attack_tower_here() && !attack_tower.is_death()) {
                    sprite_attack_tower.setPosition({pos_x, pos_y});
                    float scale_x = cell_size / texture_attack_tower.getSize().x;
                    float scale_y = cell_size / texture_attack_tower.getSize().y;
                    sprite_attack_tower.setScale({scale_x, scale_y});
                    window.draw(sprite_attack_tower);
                }
            }
        }

        // Обновление UI текста
        hp_text.setString("HP: " + std::to_string(player.get_hp()));
        mana_text.setString("Mana: " + std::to_string(player.get_mana()) + "/" + std::to_string(player.get_max_mana()));
        mode_text.setString("Mode: " + std::string(player.get_mode() == fight_mode::MELEE ? "MELEE" : "RANGE"));

        // Обновление текста очереди хода
        if (player_turn) {
            turn_text.setString("Turn: Player");
            turn_text.setFillColor(sf::Color::Green);
        } else {
            turn_text.setString("Turn: Enemy");
            turn_text.setFillColor(sf::Color::Red);
        }

        // Отображение заклинаний в руке
        std::string spells_info = "Spells:\n";
        for (int i = 0; i < player_hand.getSpellCount(); ++i) {
            Spell* spell = player_hand.getSpell(i);
            if (spell) {
                spells_info += std::to_string(i + 1) + ". " + spell->get_name() +
                              " (Cost: " + std::to_string(spell->get_mana_cost()) + ")\n";
            }
        }
        spell_text.setString(spells_info);

        // Отрисовка UI текста
        window.draw(hp_text);
        window.draw(mana_text);
        window.draw(mode_text);
        window.draw(turn_text);
        window.draw(spell_text);

        // Полоска HP
        float hp_percent = std::max(player.get_hp() / 20.0f, 0.0f);
        float hp_width = 200.0f * hp_percent;
        hp_bar.setSize({ hp_width, 20.0f });
        hp_bar.setPosition({ 50.0f, 120.0f });
        hp_bar.setFillColor(player.get_hp() > 10 ? sf::Color::Green : sf::Color::Red);
        window.draw(hp_bar);

        // Рамка для HP
        sf::RectangleShape hp_frame;
        hp_frame.setSize({ 200.0f, 20.0f });
        hp_frame.setPosition({ 50.0f, 120.0f });
        hp_frame.setFillColor(sf::Color::Transparent);
        hp_frame.setOutlineColor(sf::Color::White);
        hp_frame.setOutlineThickness(1);
        window.draw(hp_frame);

        // Полоска маны
        float mana_percent = std::max(player.get_mana() / static_cast<float>(player.get_max_mana()), 0.0f);
        float mana_width = 200.0f * mana_percent;
        mana_bar.setSize({ mana_width, 15.0f });
        mana_bar.setPosition({ 50.0f, 145.0f });
        mana_bar.setFillColor(sf::Color::Blue);
        window.draw(mana_bar);

        // Рамка для маны
        sf::RectangleShape mana_frame;
        mana_frame.setSize({ 200.0f, 15.0f });
        mana_frame.setPosition({ 50.0f, 145.0f });
        mana_frame.setFillColor(sf::Color::Transparent);
        mana_frame.setOutlineColor(sf::Color::White);
        mana_frame.setOutlineThickness(1);
        window.draw(mana_frame);

        // Индикатор режима атаки
        mode_indicator.setSize({ 30.0f, 30.0f });
        mode_indicator.setPosition({ 50.0f, 165.0f });
        mode_indicator.setFillColor(player.get_mode() == fight_mode::MELEE ? sf::Color::Yellow : sf::Color::Cyan);
        window.draw(mode_indicator);

        // Индикатор хода
        turn_indicator.setSize({ 30.0f, 30.0f });
        turn_indicator.setPosition({ 50.0f, 200.0f });
        turn_indicator.setFillColor(player_turn ? sf::Color::Blue : sf::Color::Red);
        window.draw(turn_indicator);

        // Индикатор режима заклинания
        if (casting_spell) {
            window.draw(casting_text);
        }

        // Сообщение о конце игры
        if (game_over) {
            if (victory) {
                window.draw(victory_text);
            } else {
                window.draw(game_over_text);
            }
        }

        window.display();
    }
    return 0;
}