#include "player.h"

#include <iostream>

Player::Player(sf::Vector2f position, sf::Color color)
    : velocity(0, 0), onGround(false) {
  shape.setSize(sf::Vector2f(50, 50));
  shape.setPosition(position);
  shape.setFillColor(color);
}

void Player::handleInput() {

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {

    velocity.x = -200;

  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {


      velocity.x = 200;

  } else {
    velocity.x = 0;
  }
  if (onGround && sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
    velocity.y = -400;
    onGround = false;
    
  }
}

void Player::setTexture(const sf::Texture& texture, int rows, int columns) {
  this->texture = texture;
  this->rows = rows;
  this->columns = columns;

  // Calculate the size of each frame
  int frameWidth = texture.getSize().x / columns;
  int frameHeight = texture.getSize().y / rows;

  this->totalFrames = rows * columns;

  // Set the first frame
  sf::IntRect textureRect(0, 0, frameWidth, frameHeight);
  shape.setTextureRect(textureRect);

  shape.setTexture(&this->texture);
}

void Player::update(sf::Time deltaTime) {
    std::cout<<speedBoost<<std::endl;
  velocity.y += 981 * deltaTime.asSeconds();  // Gravity
  shape.move(velocity * deltaTime.asSeconds());

  if (shape.getPosition().y + shape.getSize().y > 600) {
    // Collision with ground
    velocity.y = 0;
    onGround = true;
  }

  // Animation update
  float elapsedSeconds = animationClock.getElapsedTime().asSeconds();
  if (elapsedSeconds >= frameDuration) {
    // Move to the next frame
    int row, column;
    int frameWidth =
        texture.getSize().x / 7;  // Total width divided by number of columns
    int frameHeight =
        texture.getSize().y / 5;  // Total height divided by number of rows
    sf::IntRect textureRect;

    if (velocity.x > 0) {  // Player is moving right
      currentFrame =
          (currentFrame + 1) % 5;  // There are 5 frames for walking right
      row = 0;                     // 1st row (0-indexed) for walking right
      column = currentFrame;       // Columns 0-4 for walking right
      shape.setScale(1.f, 1.f);    // Reset the scale to default
      int offsetX = 0;
      int offsetY = -20;  // Adjust this value as needed
      textureRect =
          sf::IntRect(column * frameWidth + offsetX,
                      row * frameHeight + offsetY, frameWidth, frameHeight);
    } else if (velocity.x < 0) {  // Player is moving left
      currentFrame =
          (currentFrame + 1) % 5;  // There are 5 frames for walking left
      row = 1;                     // 2nd row (0-indexed) for walking left
      column = currentFrame;       // Columns 0-4 for walking left
      shape.setScale(1.f, 1.f);    // Flip the texture horizontally
      int offsetX = 14;
      int offsetY = -20;  // Adjust this value as needed
      textureRect =
          sf::IntRect(column * frameWidth + offsetX,
                      row * frameHeight + offsetY, frameWidth, frameHeight);
    } else {  // Player is not moving
      currentFrame =
          (currentFrame + 1) % 6;  // There are 6 frames for standing still
      row = 4;                     // 5th row (0-indexed) for standing still
      column = currentFrame;       // Columns 0-5 for standing still
      shape.setScale(1.f, 1.f);    // Reset the scale to default
      textureRect = sf::IntRect(column * frameWidth, row * frameHeight,
                                frameWidth, frameHeight);
    }

    shape.setTextureRect(textureRect);
    animationClock.restart();
  }

  sf::Vector2f position = shape.getPosition();
  sf::Vector2f size = shape.getSize();

  if (position.x < 0) {
    position.x = 0;
    velocity.x = 0;
  } else if (position.x + size.x > 800) {  
    position.x = 800 - size.x;
    velocity.x = 0;
  }

  if (position.y < 0) {
    position.y = 0;
    velocity.y = 0;
  } else if (position.y + size.y > 600) { 
    position.y = 600 - size.y;
    velocity.y = 0;
  }

  shape.setPosition(position);
}

sf::FloatRect Player::getBounds() const { return shape.getGlobalBounds(); }

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(shape, states);
}

