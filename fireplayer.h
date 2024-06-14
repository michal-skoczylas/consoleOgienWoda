#ifndef FIREPLAYER_H
#define FIREPLAYER_H
#include "player.h"
class FirePlayer : public Player
{
public:
  // Konstruktor klasy FirePlayer
  FirePlayer(sf::Vector2f position, sf::Color color);
  void handleInput() override;
  sf::FloatRect getBounds() const override;
  void update(sf::Time deltaTime) override;
  // funkcje obslugujace kolizje
  void handleCollision(const sf::Sprite &sprite) override;
  void handleCollision(const sf::RectangleShape &platform) override;
  void handleCollision(const sf::FloatRect &objectBounds) override;
  void handleLavaCollision(const sf::Sprite &sprite) override;
  void handleWaterCollision(const sf::Sprite &sprite) override;
  // funkcje obslugujace interakcje ze Sliskimi scianami
  bool isOnSlipperyWall = false;
  void setSlipperyWall(bool slipperyWall);
};

#endif // FIREPLAYER_H
