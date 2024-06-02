#include "game.h"
#include "fireplayer.h"
#include "waterplayer.h"

Game::Game(int windowWidth, int windowHeight)
    : window(sf::VideoMode(windowWidth, windowHeight), "Platformer Game"),
    player1(sf::Vector2f(0, 0), sf::Color::Red),
    player2(sf::Vector2f(0,0), sf::Color::Blue),
    level() {
    window.setFramerateLimit(60);
    level.loadFromFile("D:/gitklony/consoleOgienWoda/level1.txt");
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        processEvents();
        sf::Time deltaTime = clock.restart();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }

    player1.handleInput();
    player2.handleInput();
}

void Game::update(sf::Time deltaTime) {
    player1.update(deltaTime);
    player2.update(deltaTime);
    level.checkCollisions(player1);
    level.checkCollisions(player2);
    level.chekGoalReached(player1);
    level.chekGoalReached(player2);
}

void Game::render() {
    window.clear();
    level.draw(window);
    window.draw(player1);
    window.draw(player2);
    window.display();
}
