#include "waterplayer.h"

WaterPlayer::WaterPlayer(sf::Vector2f position, sf::Color color = sf::Color::Blue):Player(position,color) {

}
void WaterPlayer::handleInput(){
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        velocity.x = -200;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        velocity.x = 200;
    } else {
        velocity.x = 0;
    }

    if (onGround && sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        velocity.y = -400;
        onGround = false;
    }
}
