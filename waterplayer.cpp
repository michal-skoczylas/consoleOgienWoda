#include "waterplayer.h"
#include <iostream>
WaterPlayer::WaterPlayer(sf::Vector2f position, sf::Color color = sf::Color::Blue):Player(position,color) {
    //Dzwieki gracz
    if(!jumpBuffer.loadFromFile("assets/jump_sound.mp3")){
        std::cerr << "Failed to load jump sound" << std::endl;
    }
    jumpSound.setBuffer(jumpBuffer);
    jumpSound.setVolume(50);
}
void WaterPlayer::handleInput(){
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {

    velocity.x = -200-speedBoost;

  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {

      velocity.x = 200+speedBoost;

  } else {
    velocity.x = 0;
  }
  if (onGround && sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    velocity.y = -400;
    onGround = false;
    jumpSound.play();
  }
}

void WaterPlayer::update(sf::Time deltaTime){
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
    int frameWidth = texture.getSize().x / 5; // Adjust if WaterPlayer PNG has different number of columns
    int frameHeight = texture.getSize().y / 5; // Adjust if WaterPlayer PNG has different number of rows
    sf::IntRect textureRect;

    if (velocity.x > 0) { // Player is moving right
    currentFrame = (currentFrame + 1) % 5; // There are 5 frames for walking right
    row = 1; // 1st row (0-indexed) for walking right
    column = currentFrame; // Columns 0-4 for walking right
    shape.setScale(1.f, 1.f); // Reset the scale to default
    int offsetX = -5; // Adjust this value as needed, subtracted 10 to shift the texture to the left
    int offsetY = -15; // Adjust this value as needed, added 10 to shift the texture down
    textureRect = sf::IntRect(column * frameWidth + offsetX, row * frameHeight + offsetY, frameWidth, frameHeight);
} else if (velocity.x < 0) { // Player is moving left
    currentFrame = (currentFrame + 1) % 5; // There are 5 frames for walking left
    row = 0; // 2nd row (0-indexed) for walking left
    column = currentFrame; // Columns 0-4 for walking left
    shape.setScale(1.f, 1.f); // Flip the texture horizontally
    int offsetX = 10;
    int offsetY = -10;// Adjust this value as needed
    textureRect = sf::IntRect(column * frameWidth + offsetX, row * frameHeight + offsetY, frameWidth, frameHeight);
} else { // Player is not moving
        currentFrame = (currentFrame + 1) % 4; // There are 6 frames for standing still
        row = 4; // 5th row (0-indexed) for standing still
        column = currentFrame; // Columns 0-5 for standing still
        shape.setScale(1.f, 1.f); // Reset the scale to default
        int offsetX = 7; // Adjust this value as needed, subtracted 10 to shift the texture to the left
        int offsetY = -10; // Adjust this value as needed, added 10 to shift the texture down
        int thinnerFrameWidth = frameWidth - 20; // Subtract 10 to make the frame thinner
        textureRect = sf::IntRect(column * thinnerFrameWidth + offsetX, row * frameHeight + offsetY, thinnerFrameWidth, frameHeight);
    }

shape.setTextureRect(textureRect);
animationClock.restart();
}

    sf::Vector2f position = shape.getPosition();
    sf::Vector2f size = shape.getSize();

    if (position.x < 0) {
        position.x = 0;
        velocity.x = 0;
    } else if (position.x + size.x > 800) { // Adjust if window width is different
        position.x = 800 - size.x;
        velocity.x = 0;
    }

    if (position.y < 0) {
        position.y = 0;
        velocity.y = 0;
    } else if (position.y + size.y > 600) { // Adjust if window height is different
        position.y = 600 - size.y;
        velocity.y = 0;
    }

    shape.setPosition(position);
}

sf::FloatRect WaterPlayer::getBounds() const {
    return shape.getGlobalBounds();
}


void WaterPlayer::handleCollision(const sf::Sprite& sprite){
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

void WaterPlayer::handleCollision(const sf::RectangleShape& platform){
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

void WaterPlayer::handleCollision(const sf::FloatRect& objectBounds){
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
void WaterPlayer::handleLavaCollision(const sf::Sprite& sprite){
    sf::FloatRect playerBounds = shape.getGlobalBounds();
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    if (playerBounds.intersects(spriteBounds)) {
        // Collision occurred
        // Handle lava collision
    setDeathSound("assets/death.wav");
    deathSound.play();
        dead();
    }
}
void WaterPlayer::handleWaterCollision(const sf::Sprite& sprite){
    sf::FloatRect playerBounds = shape.getGlobalBounds();
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    if (playerBounds.intersects(spriteBounds)) {
        // Collision occurred
        // Handle water collision
        handleCollision(sprite);
    }
}

