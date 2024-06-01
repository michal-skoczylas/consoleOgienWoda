
#include "level.h"

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
Level::Level() {
  this->loadTextures(
      "/home/michal/QT_Apps/consoleOgienWoda/assets/textures.txt");
}
void Level::draw(sf::RenderWindow& window) {
  for (const auto& platform : platforms) {
    window.draw(platform);
  }
//   for (const auto& tiles : walls) {
//     window.draw(tiles);
//   }
  for(const auto& sprite : sprites){
    window.draw(sprite);
    std::cout<< sprite.getTexture()<<std::endl;
    std::cerr<<"Sprite drawn"<<std::endl;
  }
  std::cerr<<"Level drawn"<<std::endl;
}

void Level::checkCollisions(Player& player) {
  sf::FloatRect playerBounds = player.getBounds();

  // Sprawdź kolizje z platformami
  for (const auto& platform : platforms) {
    if (platform.getGlobalBounds().intersects(playerBounds)) {
      player.handleCollision(platform);
    }
  }

  // Sprawdź kolizje z obiektami typu wall
  for (const auto& wall : walls) {
    if (wall.getGlobalBounds().intersects(playerBounds)) {
      player.handleCollision(wall);
    }
  }

  // Dodaj kod do sprawdzania kolizji z bokami obiektów
  for (const auto& wall : walls) {
    sf::FloatRect wallBounds = wall.getGlobalBounds();
    if (wallBounds.intersects(playerBounds)) {
      player.handleCollision(wallBounds);
    }
  }
  // Sprawdź kolizje z obiektami typu sprite
  for(const auto& sprite : sprites){
    if(sprite.getGlobalBounds().intersects(playerBounds)){
      player.handleCollision(sprite);
    }
  }
  //Kolizja z bokami spriteow
  for(const auto& sprite : sprites){
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    if(spriteBounds.intersects(playerBounds)){
      player.handleCollision(spriteBounds);
    }
  }
}

void Level::loadTextures(std::string texture_loader_filepath) {
  std::string line;
  std::ifstream file(texture_loader_filepath);
  sf::Texture newTexture;
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open texture file");
  }
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string texturePath;
    iss >> texturePath;
    if(!newTexture.loadFromFile(texturePath)){
      throw std::runtime_error("Failed to load texture");
      std::cerr << "Failed to load texture" << std::endl;
      }

    std::cerr<<"Loaded texture: "<<texturePath<<std::endl;
    textures.emplace_back(newTexture);
  }
  
    
}

void Level::loadFromFile(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open level file");
  }

  // Wczytaj rozmiar mapy i rozmiar kafelka
  int mapWidth, mapHeight;
  int tileSizeX, tileSizeY;
  file >> mapWidth >> mapHeight >> tileSizeX >> tileSizeY;
 std::cerr<<"Map width: "<<mapWidth<<" Map height: "<<mapHeight<<" Tile size x: "<<tileSizeX<<" Tile size y: "<<tileSizeY<<std::endl;
 std::cout<<"Map width: "<<mapWidth<<" Map height: "<<mapHeight<<" Tile size x: "<<tileSizeX<<" Tile size y: "<<tileSizeY<<std::endl;
  // Utwórz mapę asocjacyjną dla kafli
  std::map<std::pair<int, int>, char> tileMap;

  // Wczytaj kafelki z pliku tekstowego
  std::string line;
  int y = 0;
  while (std::getline(file, line)) {
    for (int x = 0; x < line.size(); ++x) {
      char tileType = line[x];
      tileMap[{x, y}] = tileType;

      switch (tileType) {
        case '#': {
          // // Dodaj ścianę
          // sf::RectangleShape wall(sf::Vector2f(tileSizeX, tileSizeY));
          // wall.setFillColor(sf::Color::Blue);
          // wall.setPosition(x * tileSizeX, y * tileSizeY);
          // walls.push_back(wall);
          // break;
          sf::Sprite wall;
          wall.setTexture(textures[0]);
          // Pozniej dodac tu aze jak sie nie zaladuje tesktura to uzyjemy
          // zwyklych sf::RectangleShape - wtedy zamienic typ wektora na sf
          // drawable 
          //Ustawienie zunifikowanej wielkości tekstur
          wall.setScale(tileSizeX / textures[0].getSize().x,
                        tileSizeY / textures[0].getSize().y);
          wall.setPosition(x * tileSizeX, y * tileSizeY);
          sprites.push_back(wall);
          std::cerr<<"Wall added"<<std::endl;
          break;
        }
        case '@': {
          sf::RectangleShape coin(sf::Vector2f(tileSizeX, tileSizeY));
          coin.setFillColor(sf::Color::Yellow);
          coin.setPosition(x * tileSizeX, y * tileSizeY);
          walls.emplace_back(coin);
          std::cerr<<"Coin added"<<std::endl;
          break;
        }
        case '$': {
          // Ustaw pozycję celu
        //   goal = sf::Vector2f(x * tileSizeX, y * tileSizeY);
        //   break;
        sf::Sprite grass;
        grass.setTexture(textures[1]);
        // Pozniej dodac tu aze jak sie nie zaladuje tesktura to uzyjemy
        // zwyklych sf::RectangleShape - wtedy zamienic typ wektora na sf
        // drawable
        //Ustawienie zunifikowanej wielkości tekstur
        grass.setScale(tileSizeX / textures[1].getSize().x,
                      tileSizeY / textures[1].getSize().y);
        grass.setPosition(x * tileSizeX, y * tileSizeY);
        sprites.push_back(grass);
        std::cerr<<"Grass added"<<std::endl;
        break;
        }
        default:
          // Obsłuż inne typy kafli
          break;
      }
    }
    y++;
  }
 std::cerr<<"Skonczylem wczytywac poziom"<<std::endl;
  // Zamknij plik
  file.close();
}
