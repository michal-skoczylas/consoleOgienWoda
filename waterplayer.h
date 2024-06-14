#ifndef WATERPLAYER_H
#define WATERPLAYER_H
#include "player.h"
class WaterPlayer : public Player
{
public:
  WaterPlayer(sf::Vector2f position, sf::Color color);
  void handleInput() override;
  sf::FloatRect getBounds() const override;
  void update(sf::Time deltaTime) override;
  void handleCollision(const sf::Sprite &sprite) override;
  void handleCollision(const sf::RectangleShape &platform) override;
  void handleCollision(const sf::FloatRect &objectBounds) override;
  void handleLavaCollision(const sf::Sprite &sprite) override;
  void handleWaterCollision(const sf::Sprite &sprite) override;
};

#endif // WATERPLAYER_H
