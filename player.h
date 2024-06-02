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
    void setTexture(const sf::Texture& texture, int frameWidth, int frameHeight);
    sf::Vector2f position;
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
protected:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    sf::Texture texture;
    bool onGround;

    // Animation variables
    int currentFrame = 0;
    int totalFrames = 0;
    float frameDuration = 0.1f; // Duration per frame in seconds
    sf::Clock animationClock;
    int rows, columns;
};

#endif // PLAYER_H
