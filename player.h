#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Graphics.hpp>
class Player: public sf::Drawable
{
public:
    Player(sf::Vector2f position, sf::Color color);
    virtual void handleInput();
   virtual void update(sf::Time deltaTime);
    virtual sf::FloatRect getBounds() const;
    virtual void handleCollision(const sf::RectangleShape& platform);
    virtual void handleCollision(const sf::FloatRect& objectBounds);
   virtual  void handleCollision(const sf::Sprite& sprite);
    void setTexture(const sf::Texture& texture, int frameWidth, int frameHeight);
    sf::Vector2f position;
    void setTargetReached();
    bool getTargetReached();
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
    bool isTargetReached = false;
};

#endif // PLAYER_H
