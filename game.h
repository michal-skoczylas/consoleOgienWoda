#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include <vector>

#include "SFML/Audio.hpp"
#include "fireplayer.h"
#include "level.h"
#include "player.h"
#include "waterplayer.h"

class Game {
 public:
  // Constructor
  Game(int windowWidth, int windowHeight);
  void run();
  void selectLevel(std::string arglevelPath);
  void saveCompleted();
 private:
  void processEvents();
  void update(sf::Time deltaTime);
  void render();
  void setupTimer();
  sf::Clock finishClock;
  bool FinishClockStarted = false;

  sf::RenderWindow window;
  sf::Texture backgroundTexture;
  sf::Sprite backgroundSprite;
  FirePlayer player1;
  WaterPlayer player2;
  sf::Texture player1Texture;
  sf::Texture player2Texture;
  Level level;
  std::vector<Player*> players = {&player1, &player2};
  std::string levelPath;
  sf::Text endGameText;
  sf::Font endGameFont;
  sf::SoundBuffer endGameSoundBuffer;
  sf::Sound endGameSound;
  bool endGameSoundPlayed = false;
  sf::Clock gameClock;
  sf::Text timerText;
  sf::Font timerFont;
  sf::RectangleShape timerBackground;
  std::string finalTime;
};

#endif  // GAME_H