void Player::handleCollision(const sf::RectangleShape& object) {
  sf::FloatRect objectBounds = object.getGlobalBounds();
  sf::FloatRect playerBounds = shape.getGlobalBounds();

  // Sprawdź, czy gracz koliduje z obiektem z góry
  if (playerBounds.top + playerBounds.height <= objectBounds.top + 10 &&
      playerBounds.top + playerBounds.height > objectBounds.top &&
      playerBounds.left + playerBounds.width > objectBounds.left &&
      playerBounds.left < objectBounds.left + objectBounds.width) {
    shape.setPosition(playerBounds.left,
                      objectBounds.top - playerBounds.height);
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
    shape.setPosition(playerBounds.left,
                      spriteBounds.top - playerBounds.height);
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
  // Sprawdz kolizje od dołu
  else if (playerBounds.top < spriteBounds.top + spriteBounds.height &&
           playerBounds.top > spriteBounds.top + spriteBounds.height - 10 &&
           playerBounds.left + playerBounds.width > spriteBounds.left &&
           playerBounds.left < spriteBounds.left + spriteBounds.width) {
    shape.setPosition(playerBounds.left,
                      spriteBounds.top + spriteBounds.height);
    velocity.y = 0;
  }
  handleLavaCollision(sprite);
  handleWaterCollision(sprite);
}
void Player::setTargetReached() { isTargetReached = true; }

bool Player::getTargetReached() { return isTargetReached; }
void Player::setStartingPosition(sf::Vector2f startingPosition) {
  shape.setPosition(startingPosition);
  this->startingPosition = startingPosition;
}
void Player::handleLavaCollision(const sf::Sprite& sprite) {
  // Implementacja w klasach dziedziczacych
}
void Player::handleWaterCollision(const sf::Sprite& sprite) {
  // Implementacja w klasach dziedziczacych
}
// Funkcja sprawiajaca ze gracz po zetknieciu z nie ta plafrotma co moze cofa
// sie na poczatek poziomu
void Player::dead() {
  setDeathSound("assets/death.wav");
  deathSound.play();
  shape.setPosition(startingPosition);
}
void Player::setDeathSound(const std::string& soundPath) {
  if (!deathBuffer.loadFromFile(soundPath)) {
    std::cerr << "Could not load death sound" << std::endl;
  }
  deathSound.setBuffer(deathBuffer);
}
void Player::handleAcidCollision(const sf::Sprite& sprite) {
  sf::FloatRect playerBounds = shape.getGlobalBounds();
  sf::FloatRect spriteBounds = sprite.getGlobalBounds();
  if (playerBounds.intersects(spriteBounds)) {
    // Collision occurred
    // Handle acid collision
    setDeathSound("assets/death.wav");
    deathSound.play();
    dead();
  }
}
void Player::handleSlipperyWallCollision(const sf::Sprite& sprite) {
    sf::FloatRect playerBounds = shape.getGlobalBounds();
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    sf::FloatRect bufferedPlayerBounds = playerBounds;
    //Zbuforowane zeby nie powodowalo bledow
    bufferedPlayerBounds.left -= 0.1f;
    bufferedPlayerBounds.top -= 0.1f;
    bufferedPlayerBounds.width += 0.2f;
    bufferedPlayerBounds.height += 0.2f;

    if (bufferedPlayerBounds.intersects(spriteBounds)) {
        speedBoost = 200;
    } else {
        speedBoost = 0;
    }
    handleCollision(sprite);

}
void Player::normalSpeedSetter(){
    speedBoost = 0;
}
void Player::handleMudCollision(const sf::Sprite& sprite) {
    sf::FloatRect playerBounds = shape.getGlobalBounds();
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    sf::FloatRect bufferedPlayerBounds = playerBounds;
    //Zbuforowane zeby nie powodowalo bledow
    bufferedPlayerBounds.left -= 0.1f;
    bufferedPlayerBounds.top -= 0.1f;
    bufferedPlayerBounds.width += 0.2f;
    bufferedPlayerBounds.height += 0.2f;

    if (bufferedPlayerBounds.intersects(spriteBounds)) {
        speedBoost = -100;
    } else {
        speedBoost = 0;
    }
    handleCollision(sprite);

}