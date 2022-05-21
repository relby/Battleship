#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include "Types.h"
#include "Ship.h"
#include "Cell.h"

// Colors
#define BG_COLOR sf::Color(60, 60, 60)
#define SHIP_COLOR sf::Color(50, 150, 50)
#define HIT_SHIP_COLOR sf::Color(255, 100, 100)
#define CELL_COLOR sf::Color::White
#define HIT_CELL_COLOR sf::Color::Color(125, 125, 125)
#define CURSOR_COLOR sf::Color::Green

class Game {
private:
    // Window properties
    sf::RenderWindow* window;
    sf::VideoMode videoMode;
    sf::Event event;
    unsigned framerate;
    
    // Player
    std::vector<std::vector<Cell>> field;
    std::vector<std::vector<sf::RectangleShape*>> fieldToRender;
    std::vector<Ship> ships;
    
    // Opponent
    std::vector<std::vector<Cell>> opponentfield;
    std::vector<std::vector<sf::RectangleShape*>> opponentfieldToRender;
    std::vector<Ship> opponentShips;
    // Cursor
    Point cursor;
    // Game info
    size_t selectedShipIndex;
    bool gameEnded;
    sf::Text endGameText;
    sf::Font font;
    // Debug
    bool showOpponentShips;

    //Game stages
    bool placingStage;

    void initVars();
    void initField();
    void initShips();
    void initWindow();

    void moveCursor(Direction dir, std::vector<std::vector<Cell>>& field);
    void setCursor(Point loc);
    void clearRectangleColor(sf::RectangleShape* rectangle);
    Ship& getSelectedShip();
    bool checkProperShipPlacement(Ship& ship, std::vector<Ship>& ships);
    void autoPlaceShips(std::vector<Ship>& ships);
    bool shoot();
    void opponentShoot();
    void checkWin();
public:
    Game();
    ~Game();

    void pollEvents();
    void update();
    void clearFields();
    void updateField();
    void render();
    void renderField();
    bool isRunning();
};