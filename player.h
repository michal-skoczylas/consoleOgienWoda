#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Graphics.hpp>
class Player: public sf::Drawable
{
public:
    Player(sf::Vector2f position, sf::Color color);
    virtual void handleInput();
   void update(sf::Time deltaTime);
    sf::FloatRect getBounds() const;
    void handleCollision(const sf::RectangleShape& platform);
    void handleCollision(const sf::FloatRect& objectBounds);
    void handleCollision(const sf::Sprite& sprite);
    void setTargetReached();
    bool getTargetReached();
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
protected:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    bool onGround;
    bool isTargetReached = false;
};

#endif // PLAYER_H
