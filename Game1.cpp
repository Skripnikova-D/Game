#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>
#include <vector>

#include "Cell.hpp"
#include "Board.hpp" 
#include "Player.hpp"
#include "Enemy.hpp"
#include "EnemyTower.hpp"
#include "Ally.hpp"

int main()
{
    int length, width = 0;
    std::cout << "Please enter the board size. "
                 "\nThe width and length must not be less than 10 and more than 25, \n otherwise a 10x10 board will be created." << std::endl;
    std::cin >> length >> width;

    Board board(length, width);
    length = board.get_length();
    width = board.get_width();
    EnemyTower tower(length - 1, width - 1);
    board.get_cell(length - 1, width - 1).set_obstacle(true);
    Player player;

    board.get_cell(0, 0).set_player(true);
    player.set_position(0, 0);

    std::vector<Ally> allies;
    std::vector<Enemy> enemies;
    if (!board.get_cell(length - 2, width - 1).is_obstacle_here()) {
        Enemy enemy1(length - 2, width - 1);
        enemies.push_back(enemy1);
        board.get_cell(length - 2, width - 1).set_enemy(true);
    }
    else if (!board.get_cell(length - 1, width - 2).is_obstacle_here()) {
        Enemy enemy1(length - 1, width - 2);
        enemies.push_back(enemy1);
        board.get_cell(length - 2, width - 1).set_enemy(true);
    }

    bool player_turn = true;
    bool game_over = false;

    // Настройка графики
    sf::RenderWindow window(sf::VideoMode({ 1000, 800 }), "OOP_GAME", sf::Style::Titlebar | sf::Style::Close);
    // Загрузка текстур
    sf::Texture texture_player;
    sf::Texture texture_enemy;
    sf::Texture texture_obstacle_cell;
    sf::Texture texture_empty_cell;
    sf::Texture texture_tower;
    sf::Texture texture_slow_cell;

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


    if (!texture_empty_cell.loadFromFile("assets/empty_cell.png")) {
        std::cout << "Failed to load empty_cell.png" << std::endl;
        return -1;
    }else {
        std::cout<<"Success load empty_cell.png"<<std::endl;
    }

    if (!texture_tower.loadFromFile("assets/tower.png")) {
        std::cout << "Failed to load tower.png" << std::endl;
        return -1;
    }else {
        std::cout<<"Success load tower.png"<<std::endl;
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

    // Загрузка шрифта
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

    texture_player.setSmooth(true);
    texture_enemy.setSmooth(true);
    texture_empty_cell.setSmooth(true);
    texture_slow_cell.setSmooth(true);
    texture_tower.setSmooth(true);
    texture_obstacle_cell.setSmooth(true);

    // Создание спрайтов
    sf::Sprite sprite_player(texture_player);
    sf::Sprite sprite_enemy(texture_enemy);
    sf::Sprite sprite_empty_cell(texture_empty_cell);
    sf::Sprite sprite_slow_cell(texture_slow_cell);
    sf::Sprite sprite_tower(texture_tower);
    sf::Sprite sprite_obstacle_cell(texture_obstacle_cell);

    // Создание текста
    sf::Text game_over_text(font);
    game_over_text.setString("GAME OVER");
    game_over_text.setCharacterSize(50);
    game_over_text.setFillColor(sf::Color::Red);
    game_over_text.setPosition({ 400.0f, 350.0f }); // Позиция по центру

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


    // Размеры клеток для отрисовки
    float cell_size = std::min(800.0f / length, 600.0f / width);
    float offset_x = (1000 - length * cell_size) / 2;
    float offset_y = (800 - width * cell_size) / 2;

    // Форма для клеток
    sf::RectangleShape cell_shape;
    cell_shape.setSize({ cell_size - 2, cell_size - 2 });
    cell_shape.setOutlineThickness(1);
    cell_shape.setOutlineColor(sf::Color::Black);

    // Фигуры для UI информации
    sf::RectangleShape hp_bar;
    sf::RectangleShape mode_indicator;
    sf::RectangleShape turn_indicator;

    // Фигуры для игровых объектов (вместо спрайтов)
    sf::CircleShape player_circle(cell_size / 3);
    player_circle.setFillColor(sf::Color::Blue);

    sf::CircleShape enemy_circle(cell_size / 4);
    enemy_circle.setFillColor(sf::Color::Red);

    sf::RectangleShape tower_rect;
    tower_rect.setSize({ cell_size - 4, cell_size - 4 });
    tower_rect.setFillColor(sf::Color::Magenta);

    std::cout << "Game started! Use WASD to move, Q to change attack mode, SPACE to skip your step" << std::endl;

    while (window.isOpen())
    {
        // Обработка событий
        while (const std::optional event = window.pollEvent())
        {
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
                        player.move('W', board,enemies);
                        break;
                    case sf::Keyboard::Scancode::A:
                        player.move('A', board,enemies);
                        break;
                    case sf::Keyboard::Scancode::S:
                        player.move('S', board,enemies);
                        break;
                    case sf::Keyboard::Scancode::D:
                        player.move('D', board,enemies);
                        break;
                    case sf::Keyboard::Scancode::Q:
                        player.change_attack_mode();
                        std::cout << "Attack mode changed to: "
                            << (player.get_mode() == fight_mode::MELEE ? "MELEE" : "RANGE")
                            << std::endl;
                        break;
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

                    // Если игрок сделал ход, завершаем его turn
                    if (!player.get_can_act()) {
                        player_turn = false;
                        std::cout << "Player turn ended" << std::endl;
                    }
                }
            }
        }

        // Ход врагов
        if (!player_turn && !game_over) {
            std::cout << "Enemy turn started" << std::endl;

            // Спавн новых врагов
            int spawn_x, spawn_y;
            if (tower.spawn_enemy(spawn_x, spawn_y, board)) {
                enemies.emplace_back(spawn_x, spawn_y);//создае сразу в векторе
                board.get_cell(spawn_x, spawn_y).set_enemy(true);
                std::cout << "New enemy spawned at: " << spawn_x << ", " << spawn_y << std::endl;
            }

            // Движение врагов
            for (auto& enemy : enemies) {
                if (!enemy.is_dead()) {
                    int old_x, old_y;
                    enemy.get_coords(old_x, old_y);

                    enemy.move(player, board,allies);

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

            // Удаление мертвых врагов

            // Сброс состояний для следующего хода
            player.reset_step();
            for (auto& enemy : enemies) {
                enemy.reset_turn();
            }

            player_turn = true;
            std::cout << "Enemy turn ended. Player HP: " << player.get_hp() << std::endl;
        }

        // Проверка состояния игры
        if (player.is_dead()) {
            game_over = true;
            std::cout << "GAME OVER! Player died." << std::endl;
        }

        // Отрисовка
        window.clear(sf::Color(50, 50, 50)); // Темный фон для контраста

        // Отрисовка клеток поля10 10
        for (int y = 0; y < width; y++) {
            for (int x = 0; x < length; x++) {
                Cell& cell = board.get_cell(x, y);

                // Установка позиции клетки
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
            }
        }

        // Отрисовка башни
        sprite_tower.setPosition({offset_x + (length - 1) * cell_size,
                                 offset_y + (width - 1) * cell_size});
        float scale_x = cell_size / texture_tower.getSize().x;
        float scale_y = cell_size / texture_tower.getSize().y;
        sprite_tower.setScale({scale_x, scale_y});
        window.draw(sprite_tower);

        // Обновление текста
        hp_text.setString("HP: " + std::to_string(player.get_hp()));
        mode_text.setString("Mode: " + std::string(player.get_mode() == fight_mode::MELEE ? "MELEE" : "RANGE"));

        // Отрисовка UI
        window.draw(hp_text);
        window.draw(mode_text);

        // Полоска HP
        float hp_percent = std::max(player.get_hp() / 20.0f, 0.0f); // предполагая max HP = 20
        float hp_width = 200.0f * hp_percent;
        hp_bar.setSize({ hp_width, 20.0f });
        hp_bar.setPosition({ 50.0f, 50.0f });
        hp_bar.setFillColor(player.get_hp() > 10 ? sf::Color::Green : sf::Color::Red);
        window.draw(hp_bar);

        // Рамка для HP
        sf::RectangleShape hp_frame;
        hp_frame.setSize({ 200.0f, 20.0f });
        hp_frame.setPosition({ 50.0f, 50.0f });
        hp_frame.setFillColor(sf::Color::Transparent);
        hp_frame.setOutlineColor(sf::Color::White);
        hp_frame.setOutlineThickness(1);
        window.draw(hp_frame);

        // Индикатор режима атаки
        mode_indicator.setSize({ 30.0f, 30.0f });
        mode_indicator.setPosition({ 50.0f, 80.0f });
        mode_indicator.setFillColor(player.get_mode() == fight_mode::MELEE ? sf::Color::Yellow : sf::Color::Cyan);
        window.draw(mode_indicator);

        // Индикатор хода
        turn_indicator.setSize({ 30.0f, 30.0f });
        turn_indicator.setPosition({ 50.0f, 120.0f });
        turn_indicator.setFillColor(player_turn ? sf::Color::Blue : sf::Color::Red);
        window.draw(turn_indicator);

        // Сообщение о конце игры
        if (game_over) {
            window.draw(game_over_text);
        }

        window.display();
    }

    return 0;
}