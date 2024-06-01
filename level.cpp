
#include "level.h"

Level::Level() {
    sf::RectangleShape platform(sf::Vector2f(200,20));
    platform.setFillColor(sf::Color::Green);
    platform.setPosition(300,400);
    platforms.emplace_back(platform);
}
void Level::draw(sf::RenderWindow& window){
    for(const auto& platform : platforms){
        window.draw(platform);
    }
}


void Level::checkCollisions(Player& player) {
    sf::FloatRect playerBounds = player.getBounds();
    for (const auto& platform : platforms) {
        if (platform.getGlobalBounds().intersects(playerBounds)) {
            player.handleCollision(platform);
        }
    }
}
