#ifndef WATERPLAYER_H
#define WATERPLAYER_H
#include "player.h"
class WaterPlayer:public Player
{
public:
    WaterPlayer(sf::Vector2f position,sf::Color color);
    void handleInput() override;
    sf::FloatRect getBounds() const;
    void update(sf::Time deltaTime) override;
    void handleCollision(const sf::Sprite& sprite) override;
    void handleCollision(const sf::RectangleShape& platform) override;
    void handleCollision(const sf::FloatRect& objectBounds) override;
};

#endif // WATERPLAYER_H
