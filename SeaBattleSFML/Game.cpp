#include "Game.h"
// Initialization
void Game::initVars() {
    this->placingStage = true;
    this->showOpponentShips = false;
    this->endGameText = sf::Text();
    this->font = sf::Font();
    if (!this->font.loadFromFile("arial.ttf")) {
        std::cout << "NO FONT FILE\n";
        exit(1);
    }
    this->endGameText.setFont(this->font);
    this->endGameText.setPosition(sf::Vector2f(CELL_WIDTH * WIDTH, CELL_HEIGHT * HEIGHT * 1.2));
    this->endGameText.setCharacterSize(24);
    this->endGameText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    this->gameEnded = false;
    this->cursor = Point{ 0, 0 };
    srand(time(0));
}

void Game::initField() {
    this->field = std::vector<std::vector<Cell>>(HEIGHT, std::vector<Cell>(WIDTH, Cell()));
    this->opponentfield = std::vector<std::vector<Cell>>(HEIGHT, std::vector<Cell>(WIDTH, Cell()));
    
    this->field[0][0].setCursor(true);
    for (size_t y = 0; y < HEIGHT; y++) {
        for (size_t x = 0; x < WIDTH; x++) {
            this->field[y][x].setLoc(Point{ x, y });
            this->opponentfield[y][x].setLoc(Point{ x, y });
        }
    }

    this->fieldToRender = std::vector<std::vector<sf::RectangleShape*>>(HEIGHT, std::vector<sf::RectangleShape*>(WIDTH));
    this->opponentfieldToRender = std::vector<std::vector<sf::RectangleShape*>>(HEIGHT, std::vector<sf::RectangleShape*>(WIDTH));
    // player render field
    for (size_t y = 0; y < HEIGHT; y++) {
        for (size_t x = 0; x < WIDTH; x++) {
            sf::RectangleShape* rectangle = new sf::RectangleShape();
            rectangle->setSize(sf::Vector2f(CELL_WIDTH - 5, CELL_HEIGHT - 5));
            rectangle->setPosition(sf::Vector2f((x + 1) * CELL_WIDTH, (y + 1) * CELL_HEIGHT));
            this->clearRectangleColor(rectangle);
            this->fieldToRender[y][x] = rectangle;
    
        }
    }
    // opponent render field
    float offset = WIDTH * CELL_WIDTH + CELL_WIDTH;
    for (size_t y = 0; y < HEIGHT; y++) {
        for (size_t x = 0; x < WIDTH; x++) {
            sf::RectangleShape* rectangle = new sf::RectangleShape();
            rectangle->setSize(sf::Vector2f(CELL_WIDTH - 5, CELL_HEIGHT - 5));
            rectangle->setPosition(sf::Vector2f(offset + (x + 1) * CELL_WIDTH, (y + 1) * CELL_HEIGHT));
            this->clearRectangleColor(rectangle);
            this->opponentfieldToRender[y][x] = rectangle;
        }
    }


}

void Game::initShips() {
    this->selectedShipIndex = 0;
    this->ships = std::vector<Ship>();
    for (size_t i = 4; i >= 1; i--) {
        for (size_t j = i; j <= 4; j++) {
            this->ships.push_back(Ship(Point{ 0, 0 }, i));
            this->opponentShips.push_back(Ship(Point{ 0, 0 }, i));
        }
    }

    autoPlaceShips(this->opponentShips);

    this->ships[this->selectedShipIndex].select();
}

void Game::initWindow() {
    this->framerate = 60;
    this->videoMode.width = 1150;
    this->videoMode.height = 640;
    this->window = new sf::RenderWindow(this->videoMode, "Battleship", sf::Style::Titlebar | sf::Style::Close);
    this->window->setFramerateLimit(this->framerate);
}

// Constructors and Destructor
Game::Game() {
    this->initVars();
    this->initField();
    this->initShips();
    this->initWindow();
}

Game::~Game() {
    for (size_t y = 0; y < HEIGHT; y++) {
        for (size_t x = 0; x < WIDTH; x++) {
            delete this->fieldToRender[y][x];
            delete this->opponentfieldToRender[y][x];
        }
    }
    delete this->window;
}
// Game functions
void Game::moveCursor(Direction dir, std::vector<std::vector<Cell>>& cellField) {
    size_t newY = this->cursor.y;
    size_t newX = this->cursor.x;
    if (dir == Direction::Up && newY > 0) {
        newY -= 1;
    } else if (dir == Direction::Down && newY < HEIGHT - 1) {
        newY += 1;
    } else if (dir == Direction::Left && newX > 0) {
        newX -= 1;
    } else if (dir == Direction::Right && newX < WIDTH - 1) {
        newX += 1;
    }
    cellField[this->cursor.y][this->cursor.x].setCursor(false);
    if (this->placingStage) {
        if (this->getSelectedShip().move(Point{ newX, newY })) {
            this->cursor = Point{ newX, newY };
        }
    } else {
        this->cursor = Point{ newX, newY };
    }
    cellField[this->cursor.y][this->cursor.x].setCursor(true);
}

