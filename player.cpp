#include "player.h"

Player::Player(sf::Vector2f position, sf::Color color) :velocity(0,0),onGround(false){
    shape.setSize(sf::Vector2f(50,50));
    shape.setPosition(position);
    shape.setFillColor(color);
}

void Player::handleInput(){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
        velocity.x=-200;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
        velocity.x=200;
    }
    else{
        velocity.x=0;
    }
    if(onGround && sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
        velocity.y=-400;
        onGround=false;
    }
}

void Player::update(sf::Time deltaTime){
    velocity.y+=981*deltaTime.asSeconds();//Grawitacja
    shape.move(velocity*deltaTime.asSeconds());

    if(shape.getPosition().y + shape.getSize().y > 600){
        //Kolizja z ziemią
        shape.setPosition(shape.getPosition().x, 600 - shape.getSize().y);
        velocity.y=0;
        onGround=true;
    }

    sf::Vector2f position = shape.getPosition();
    sf::Vector2f size = shape.getSize();

    if (position.x < 0) {
        position.x = 0;
        velocity.x = 0;
    } else if (position.x + size.x > 800) { // assuming window width is 800
        position.x = 800 - size.x;
        velocity.x = 0;
    }

    if (position.y < 0) {
        position.y = 0;
        velocity.y = 0;
    } else if (position.y + size.y > 600) { // assuming window height is 600
        position.y = 600 - size.y;
        velocity.y = 0;
        onGround = true;
    }

    shape.setPosition(position);
}

sf::FloatRect Player::getBounds() const{
    return shape.getGlobalBounds();
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states)const{
    target.draw(shape,states);
}

void Player::handleCollision(const sf::RectangleShape& platform) {
    sf::FloatRect platformBounds = platform.getGlobalBounds();
    sf::FloatRect playerBounds = shape.getGlobalBounds();

    // Check if player is falling and collides from above
    if (playerBounds.top + playerBounds.height <= platformBounds.top + 10 &&
        playerBounds.top + playerBounds.height > platformBounds.top &&
        playerBounds.left + playerBounds.width > platformBounds.left &&
        playerBounds.left < platformBounds.left + platformBounds.width) {

        shape.setPosition(playerBounds.left, platformBounds.top - playerBounds.height);
        velocity.y = 0;
        onGround = true;
    }
}
