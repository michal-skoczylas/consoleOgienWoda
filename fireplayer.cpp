#include "fireplayer.h"

#include <iostream>

FirePlayer::FirePlayer(sf::Vector2f position, sf::Color color=sf::Color::Red):Player(position,color) {

}

void FirePlayer::handleInput(){
    
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
  
    velocity.x = -200-speedBoost;
 }
   else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    
      velocity.x = 200+speedBoost;
    }
   else {
    velocity.x = 0;
  }
  if (onGround && sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
    velocity.y = -400;
    onGround = false;
  }
}

void FirePlayer::update(sf::Time deltaTime){

    velocity.y += 981 * deltaTime.asSeconds(); // Gravity
    shape.move(velocity * deltaTime.asSeconds());
    

    if(shape.getPosition().y + shape.getSize().y > 600){
        // Collision with ground
        velocity.y = 0;
        onGround = true;
    }

    // Animation update
    float elapsedSeconds = animationClock.getElapsedTime().asSeconds();
    if (elapsedSeconds >= frameDuration) {
        // Move to the next frame
        int row, column;
    int frameWidth = texture.getSize().x / 7; // Total width divided by number of columns
    int frameHeight = texture.getSize().y / 5; // Total height divided by number of rows
    sf::IntRect textureRect;

    if (velocity.x > 0) { // Player is moving right
        currentFrame = (currentFrame + 1) % 5; // There are 5 frames for walking right
        row = 0; // 1st row (0-indexed) for walking right
        column = currentFrame; // Columns 0-4 for walking right
        shape.setScale(1.f, 1.f); // Reset the scale to default
        int offsetX = 0;
    int offsetY = -20;// Adjust this value as needed
    textureRect = sf::IntRect(column * frameWidth + offsetX, row * frameHeight + offsetY, frameWidth, frameHeight);
    } else if (velocity.x < 0) { // Player is moving left
    currentFrame = (currentFrame + 1) % 5; // There are 5 frames for walking left
    row = 1; // 2nd row (0-indexed) for walking left
    column = currentFrame; // Columns 0-4 for walking left
    shape.setScale(1.f, 1.f); // Flip the texture horizontally
    int offsetX = 14;
    int offsetY = -20;// Adjust this value as needed
    textureRect = sf::IntRect(column * frameWidth + offsetX, row * frameHeight + offsetY, frameWidth, frameHeight);
} else { // Player is not moving
        currentFrame = (currentFrame + 1) % 6; // There are 6 frames for standing still
        row = 4; // 5th row (0-indexed) for standing still
        column = currentFrame; // Columns 0-5 for standing still
        shape.setScale(1.f, 1.f); // Reset the scale to default
        textureRect = sf::IntRect(column * frameWidth, row * frameHeight, frameWidth, frameHeight);
    }

    shape.setTextureRect(textureRect);
    animationClock.restart();
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
    }

    shape.setPosition(position);
} 

sf::FloatRect FirePlayer::getBounds() const {
    return shape.getGlobalBounds();
}

void FirePlayer::handleCollision(const sf::Sprite& sprite){
    sf::FloatRect playerBounds = shape.getGlobalBounds();
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    if (playerBounds.intersects(spriteBounds)) {
        // Collision occurred
        sf::Vector2f playerPosition = shape.getPosition();
        sf::Vector2f spritePosition = sprite.getPosition();
        sf::Vector2f overlap;
        if (playerPosition.x < spritePosition.x) {
            // Player is to the left of the sprite
            overlap.x = playerBounds.left + playerBounds.width - spriteBounds.left;
        } else {
            // Player is to the right of the sprite
            overlap.x = spriteBounds.left + spriteBounds.width - playerBounds.left;
        }
        if (playerPosition.y < spritePosition.y) {
            // Player is above the sprite
            overlap.y = playerBounds.top + playerBounds.height - spriteBounds.top;
        } else {
            // Player is below the sprite
            overlap.y = spriteBounds.top + spriteBounds.height - playerBounds.top;
        }
        if (overlap.x < overlap.y) {
            // Resolve the collision horizontally
            if (playerPosition.x < spritePosition.x) {
                // Move the player to the left
                shape.move(-overlap.x, 0);
            } else {
                // Move the player to the right
                shape.move(overlap.x, 0);
            }
        } else {
            // Resolve the collision vertically
            if (playerPosition.y < spritePosition.y) {
                // Move the player up
                shape.move(0, -overlap.y);
                velocity.y = 0;
                onGround = true;
            } else {
                // Move the player down
                shape.move(0, overlap.y);
                velocity.y = 0;
            }
        }
    }
}