void Game::setCursor(Point loc)
{
    this->cursor = loc;
}

void Game::clearRectangleColor(sf::RectangleShape* rectangle)
{
    rectangle->setFillColor(CELL_COLOR);
    rectangle->setOutlineColor(sf::Color::Black);
    rectangle->setOutlineThickness(1.f);
}

Ship& Game::getSelectedShip()
{
    for (Ship& ship : this->ships) {
        if (ship.isSelected()) return ship;
    }
    assert(false, "unreachable");
}

bool Game::checkProperShipPlacement(Ship& ship, std::vector<Ship>& shipsToCheck)
{
    std::vector<Point> notAvailableLocations;
    for (Ship& fieldShip : shipsToCheck) {
        if (!fieldShip.isPlaced()) continue;
        for (Cell& cell : fieldShip.getCells()) {
            Point loc = cell.getLoc();
            for (size_t y = 0; y < 3; y++) {
                for (size_t x = 0; x < 3; x++) {
                    notAvailableLocations.push_back(Point{ loc.x - 1 + x, loc.y - 1 + y });
                }
            }
        }
    }
    for (Cell& cell : ship.getCells()) {
        if (std::find_if(notAvailableLocations.begin(), notAvailableLocations.end(), [&] (Point& locToCheck){
                return locToCheck.x == cell.getLoc().x && locToCheck.y == cell.getLoc().y;
            }) != notAvailableLocations.end()) {
            return false;
        }
    }
    return true;
}

void Game::autoPlaceShips(std::vector<Ship>& shipsToPlace)
{
    for (Ship& ship : shipsToPlace) {
        do {
            if (rand() % 2) ship.flip();
            size_t x, y;
            if (ship.isHorizontal()) {
                x = rand() % (WIDTH - ship.getSize() + 1);
                y = rand() % HEIGHT;
            }
            else {
                x = rand() % WIDTH;
                y = rand() % (HEIGHT - ship.getSize() + 1);
            }
            ship.move(Point{ x, y });
        } while (!checkProperShipPlacement(ship, shipsToPlace)); 
        ship.place();
    }
}

bool Game::shoot()
{
    if (this->opponentfield[this->cursor.y][this->cursor.x].isHit()) return false;
    
    this->opponentfield[this->cursor.y][this->cursor.x].setHit(true);
    return true;
}

void Game::opponentShoot()
{
    size_t x = rand() % WIDTH;
    size_t y = rand() % HEIGHT;
    while (this->field[y][x].isHit()) {
        x = rand() % WIDTH;
        y = rand() % HEIGHT;
    }
    this->field[y][x].setHit(true);
}

void Game::checkWin()
{
    bool playerWin = true;
    bool opponentWin = true;
    for (size_t y = 0; y < HEIGHT; y++) {
        for (size_t x = 0; x < WIDTH; x++) {
            if (this->opponentfield[y][x].isShip() && !this->opponentfield[y][x].isHit()) {
                playerWin = false;
            };
            if (this->field[y][x].isShip() && !this->field[y][x].isHit()) {
                opponentWin = false;
            };
        }
    }
    if (playerWin) {
        
        this->endGameText.setString("Player has won");
        this->endGameText.setFillColor(sf::Color::Green);
        this->gameEnded = true;
        return;
    }
    if (opponentWin) {
        this->endGameText.setString("Opponent has won");
        this->endGameText.setFillColor(sf::Color::Red);
        this->gameEnded = true;
        return;
    }

}

void Game::pollEvents() {
    while (this->window->pollEvent(this->event))
    {
        switch (this->event.type)
        {
        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::KeyPressed:
            switch (this->event.key.code) {
            case sf::Keyboard::W:
                if (this->gameEnded) break;
                if (this->placingStage)
                    this->moveCursor(Direction::Up, this->field);
                else
                    this->moveCursor(Direction::Up, this->opponentfield);
                break;
            case sf::Keyboard::S:
                if (this->gameEnded) break;
                if (this->placingStage)
                    this->moveCursor(Direction::Down, this->field);
                else
                    this->moveCursor(Direction::Down, this->opponentfield);
                break;
            case sf::Keyboard::A:
                if (this->gameEnded) break;;
                if (this->placingStage)
                    this->moveCursor(Direction::Left, this->field);
                else
                    this->moveCursor(Direction::Left, this->opponentfield);
                break;
            case sf::Keyboard::D:
                if (this->gameEnded) break;
                if (this->placingStage)
                    this->moveCursor(Direction::Right, this->field);
                else
                    this->moveCursor(Direction::Right, this->opponentfield);
                break;
            case sf::Keyboard::F:
                if (this->gameEnded) break;
                if (this->placingStage) {
                    this->getSelectedShip().flip();
                } else {
                    // TODO: Implement post placing stage
                }
                break;
            case sf::Keyboard::Space:
                if (this->gameEnded) break;
                if (this->placingStage) {
                    bool ok = this->checkProperShipPlacement(this->getSelectedShip(), this->ships);
                    if (!ok) break;
                    this->getSelectedShip().place();
                    if (selectedShipIndex == this->ships.size() - 1) {
                        this->placingStage = false;
                        this->field[this->cursor.y][this->cursor.x].setCursor(false);
                        this->setCursor(Point{ 0, 0 });
                        break;
                    }
                    this->ships[++selectedShipIndex].select();
                    this->setCursor(this->getSelectedShip().getLoc());
                }
                else {
                    if (this->shoot()) {
                        this->opponentShoot();
                        this->checkWin();
                    }
                }
                break;
            case sf::Keyboard::Tilde:
                if (this->gameEnded) break;
                this->showOpponentShips = !this->showOpponentShips;
                break;
            case sf::Keyboard::Escape:
                this->window->close();
            }
            break;
        }
    }
}

