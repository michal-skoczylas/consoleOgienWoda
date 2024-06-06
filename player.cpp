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

void Player::handleCollision(const sf::RectangleShape& object) {
    sf::FloatRect objectBounds = object.getGlobalBounds();
    sf::FloatRect playerBounds = shape.getGlobalBounds();

    // Sprawdź, czy gracz koliduje z obiektem z góry
    if (playerBounds.top + playerBounds.height <= objectBounds.top + 10 &&
        playerBounds.top + playerBounds.height > objectBounds.top &&
        playerBounds.left + playerBounds.width > objectBounds.left &&
        playerBounds.left < objectBounds.left + objectBounds.width) {

        shape.setPosition(playerBounds.left, objectBounds.top - playerBounds.height);
        velocity.y = 0;
        onGround = true;
    }
    // Sprawdź, czy gracz koliduje z obiektem z boku (lewej strony)
    else if (playerBounds.left + playerBounds.width > objectBounds.left &&
             playerBounds.left + playerBounds.width < objectBounds.left + 10 &&
             playerBounds.top + playerBounds.height > objectBounds.top &&
             playerBounds.top < objectBounds.top + objectBounds.height) {

        // Obsłuż kolizję z lewej strony, np. zatrzymaj ruch w prawo
        velocity.x = 0;
    }
    // Sprawdź, czy gracz koliduje z obiektem z boku (prawej strony)
    else if (playerBounds.left < objectBounds.left + objectBounds.width &&
             playerBounds.left > objectBounds.left + objectBounds.width - 10 &&
             playerBounds.top + playerBounds.height > objectBounds.top &&
             playerBounds.top < objectBounds.top + objectBounds.height) {

        // Obsłuż kolizję z prawej strony, np. zatrzymaj ruch w lewo
        velocity.x = 0;
    }
}
void Player::handleCollision(const sf::FloatRect& objectBounds) {
    sf::FloatRect playerBounds = shape.getGlobalBounds();

    // Sprawdź, czy gracz koliduje z obiektem z lewej strony
    if (playerBounds.left + playerBounds.width > objectBounds.left &&
        playerBounds.left + playerBounds.width < objectBounds.left + 10 &&
        playerBounds.top + playerBounds.height > objectBounds.top &&
        playerBounds.top < objectBounds.top + objectBounds.height) {

        // Obsłuż kolizję z lewej strony, np. zatrzymaj ruch w prawo
        shape.setPosition(objectBounds.left - playerBounds.width, playerBounds.top);
        velocity.x = 0;
    }
    // Sprawdź, czy gracz koliduje z obiektem z prawej strony
    else if (playerBounds.left < objectBounds.left + objectBounds.width &&
             playerBounds.left > objectBounds.left + objectBounds.width - 10 &&
             playerBounds.top + playerBounds.height > objectBounds.top &&
             playerBounds.top < objectBounds.top + objectBounds.height) {

        // Obsłuż kolizję z prawej strony, np. zatrzymaj ruch w lewo
        shape.setPosition(objectBounds.left + objectBounds.width, playerBounds.top);
        velocity.x = 0;
    }
}
 void Player::handleCollision(const sf::Sprite& sprite) {
    sf::FloatRect playerBounds = shape.getGlobalBounds();
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();

    // Sprawdź, czy gracz koliduje z obiektem z góry
    if (playerBounds.top + playerBounds.height <= spriteBounds.top + 10 &&
        playerBounds.top + playerBounds.height > spriteBounds.top &&
        playerBounds.left + playerBounds.width > spriteBounds.left &&
        playerBounds.left < spriteBounds.left + spriteBounds.width) {

        shape.setPosition(playerBounds.left, spriteBounds.top - playerBounds.height);
        velocity.y = 0;
        onGround = true;
    }
    // Sprawdź, czy gracz koliduje z obiektem z boku (lewej strony)
    else if (playerBounds.left + playerBounds.width > spriteBounds.left &&
             playerBounds.left + playerBounds.width < spriteBounds.left + 10 &&
             playerBounds.top + playerBounds.height > spriteBounds.top &&
             playerBounds.top < spriteBounds.top + spriteBounds.height) {

        // Obsłuż kolizję z lewej strony, np. zatrzymaj ruch w prawo
        velocity.x = 0;
    }
    // Sprawdź, czy gracz koliduje z obiektem z boku (prawej strony)
    else if (playerBounds.left < spriteBounds.left + spriteBounds.width &&
             playerBounds.left > spriteBounds.left + spriteBounds.width - 10 &&
             playerBounds.top + playerBounds.height > spriteBounds.top &&
             playerBounds.top < spriteBounds.top + spriteBounds.height) {

        // Obsłuż kolizję z prawej strony, np. zatrzymaj ruch w lewo
        velocity.x = 0;
    }
    //Sprawdz kolizje od dołu
    else if(playerBounds.top < spriteBounds.top + spriteBounds.height &&
            playerBounds.top > spriteBounds.top + spriteBounds.height - 10 &&
            playerBounds.left + playerBounds.width > spriteBounds.left &&
            playerBounds.left < spriteBounds.left + spriteBounds.width){
        shape.setPosition(playerBounds.left, spriteBounds.top + spriteBounds.height);
        velocity.y = 0;
    }
 }
void Player::setTargetReached() {
    isTargetReached = true;
}

bool Player::getTargetReached() {
    return isTargetReached;
}