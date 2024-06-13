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
}

void Game::run() {
  sf::Clock clock;
  while (window.isOpen()) {
    processEvents();
    sf::Time deltaTime = clock.restart();
    update(deltaTime);
    render();
<<<<<<< Updated upstream
=======

    // Check if the game has finished
    // Replace 'checkGameFinished()' with the appropriate method in your code
     if (level.getLevelFinished() && !gameFinished) {
      // If the game has just finished, store the final time
      finalTime = timerText.getString();
      level.setFinalTime(finalTime);
      gameFinished = true;
      std::cout << "Game finished in " << finalTime << std::endl;
         level.saveBestTime();
         std::cout<<"Best time: "<<level.getFinalTime()<<std::endl;
    }
>>>>>>> Stashed changes
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