void Game::update() {
    this->pollEvents();
    this->updateField();
}

void Game::clearFields()
{
    for (size_t y = 0; y < HEIGHT; y++) {
        for (size_t x = 0; x < WIDTH; x++) {
            this->field[y][x].setCursor(false);
            this->field[y][x].setShip(false);

            this->opponentfield[y][x].setCursor(false);
            this->opponentfield[y][x].setShip(false);
        }
    }
}

void Game::updateField() {
    this->clearFields();
    if (placingStage)
        this->field[this->cursor.y][this->cursor.x].setCursor(true);
    else
        this->opponentfield[this->cursor.y][this->cursor.x].setCursor(true);
    for (Ship ship : this->ships) {
        Point loc = ship.getLoc();
        for (size_t i = 0; i < ship.getSize(); i++) {
            if (!ship.isPlaced() && !ship.isSelected()) continue;
            if (ship.isHorizontal()) {
                this->field[loc.y][loc.x + i].setShip(true);
            } else {
                this->field[loc.y + i][loc.x].setShip(true);
            }
        }
    }

    for (Ship ship : this->opponentShips) {
        Point loc = ship.getLoc();
        for (size_t i = 0; i < ship.getSize(); i++) {
            if (!ship.isPlaced() && !ship.isSelected()) continue;
            if (ship.isHorizontal()) {
                this->opponentfield[loc.y][loc.x + i].setShip(true);
            }
            else {
                this->opponentfield[loc.y + i][loc.x].setShip(true);
            }
        }
    }
}

void Game::render() {
    this->window->clear(BG_COLOR);
    this->renderField();
    this->window->display();
}

void Game::renderField() {
    // Player field to render
    for (size_t y = 0; y < HEIGHT; y++) {
        for (size_t x = 0; x < WIDTH; x++) {
            sf::RectangleShape* rectangle = this->fieldToRender[y][x];
            this->clearRectangleColor(rectangle);
            Cell cell = this->field[y][x];
            if (cell.isCursor()) {
                rectangle->setOutlineColor(CURSOR_COLOR);
                rectangle->setOutlineThickness(3.f);
            }
            if (cell.isHit() && cell.isShip()) {
                rectangle->setFillColor(HIT_SHIP_COLOR);
            } else if (cell.isHit() && !cell.isShip()) {
                rectangle->setFillColor(HIT_CELL_COLOR);
            } else if (cell.isShip()) {
                rectangle->setFillColor(SHIP_COLOR);
            } else {
                rectangle->setFillColor(CELL_COLOR);
            }
        }
    }
    // Opponent field to render
    for (size_t y = 0; y < HEIGHT; y++) {
        for (size_t x = 0; x < WIDTH; x++) {
            sf::RectangleShape* rectangle = this->opponentfieldToRender[y][x];
            this->clearRectangleColor(rectangle);
            Cell cell = this->opponentfield[y][x];
            if (cell.isCursor()) {
                rectangle->setOutlineColor(CURSOR_COLOR);
                rectangle->setOutlineThickness(3.f);
            }
            if (cell.isShip() && cell.isHit()) {
                rectangle->setFillColor(HIT_SHIP_COLOR);
            } else if (!cell.isShip() && cell.isHit()) {
                rectangle->setFillColor(HIT_CELL_COLOR);
            } else if (cell.isShip() && this->showOpponentShips) {
                rectangle->setFillColor(SHIP_COLOR);
            } else {
                rectangle->setFillColor(CELL_COLOR);
            }
        }
    }

    for (size_t y = 0; y < HEIGHT; y++) {
        for (size_t x = 0; x < WIDTH; x++) {
            this->window->draw(*this->fieldToRender[y][x]);
            this->window->draw(*this->opponentfieldToRender[y][x]);
        }
    }
    if (this->gameEnded) {
        this->window->draw(this->endGameText);
    }
}

bool Game::isRunning() {
    return this->window->isOpen();
}