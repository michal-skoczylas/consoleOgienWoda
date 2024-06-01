#include "fireplayer.h"

FirePlayer::FirePlayer(sf::Vector2f position, sf::Color color=sf::Color::Red):Player(position,color) {

}

void FirePlayer::handleInput(){
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
