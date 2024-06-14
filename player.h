#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
class Player : public sf::Drawable {
 public:
  Player(sf::Vector2f position, sf::Color color);
  virtual void handleInput();
  virtual void update(sf::Time deltaTime);
  virtual sf::FloatRect getBounds() const;
  virtual void handleCollision(const sf::RectangleShape& platform);
  virtual void handleCollision(const sf::FloatRect& objectBounds);
  virtual void handleCollision(const sf::Sprite& sprite);
  void setTexture(const sf::Texture& texture, int frameWidth, int frameHeight);
  sf::Vector2f position;
  void setTargetReached();
  bool getTargetReached();
  void setStartingPosition(sf::Vector2f startingPosition);
  virtual void handleLavaCollision(const sf::Sprite& sprite);
  virtual void handleWaterCollision(const sf::Sprite& sprite);
  void handleAcidCollision(const sf::Sprite& sprite);
void handleSlipperyWallCollision(const sf::Sprite& sprite);
void handleMudCollision(const sf::Sprite& sprite);
  void dead();
float speedBoost=0;
  bool getSlipperyWall();
  void setDeathSound(const std::string& soundPath);
  void normalSpeedSetter();

 private:
  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override;

 protected:
  sf::RectangleShape shape;
  sf::Vector2f velocity;
  sf::Vector2f slipperyVelocity;
  sf::Texture texture;
  bool onGround;
  sf::Vector2f startingPosition;
  sf::SoundBuffer deathBuffer;
  sf::Sound deathSound;
 bool isOnSlipperyWall = false; 
  // Animation variables
  int currentFrame = 0;
  int totalFrames = 0;
  float frameDuration = 0.1f;  // Duration per frame in seconds
  sf::Clock animationClock;
  int rows, columns;
  bool isTargetReached = false;

};

#endif  // PLAYER_H
