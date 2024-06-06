#include "game.h"
#include "fireplayer.h"
#include "waterplayer.h"
#include "level.h"
#include <iostream>

Game::Game(int windowWidth, int windowHeight)
    : window(sf::VideoMode(windowWidth, windowHeight), "Platformer Game"),
    player1(sf::Vector2f(0, 0), sf::Color::Red),
    player2(sf::Vector2f(0,0), sf::Color::Blue),
    level() {
    window.setFramerateLimit(60);
    level.loadFromFile("D:/gitklony/consoleOgienWoda/level1.txt");
<<<<<<< Updated upstream
    if (!backgroundTexture.loadFromFile("D:/gitklony/consoleOgienWoda/assets/backgroundtheme.png")) {
        std::cout << "Failed to load background image" << std::endl;
    }

    // Set the texture to the sprite
    backgroundSprite.setTexture(backgroundTexture);

     if (!player1Texture.loadFromFile("D:/gitklony/consoleOgienWoda/assets/fire_boy.png") ||
        !player2Texture.loadFromFile("D:/gitklony/consoleOgienWoda/assets/water_girl.png")) {
        std::cout << "Failed to load player images" << std::endl;
    }
    player1.setTexture(player1Texture,8 ,16);
    player2.setTexture(player2Texture,8 ,16);

    // Scale the sprite to match the size of the window
    sf::Vector2u textureSize = backgroundTexture.getSize();
    sf::Vector2u windowSize = window.getSize();
    backgroundSprite.setScale((float) windowSize.x / textureSize.x, (float) windowSize.y / textureSize.y);
=======
>>>>>>> Stashed changes

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
    window.draw(backgroundSprite);
    level.draw(window);
    window.draw(player1);
    window.draw(player2);
    window.display();
}