void FirePlayer::handleCollision(const sf::RectangleShape& platform){
    sf::FloatRect playerBounds = shape.getGlobalBounds();
    sf::FloatRect platformBounds = platform.getGlobalBounds();
    if (playerBounds.intersects(platformBounds)) {
        // Collision occurred
        sf::Vector2f playerPosition = shape.getPosition();
        sf::Vector2f platformPosition = platform.getPosition();
        sf::Vector2f overlap;
        if (playerPosition.x < platformPosition.x) {
            // Player is to the left of the platform
            overlap.x = playerBounds.left + playerBounds.width - platformBounds.left;
        } else {
            // Player is to the right of the platform
            overlap.x = platformBounds.left + platformBounds.width - playerBounds.left;
        }
        if (playerPosition.y < platformPosition.y) {
            // Player is above the platform
            overlap.y = playerBounds.top + playerBounds.height - platformBounds.top;
        } else {
            // Player is below the platform
            overlap.y = platformBounds.top + platformBounds.height - playerBounds.top;
        }
        if (overlap.x < overlap.y) {
            // Resolve the collision horizontally
            if (playerPosition.x < platformPosition.x) {
                // Move the player to the left
                shape.move(-overlap.x, 0);
            } else {
                // Move the player to the right
                shape.move(overlap.x, 0);
            }
        } else {
            // Resolve the collision vertically
            if (playerPosition.y < platformPosition.y) {
                // Move the player up
                shape.move(0, -overlap.y);
                velocity.y = 0;
                onGround = true;
            } else {
                // Move the player down
                shape.move(0, overlap.y);
                velocity.y = 0;
            }
        }
    }
}

void FirePlayer::handleCollision(const sf::FloatRect& objectBounds){
    sf::FloatRect playerBounds = shape.getGlobalBounds();
    if (playerBounds.intersects(objectBounds)) {
        // Collision occurred
        sf::Vector2f playerPosition = shape.getPosition();
        sf::Vector2f objectPosition(objectBounds.left, objectBounds.top);
        sf::Vector2f overlap;
        if (playerPosition.x < objectPosition.x) {
            // Player is to the left of the object
            overlap.x = playerBounds.left + playerBounds.width - objectBounds.left;
        } else {
            // Player is to the right of the object
            overlap.x = objectBounds.left + objectBounds.width - playerBounds.left;
        }
        if (playerPosition.y < objectPosition.y) {
            // Player is above the object
            overlap.y = playerBounds.top + playerBounds.height - objectBounds.top;
        } else {
            // Player is below the object
            overlap.y = objectBounds.top + objectBounds.height - playerBounds.top;
        }
        if (overlap.x < overlap.y) {
            // Resolve the collision horizontally
            if (playerPosition.x < objectPosition.x) {
                // Move the player to the left
                shape.move(-overlap.x, 0);
            } else {
                // Move the player to the right
                shape.move(overlap.x, 0);
            }
        } else {
            // Resolve the collision vertically
            if (playerPosition.y < objectPosition.y) {
                // Move the player up
                shape.move(0, -overlap.y);
                velocity.y = 0;
                onGround = true;
            } else {
                // Move the player down
                shape.move(0, overlap.y);
                velocity.y = 0;
            }
        }
    }
}
void FirePlayer::handleLavaCollision(const sf::Sprite& sprite){
    sf::FloatRect playerBounds = shape.getGlobalBounds();
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    if (playerBounds.intersects(spriteBounds)) {
        // Collision occurred
        // Handle lava collision
        handleCollision(sprite);
    }
}
void FirePlayer::handleWaterCollision(const sf::Sprite& sprite){
    sf::FloatRect playerBounds = shape.getGlobalBounds();
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    if (playerBounds.intersects(spriteBounds)) {
        // Collision occurred
        // Handle water collision
    setDeathSound("assets/death.wav");
    deathSound.play();
        dead();
    }
}


void FirePlayer::setSlipperyWall(bool slipperyWall){
    isOnSlipperyWall = slipperyWall;
}
