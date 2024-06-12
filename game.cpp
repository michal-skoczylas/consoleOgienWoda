#include "game.h"

#include <iostream>

#include "fireplayer.h"
#include "level.h"
#include "waterplayer.h"

Game::Game(int windowWidth, int windowHeight)
    : window(sf::VideoMode(windowWidth, windowHeight), "Platformer Game"),
      player1(sf::Vector2f(0, 0), sf::Color::White),
      player2(sf::Vector2f(0, 0), sf::Color::White),
      level() {
  window.setFramerateLimit(60);
  // this->selectLevel(levelPath);
  // level.loadFromFile(levelPath);
  if (!backgroundTexture.loadFromFile("assets/backgroundtheme.png")) {
    std::cout << "Failed to load background image" << std::endl;
  }
  // Ustawienie endGameSound

  // Set the texture to the sprite
  backgroundSprite.setTexture(backgroundTexture);

  if (!player1Texture.loadFromFile("assets/fire_boy.png") ||
      !player2Texture.loadFromFile("assets/water_girl.png")) {
    std::cout << "Failed to load player images" << std::endl;
  }
  player1.setTexture(player1Texture, 8, 16);
  player2.setTexture(player2Texture, 8, 16);

  // Scale the sprite to match the size of the window
  sf::Vector2u textureSize = backgroundTexture.getSize();
  sf::Vector2u windowSize = window.getSize();
  backgroundSprite.setScale((float)windowSize.x / textureSize.x,
                            (float)windowSize.y / textureSize.y);

  // Set the end game text
  if (!endGameFont.loadFromFile("assets/Robus.otf")) {
    std::cout << "Failed to load font" << std::endl;
  } else {
    endGameText.setFont(endGameFont);
  }

  endGameText.setString("Game Finished");
  endGameText.setCharacterSize(150);
  endGameText.setFillColor(sf::Color::White);
  endGameText.setPosition(
      window.getSize().x / 2 - endGameText.getGlobalBounds().width / 2,
      window.getSize().y / 2 - endGameText.getGlobalBounds().height / 2);
  endGameText.setStyle(sf::Text::Bold);
  if (!endGameSoundBuffer.loadFromFile("assets/levelEnd.mp3")) {
    std::cout << "Failed to load end game sound" << std::endl;
  }
  // else{
  //   std::cerr<<"Loaded end game sound"<<std::endl;
  // }
  endGameSound.setBuffer(endGameSoundBuffer);

  if (!timerFont.loadFromFile("assets/arial.ttf")) {
    std::cout << "Failed to load timer font" << std::endl;
} else {
    timerText.setFont(timerFont);
}

timerText.setString("0:00");
timerText.setCharacterSize(30);
timerText.setFillColor(sf::Color::Black);
timerText.setPosition(window.getSize().x - timerText.getGlobalBounds().width, 0);

sf::RectangleShape timerBackground;
timerBackground.setFillColor(sf::Color::Black);  // Change this to the color you want
timerBackground.setOutlineThickness(0.1);  // Increase the outline thickness
timerBackground.setOutlineColor(sf::Color::White);  // Set the outline color to a contrasting color
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        processEvents();
        sf::Time deltaTime = clock.restart();
        update(deltaTime);

        // Update the timerText string before rendering
        sf::Time elapsed = gameClock.getElapsedTime();
        int minutes = elapsed.asSeconds() / 60;
        int seconds = (int)elapsed.asSeconds() % 60;
        int milliseconds = elapsed.asMilliseconds() % 1000;  // Get the remaining milliseconds
        timerText.setString(std::to_string(minutes) + ":" + 
                            (seconds < 10 ? "0" : "") + std::to_string(seconds) + ":" + 
                            (milliseconds < 100 ? (milliseconds < 10 ? "00" : "0") : "") + std::to_string(milliseconds));

        // Update the position of the timerText
        timerText.setPosition(window.getSize().x - timerText.getGlobalBounds().width, 0);
        timerBackground.setSize(sf::Vector2f(timerText.getGlobalBounds().width + 15, timerText.getGlobalBounds().height + 10));  // Increase the size as needed
        timerBackground.setPosition(timerText.getPosition().x - 2, timerText.getPosition().y+2);  // Subtract 10 (or any other amount) from the x-coordinate


        render();
    }
}

void Game::processEvents() {
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) window.close();
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
  level.checkEndGame(players);
}

void Game::render() {
  window.clear();
  window.draw(backgroundSprite);
  level.draw(window);
  window.draw(timerBackground);
  window.draw(timerText);
  window.draw(player1);
  window.draw(player2);
  if (level.getLevelFinished()) {
        window.draw(endGameText);
    if (endGameSoundPlayed == false) {
      endGameSound.play();
      endGameSoundPlayed = true;
    }
  }
  window.display();
}
void Game::selectLevel(std::string arglevelPath) {
  this->levelPath = arglevelPath;
  level.loadFromFile(arglevelPath);

  // setup player starting positions
  // dodanie wektora 0,-25 powoduje ze postacie nie zapadaja sie pod ziemie po
  // zrespieniu
  player1.setStartingPosition(level.getStartingPosition() +
                              sf::Vector2f(0, -25));
  player2.setStartingPosition(level.getStartingPosition() +
                              sf::Vector2f(0, -25));
  //
  if (level.getFireStartingPosition() != sf::Vector2f(0, 0)) {
    player1.setStartingPosition(level.getFireStartingPosition() +
                                sf::Vector2f(0, -25));
  }
  if (level.getWaterStartingPosition() != sf::Vector2f(0, 0)) {
    player2.setStartingPosition(level.getWaterStartingPosition() +
                                sf::Vector2f(0, -25));
  }
}