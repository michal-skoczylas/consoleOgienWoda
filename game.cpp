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
  // Load background texture
  if (!backgroundTexture.loadFromFile("assets/backgroundtheme.png")) {
    std::cerr << "Failed to load background image" << std::endl;
  } else {
    backgroundSprite.setTexture(backgroundTexture);
    // Scale the sprite to match the size of the window
    sf::Vector2u textureSize = backgroundTexture.getSize();
    sf::Vector2u windowSize = window.getSize();
    backgroundSprite.setScale(
      static_cast<float>(windowSize.x) / textureSize.x,
      static_cast<float>(windowSize.y) / textureSize.y
    );
  }

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

  // Load end game text
  if (!endGameFont.loadFromFile("assets/Robus.otf")) {
    std::cerr << "Failed to load font" << std::endl;
  } else {
    endGameText.setFont(endGameFont);
    endGameText.setString("Game Finished");
    endGameText.setCharacterSize(150);
    endGameText.setFillColor(sf::Color::White);
    endGameText.setPosition(
      window.getSize().x / 2 - endGameText.getGlobalBounds().width / 2,
      window.getSize().y / 2 - endGameText.getGlobalBounds().height / 2
    );
    endGameText.setStyle(sf::Text::Bold);
  }
  // Load end game sound
  if (!endGameSoundBuffer.loadFromFile("assets/levelEnd.mp3")) {
    std::cerr << "Failed to load end game sound" << std::endl;
  } else {
    endGameSound.setBuffer(endGameSoundBuffer);
  }

  // Set up timer
  setupTimer();
}

// Set up the timer text and background
  void Game::setupTimer() {
  if (!timerFont.loadFromFile("assets/arial.ttf")) {
    std::cerr << "Failed to load timer font" << std::endl;
  } else {
    // Set up the timer text
    timerText.setFont(timerFont);
    timerText.setString("0:00:000");
    timerText.setCharacterSize(30);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(window.getSize().x - timerText.getGlobalBounds().width, 0);
  }

  // Set up the timer background
  timerBackground.setFillColor(sf::Color::Black);  // Background color
  timerBackground.setOutlineThickness(1);  // Outline thickness
  timerBackground.setOutlineColor(sf::Color::White);  // Outline color
  timerBackground.setSize(sf::Vector2f(timerText.getGlobalBounds().width + 15, timerText.getGlobalBounds().height + 10));  // Background size
  timerBackground.setPosition(timerText.getPosition().x - 5, timerText.getPosition().y - 5);  // Background position
}

void Game::run() {
  sf::Clock clock;
  while (window.isOpen()) {
    processEvents();
    sf::Time deltaTime = clock.restart();
    update(deltaTime);

    // Update the timerText string before rendering
    sf::Time elapsed = gameClock.getElapsedTime();
    int minutes = static_cast<int>(elapsed.asSeconds()) / 60;
    int seconds = static_cast<int>(elapsed.asSeconds()) % 60;
    int milliseconds = elapsed.asMilliseconds() % 1000;

    // Update the timer text
    timerText.setString(
      std::to_string(minutes) + ":" + 
      (seconds < 10 ? "0" : "") + std::to_string(seconds) + ":" + 
      (milliseconds < 100 ? "0" : "") + (milliseconds < 10 ? "0" : "") + std::to_string(milliseconds)
    );

    // Update the position and size of the timer background
    timerText.setPosition(window.getSize().x - timerText.getGlobalBounds().width - 10, 10);
    timerBackground.setSize(sf::Vector2f(timerText.getGlobalBounds().width + 10, timerText.getGlobalBounds().height + 5));
    timerBackground.setPosition(timerText.getPosition().x - 5, timerText.getPosition().y+5);

    // Render the game
    render();
  }
}

void Game::processEvents() {
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      window.close();
    } 
  }

  // Handle player input
  player1.handleInput();
  player2.handleInput();
}

void Game::update(sf::Time deltaTime) {
  // Update the players and check for collisions
  player1.update(deltaTime);
  player2.update(deltaTime);
  level.checkCollisions(player1);
  level.checkCollisions(player2);
  level.chekGoalReached(player1);
  level.chekGoalReached(player2);
  // Check if the level is finished
  level.checkEndGame(players);
}

void Game::render() {
  // Clear the window
  window.clear();
  // Draw the game elements
  window.draw(backgroundSprite);
  level.draw(window);
  window.draw(timerBackground);
  window.draw(timerText);
  window.draw(player1);
  window.draw(player2);
  // Draw the end game text if the level is finished
  if (level.getLevelFinished()) {
    window.draw(endGameText);
    if (!endGameSoundPlayed) {
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