#include "level.h"

#include <SFML/Graphics.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "tiles.h"
#include "level.h"

  // Constructor
Level::Level() { this->loadTextures("assets/textures.txt"); 
}

  // Rysowanie poziomu
void Level::draw(sf::RenderWindow& window) {
  if(!isLoaded){
    return;
  }
  for (const auto& platform : platforms) {
    window.draw(platform);
  }
  for(const auto&ptr_sprite:ptr_sprites){
      window.draw(*ptr_sprite);
  }
  for (const auto& sprite : sprites) {
    window.draw(sprite);
  }

  for (const auto& sprite : sprites) {
    //dodac ze nie renderuje gema jak zostanie zdobyty
    if(isGem(sprite)&&getGemCollected()){
        continue;
    }
  }
}
  // Sprawdzenie kolizji
void Level::checkCollisions(Player& player) {
  sf::FloatRect playerBounds = player.getBounds();
    playerBounds.left-=0.3f;
  playerBounds.top-=0.3f;
    playerBounds.width-=0.3f;
  playerBounds.height-=0.3f;
  // Check collisions with walls
  for (const auto& wall : walls) {
    if (wall.getGlobalBounds().intersects(playerBounds)) {
      player.handleCollision(wall);
    }
  }

  for(const auto& sprite:ptr_sprites){
      sf::FloatRect spriteBounds = (*sprite).getGlobalBounds();
      // Wylaczenie kolizji dla goalTile i startingTile
      if (spriteBounds == goalTile.getGlobalBounds() ||
          spriteBounds == gemTile.getGlobalBounds() ||
          spriteBounds == startingTile.getGlobalBounds() ||
          spriteBounds == fireStartingTile.getGlobalBounds() ||
          spriteBounds == waterStartingTile.getGlobalBounds() ||
          spriteBounds == buttonTile.getGlobalBounds()) {
          continue;
      }
      if(auto derivedPtr = std::dynamic_pointer_cast<LavaTile>(sprite)){
          player.handleLavaCollision(*derivedPtr);
      }
      if(auto derivedPtr = std::dynamic_pointer_cast<WaterTile>(sprite)){
          player.handleWaterCollision(*derivedPtr);
      }
      if(auto derivedPtr = std::dynamic_pointer_cast<AcidTile>(sprite)){
          player.handleAcidCollision(*derivedPtr);
      }
      if(auto derivedPtr = std::dynamic_pointer_cast<SlipperyTile>(sprite)){
          player.handleSlipperyWallCollision(*derivedPtr);
      }

  }
  // Check side collisions with walls
  for (const auto& wall : walls) {
    sf::FloatRect wallBounds = wall.getGlobalBounds();
    if (wallBounds.intersects(playerBounds)) {
      player.handleCollision(wallBounds);
    }
  }

  // Check collisions with sprites
  for (const auto& sprite : sprites) {
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    // Wylaczenie kolizji dla goalTile i startingTile
   if (spriteBounds == goalTile.getGlobalBounds() ||
            spriteBounds == gemTile.getGlobalBounds() ||
            spriteBounds == startingTile.getGlobalBounds() ||
            spriteBounds == fireStartingTile.getGlobalBounds() ||
            spriteBounds == waterStartingTile.getGlobalBounds() ||
            spriteBounds == buttonTile.getGlobalBounds()) {
            continue;
        }


    // Sprawdzenie kolizji od gory
    if (sprite.getGlobalBounds().intersects(playerBounds)) {
        player.handleCollision(sprite);
        player.normalSpeedSetter();

    }
     if (!isButtonPressed) {
            for (const auto& receiverSprite : receiverSprites) {
              if(isButtonPressed){
                continue;
              }
                if (receiverSprite.getGlobalBounds().intersects(playerBounds)) {
                    player.handleCollision(receiverSprite);
                }
            }
        }
    // for(auto &receiverSprite : receiverSprites) {
    //   if(isButtonPressed) {
    //     continue;  // Jeśli isButtonPressed jest true, pomiń ten sprite
    //   }

    //   if(receiverSprite.getGlobalBounds().intersects(playerBounds)) {
    //     player.handleCollision(receiverSprite);
    //   }
    // }
    // Sprawdzenie kolizji z lava, water, acid, slippery walls
    for (const auto& lavaSprite : lavaSprites) {
      if (lavaSprite.getGlobalBounds().intersects(playerBounds)) {
        player.handleLavaCollision(lavaSprite);
          player.normalSpeedSetter();
      }
    }
    for (const auto& waterSprite : waterSprites) {
      if (waterSprite.getGlobalBounds().intersects(playerBounds)) {
        player.handleWaterCollision(waterSprite);
          player.normalSpeedSetter();
      }
    }
    for (const auto& acidSprite : acidSprites) {
      if (acidSprite.getGlobalBounds().intersects(playerBounds)) {
        player.handleAcidCollision(acidSprite);
          player.normalSpeedSetter();
      }
    }
    for (const auto& slippery_wallsSprite : slippery_wallsSprites) {
      if (slippery_wallsSprite.getGlobalBounds().intersects(playerBounds)) {
      
        player.handleSlipperyWallCollision(slippery_wallsSprite);
      }

    }
    for(auto & mudSprite : mudSprites){
        if(mudSprite.getGlobalBounds().intersects(playerBounds)){
            player.handleMudCollision(mudSprite);
        }
    }
    // Sprawdzenie czy ktorys z graczy zdobywa gem
    for(auto & gemSprite : gemSprites){
        if(gemSprite.getGlobalBounds().intersects(playerBounds)){
          if(!getGemCollected()){
          gemSprite.setColor(sf::Color(255,255,255,0));
          gemCollected=true;
          std::cout<<"Gem collected"<<std::endl;
         }
        }
    }
    //Sprawdzenie czy ktorys z graczy znajduje sie na przycisku
    for(auto &buttonSprite : buttonSprites){
        if(buttonSprite.getGlobalBounds().intersects(playerBounds)){
            isButtonPressed=true;
        }
        else{
            isButtonPressed=false;

        }
    }
    for(auto &receiverSprites : receiverSprites){
        if(receiverSprites.getGlobalBounds().intersects(playerBounds)){
            if(!isButtonPressed){
                player.handleCollision(receiverSprites);
            }
            else{
              continue;
            }
        }
    } // Sprawdzenie kolizji od gory
    if (sprite.getGlobalBounds().intersects(playerBounds)) {
        player.handleCollision(sprite);
        player.normalSpeedSetter();
  }
  }
}

  // Sprawdzenie czy gracz dotarl do celu
void Level::loadTextures(std::string texture_loader_filepath) {
  std::string line;
  std::ifstream file(texture_loader_filepath);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open texture file");
  }
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string texturePath;
    iss >> texturePath;
    sf::Texture newTexture;
    if (!newTexture.loadFromFile(texturePath)) {
      throw std::runtime_error("Failed to load texture from " + texturePath);
    }

    // std::cerr << "Loaded texture: " << texturePath << std::endl;
    textures.emplace_back(newTexture);
  }
}

  // Sprawdzenie czy gracz dotarl do celu
void Level::loadFromFile(std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open level file");
  }

  int mapWidth, mapHeight;
  int tileSizeX, tileSizeY;
  file >> mapWidth >> mapHeight >> tileSizeX >> tileSizeY;
  // std::cerr << "Map width: " << mapWidth << " Map height: " << mapHeight << "
  // Tile size x: " << tileSizeX << " Tile size y: " << tileSizeY << std::endl;
  std::cout << "Map width: " << mapWidth << " Map height: " << mapHeight
            << " Tile size x: " << tileSizeX << " Tile size y: " << tileSizeY
            << std::endl;

  std::map<std::pair<int, int>, char> tileMap;

  std::string line;
  int y = 0;
  while (std::getline(file, line)) {
    for (int x = 0; x < line.size(); ++x) {
      char tileType = line[x];
      tileMap[{x, y}] = tileType;

      switch (tileType) {
        case '#': {
          sf::Sprite wall;
          wall.setTexture(textures[0]);
          wall.setScale(
              tileSizeX / static_cast<float>(textures[0].getSize().x),
              tileSizeY / static_cast<float>(textures[0].getSize().y));
          wall.setPosition(x * tileSizeX, y * tileSizeY);
          sprites.push_back(wall);
          // std::cerr << "Wall added at (" << x * tileSizeX << ", " << y *
          // tileSizeY << ")" << std::endl;
          break;
        }
        case '@': {
          sf::RectangleShape coin(sf::Vector2f(tileSizeX, tileSizeY));
          coin.setFillColor(sf::Color::Yellow);
          coin.setPosition(x * tileSizeX, y * tileSizeY);
          walls.emplace_back(coin);
          // std::cerr << "Coin added at (" << x * tileSizeX << ", " << y *
          // tileSizeY << ")" << std::endl;
          break;
        }
        case '$': {
          sf::Sprite grass;
          grass.setTexture(textures[1]);
          grass.setScale(
              tileSizeX / static_cast<float>(textures[1].getSize().x),
              tileSizeY / static_cast<float>(textures[1].getSize().y));
          grass.setPosition(x * tileSizeX, y * tileSizeY);
          sprites.push_back(grass);
          // std::cerr << "Grass added at (" << x * tileSizeX << ", " << y *
          // tileSizeY << ")" << std::endl;
          break;
        }
        case 'o': {
          sf::Sprite goal;
          goal.setTexture(textures[2]);
          goal.setScale(
              tileSizeX / static_cast<float>(textures[2].getSize().x),
              tileSizeY / static_cast<float>(textures[2].getSize().y));
          goal.setPosition(x * tileSizeX, y * tileSizeY);
          sprites.push_back(goal);
          goalTile = goal;
          // std::cerr << "Goal added at (" << x * tileSizeX << ", " << y *
          // tileSizeY << ")" << std::endl;
          break;
        }
        case 's': {
          // Starting position tile, textura to drzwi
          sf::Sprite start;
          start.setTexture(textures[10]);
          start.setScale(
              tileSizeX / static_cast<float>(textures[10].getSize().x),
              tileSizeY / static_cast<float>(textures[10].getSize().y));
          start.setPosition(x * tileSizeX, y * tileSizeY);
          sprites.push_back(start);
          startingPosition = start.getPosition();
          startingTile = start;
          // std::cerr << "Start added at (" << x * tileSizeX << ", " << y *
          // tileSizeY << ")" << std::endl;
          break;
        }
        case 'l': {
          sf::Sprite lava;
          lava.setTexture(textures[6]);
          lava.setScale(
              tileSizeX / static_cast<float>(textures[6].getSize().x),
              tileSizeY / static_cast<float>(textures[6].getSize().y));
          lava.setPosition(x * tileSizeX, y * tileSizeY);
          sprites.push_back(lava);
          // Dodanie do wektora spriteow law zeby potem mozna bylo sprawdzic czy
          // woda nie wchodzi do lawy
          lavaSprites.push_back(lava);
          // std::cerr << "Lava added at (" << x * tileSizeX << ", " << y *
          // tileSizeY << ")" << std::endl;
          break;
        }
        case 'w': {
            auto waterSprite = std::make_unique<WaterTile>();
            waterSprite->setTexture(textures[7]);
            waterSprite->setScale(tileSizeX / static_cast<float>(textures[7].getSize().x),
                                  tileSizeY / static_cast<float>(textures[7].getSize().y));
            waterSprite->setPosition(x*tileSizeX,y*tileSizeY);
            ptr_sprites.emplace_back(std::move(waterSprite));

          sf::Sprite water;

          water.setTexture(textures[7]);
          water.setScale(
              tileSizeX / static_cast<float>(textures[7].getSize().x),
              tileSizeY / static_cast<float>(textures[7].getSize().y));
          water.setPosition(x * tileSizeX, y * tileSizeY);
          sprites.push_back(water);
          // Dodanie do wektora spriteow water zeby potem mozna bylo sprawdzic
          // czy goein nie wchodzi do wody
          waterSprites.push_back(water);
          // std::cerr << "Water added at (" << x * tileSizeX << ", " << y *
          // tileSizeY << ")" << std::endl;
          break;
        }
        // Punkty startowe dla graczy gdyby mieli zaczynac w innych miejscach
        case '1': {
          sf::Sprite fireStartingTile;
          fireStartingTile.setTexture(textures[11]);
          fireStartingTile.setScale(
              tileSizeX / static_cast<float>(textures[11].getSize().x),
              tileSizeY / static_cast<float>(textures[11].getSize().y));
          fireStartingTile.setPosition(x * tileSizeX, y * tileSizeY);
          sprites.push_back(fireStartingTile);
          fireStartingPosition = fireStartingTile.getPosition();
          this->fireStartingTile = fireStartingTile;
          break;
        }
        case '2': {
          sf::Sprite waterStartingTile;
          waterStartingTile.setTexture(textures[12]);
          waterStartingTile.setScale(
              tileSizeX / static_cast<float>(textures[12].getSize().x),
              tileSizeY / static_cast<float>(textures[12].getSize().y));
          waterStartingTile.setPosition(x * tileSizeX, y * tileSizeY);
          sprites.push_back(waterStartingTile);
          waterStartingPosition = waterStartingTile.getPosition();
          this->waterStartingTile = waterStartingTile;
          break;
        }
        // Tile z kwasem w ktorym gracze giną
        case 'a': {
            //Proba z pointerami
            auto acidSprite = std::make_unique<AcidTile>();
            acidSprite->setTexture(textures[13]);
            acidSprite->setScale(tileSizeX / static_cast<float>(textures[13].getSize().x),
                                  tileSizeY / static_cast<float>(textures[13].getSize().y));
            acidSprite->setPosition(x*tileSizeX,y*tileSizeY);
            ptr_sprites.emplace_back(std::move(acidSprite));


          sf::Sprite acid;
          acid.setTexture(textures[13]);
          acid.setScale(
              tileSizeX / static_cast<float>(textures[13].getSize().x),
              tileSizeY / static_cast<float>(textures[13].getSize().y));
          acid.setPosition(x * tileSizeX, y * tileSizeY);
          sprites.push_back(acid);
          acidSprites.push_back(acid);
          // std::cerr << "Acid added at (" << x * tileSizeX << ", " << y *
          // tileSizeY << ")" << std::endl;
          break;
        }

        case 'k': {
            auto slipperySprite = std::make_unique<SlipperyTile>();
            slipperySprite->setTexture(textures[14]);
            slipperySprite->setScale(
                tileSizeX / static_cast<float>(textures[14].getSize().x),
                tileSizeY / static_cast<float>(textures[14].getSize().y));
            slipperySprite->setPosition(x * tileSizeX, y * tileSizeY);
            ptr_sprites.push_back(std::move(slipperySprite));


          sf::Sprite slippery_wall;
          slippery_wall.setTexture(textures[14]);
          slippery_wall.setScale(
              tileSizeX / static_cast<float>(textures[14].getSize().x),
              tileSizeY / static_cast<float>(textures[14].getSize().y));
          slippery_wall.setPosition(x * tileSizeX, y * tileSizeY);
          sprites.push_back(slippery_wall);
          slippery_wallsSprites.push_back(slippery_wall);
          // std::cerr << "Slippery wall added at (" << x * tileSizeX << ", " <<
          // y * tileSizeY << ")" << std::endl;
          break;
        }
        case 'g': {
          if(gemCollected){
              break;
          }
          sf::Sprite gem;
          gem.setTexture(textures[15]);
          gem.setScale(
              tileSizeX / static_cast<float>(textures[15].getSize().x),
              tileSizeY / static_cast<float>(textures[15].getSize().y));
          gem.setPosition(x * tileSizeX, y * tileSizeY);
          sprites.push_back(gem);
          gemSprites.push_back(gem);
          gemTile = gem;
          break;
        }
        case 'm':
        {
            //Dodanie blota na kotrym gracz bedzie zwalniac
            sf::Sprite mud_Sprite;
            mud_Sprite.setTexture(textures[16]);
            mud_Sprite.setScale(
                tileSizeX / static_cast<float>(textures[16].getSize().x),
                tileSizeY / static_cast<float>(textures[16].getSize().y));
            mud_Sprite.setPosition(x * tileSizeX, y * tileSizeY);
            sprites.push_back(mud_Sprite);
            mudSprites.push_back(mud_Sprite);
            break;

        }
        case 'b':
        {
          //Przycisk na ktorym jeden gracz bedzie musial stanac zeby drugi mogl przejsc
          sf::Sprite button;
          button.setTexture(textures[17]);
          button.setScale(
              tileSizeX / static_cast<float>(textures[17].getSize().x),
              tileSizeY / static_cast<float>(textures[17].getSize().y));
          button.setPosition(x * tileSizeX, y * tileSizeY);
          sprites.push_back(button);
          buttonSprites.push_back(button);
          buttonTile = button;
          break;
      
        }
        case 'r':
        {
          //Odbiornik ktory po przycisnieciu przycisku bedzie znikal
          sf::Sprite receiver;
          receiver.setTexture(textures[18]);
          receiver.setScale(
              tileSizeX / static_cast<float>(textures[18].getSize().x),
              tileSizeY / static_cast<float>(textures[18].getSize().y));
          receiver.setPosition(x * tileSizeX, y * tileSizeY);
          sprites.push_back(receiver);
          receiverSprites.push_back(receiver);
          receiverTile = receiver;
          break;
        }
        default:
          // Handle other tile types
          break;

      }
    }
    y++;
  }
  std::cerr << "Finished loading level " << filename << std::endl;
  file.close();
  isLoaded = true;
}
bool Level::chekGoalReached(Player& player) {
  sf::FloatRect playerBounds = player.getBounds();
  if (playerBounds.intersects(goalTile.getGlobalBounds())) {
    // std::cout<<"Goal reached"<<std::endl;
    player.setTargetReached();
    return true;
  }
  return false;
}

  // Sprawdzenie czy gracz zdobyl gem
bool Level::checkGemReached(Player& player) {
  sf::FloatRect playerBounds = player.getBounds();
  if (playerBounds.intersects(gemTile.getGlobalBounds())) {
    // std::cout<<"Gem reached"<<std::endl;
    // player.setTargetReached();
    return true;
  }
  return false;
}

// Spradza czy gra jest skonczona itd
void Level::checkEndGame(std::vector<Player*> players) {
  if (players[0]->getTargetReached() && players[1]->getTargetReached()) {
    setLevelFinished();
    // Jezeli gra jest skonczona to odpala funkcje konczaca gre
    // endGame();
  }
  // else if(players[0]->getTargetReached()){
  //     std::cout<<"Player 1 reached the goal"<<std::endl;
  //     exit(0);
  // }
  // else if(players[1]->getTargetReached()){
  //     std::cout<<"Player 2 reached the goal"<<std::endl;
  //     exit(0);
  // }
  // else{
  //     //std::cout<<"No player reached the goal"<<std::endl;
  // }
}
void Level::setLevelFinished() { this->levelFinished = true; }
// Funkcja konczaca gre
void Level::endGame() {
  if (this->levelFinished) {
  }
}
bool Level::getLevelFinished() { return levelFinished; }
// getter do pozycji startowej graczy
sf::Vector2f Level::getStartingPosition() { return this->startingPosition; }
// getter do pozycji startowej gracza fire
sf::Vector2f Level::getFireStartingPosition() {
  return this->fireStartingPosition;
}
// getter do pozycji startowej gracza water
sf::Vector2f Level::getWaterStartingPosition() {
  return this->waterStartingPosition;
}

std::string Level::getLevelPath() { return this->levelPath; }

void Level::saveBestTime() {
  std::string levelFilePath = this->getLevelPath();

  // Extract the base filename from the level path
  std::filesystem::path levelPath(levelFilePath);
  std::string filename = levelPath.filename().string();

  // std::cout << this->arglevelPath << std::endl;

  std::string timeFileName =
      arglevelPath.substr(arglevelPath.find_last_of("level") + 1);
  timeFileName = timeFileName[0];
  // std::cout << timeFileName << std::endl; // Assuming the filename starts
  // with "level" and is followed by a number

  // arglevelPath == "assets/level" + n +".txt"
  // Construct the time file path
  // timeFilePath = "assets/" + timeFileName + ".txt";
  // Poprwana wersja zapisu czasu do plików z czasami
  std::string timeFilePath = "assets/Time" + timeFileName + ".txt";

  // Open the file for appending
  std::ofstream file;
  file.open(timeFilePath, std::ios::app);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open best times file: " + timeFilePath);
  }

  // Write the new time to the file
  std::string newTime = this->getFinalTime();
  std::cerr << "Time to be saved: " << newTime << std::endl;
  file << newTime << std::endl;
  if (!file) {
    throw std::runtime_error("Failed to write to best times file: " +
                             timeFilePath);
  }
  file.close();
  std::cerr << "Time successfully saved" << std::endl;
}

void Level::setFinalTime(std::string time) { this->finalTime = time; }
std::string Level::getFinalTime() { return this->finalTime; }
bool Level::getGemCollected() { return gemCollected; }
bool Level::isGem(const sf::Sprite& sprite) {
  // Pobierz teksturę sprite'a
  const sf::Texture* spriteTexture = sprite.getTexture(); 

  // Pobierz teksturę gemu
  sf::Texture* gemTexture = &textures[15]; 

  // Sprawdź czy wtekstury są takie same
  return spriteTexture == gemTexture; 
}
bool Level::isReceiver(const sf::Sprite& sprite) {
  // Pobierz teksturę sprite'a
  const sf::Texture* spriteTexture = sprite.getTexture(); 

  // Pobierz teksturę receivera
  sf::Texture* receiverTexture = &textures[18]; 

  // Sprawdź czy wtekstury są takie same
  return spriteTexture == receiverTexture; 
}
void Level::checkCollisions(const std::vector<Player*>& players) {
  for (auto& playerPtr : players) {
    Player& player = *playerPtr;

  sf::FloatRect playerBounds = player.getBounds();
    playerBounds.left-=0.3f;
  playerBounds.top-=0.3f;
    playerBounds.width-=0.3f;
  playerBounds.height-=0.3f;
  // Check collisions with walls
  for (const auto& wall : walls) {
    if (wall.getGlobalBounds().intersects(playerBounds)) {
      player.handleCollision(wall);
    }
  }

  // Check side collisions with walls
  for (const auto& wall : walls) {
    sf::FloatRect wallBounds = wall.getGlobalBounds();
    if (wallBounds.intersects(playerBounds)) {
      player.handleCollision(wallBounds);
    }
  }
  // Check collisions with sprites
  for (const auto& sprite : sprites) {
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    // Wylaczenie kolizji dla goalTile i startingTile
   if (spriteBounds == goalTile.getGlobalBounds() ||
            spriteBounds == gemTile.getGlobalBounds() ||
            spriteBounds == startingTile.getGlobalBounds() ||
            spriteBounds == fireStartingTile.getGlobalBounds() ||
            spriteBounds == waterStartingTile.getGlobalBounds() ||
            spriteBounds == buttonTile.getGlobalBounds()) {
            continue;
        } 
        if(spriteBounds == receiverTile.getGlobalBounds()&&isButtonPressed){
            continue;
        }
    // Sprawdzenie kolizji od gory
        if (!isButtonPressed) {
            for (const auto& receiverSprite : receiverSprites) {
              if(isButtonPressed){
                continue;
              }
                if (receiverSprite.getGlobalBounds().intersects(playerBounds)) {
                    player.handleCollision(receiverSprite);
                }
            }
        }
    // for(auto &receiverSprite : receiverSprites) {
    //   if(isButtonPressed) {
    //     continue;  // Jeśli isButtonPressed jest true, pomiń ten sprite
    //   }

    //   if(receiverSprite.getGlobalBounds().intersects(playerBounds)) {
    //     player.handleCollision(receiverSprite);
    //   }
    // }
    for (const auto& lavaSprite : lavaSprites) {
      if (lavaSprite.getGlobalBounds().intersects(playerBounds)) {
        player.handleLavaCollision(lavaSprite);
          player.normalSpeedSetter();
      }
    }
    for (const auto& waterSprite : waterSprites) {
      if (waterSprite.getGlobalBounds().intersects(playerBounds)) {
        player.handleWaterCollision(waterSprite);
          player.normalSpeedSetter();
      }
    }
    for (const auto& acidSprite : acidSprites) {
      if (acidSprite.getGlobalBounds().intersects(playerBounds)) {
        player.handleAcidCollision(acidSprite);
          player.normalSpeedSetter();
      }
    }
    for (const auto& slippery_wallsSprite : slippery_wallsSprites) {
      if (slippery_wallsSprite.getGlobalBounds().intersects(playerBounds)) {
      
        player.handleSlipperyWallCollision(slippery_wallsSprite);
      }

    }
    for(auto & mudSprite : mudSprites){
        if(mudSprite.getGlobalBounds().intersects(playerBounds)){
            player.handleMudCollision(mudSprite);
        }
    }
    for(auto & gemSprite : gemSprites){
        if(gemSprite.getGlobalBounds().intersects(playerBounds)){
          if(!getGemCollected()){
          gemSprite.setColor(sf::Color(255,255,255,0));
          gemCollected=true;
          std::cout<<"Gem collected"<<std::endl;
         }
        }
    }
    //Sprawdzenie czy ktorys z graczy znajduje sie na przycisku
    for(auto &buttonSprite : buttonSprites){
        if(buttonSprite.getGlobalBounds().intersects(playerBounds)){
            isButtonPressed=true;
        }
        // else{
        //     isButtonPressed=false;

        // }
    }
    for(auto &receiverSprites : receiverSprites){
        if(receiverSprites.getGlobalBounds().intersects(playerBounds)){
            if(!isButtonPressed){
                player.handleCollision(receiverSprites);
            }
            else{
              continue;
            }
        }
     // Sprawdzenie kolizji od gory
  //   if (sprite.getGlobalBounds().intersects(playerBounds)) {
  //       player.handleCollision(sprite);
  //       player.normalSpeedSetter();

    
  //   // Sprawdzenie kolizji od gory
  //   // if (sprite.getGlobalBounds().intersects(playerBounds)) {
  //   //   player.handleCollision(sprite);
  //   // }
  // }
  }
  // Check side collisions with sprites
  // for (const auto& sprite : sprites) {
  //   sf::FloatRect spriteBounds = sprite.getGlobalBounds();
  //   // Wylaczenie kolizji dla goalTile i startingTile
  //   if (spriteBounds == goalTile.getGlobalBounds()) {
  //     continue;
  //   }
    
  //   if (spriteBounds == startingTile.getGlobalBounds()) {
  //     continue;
  //   }
  //   if (spriteBounds == gemTile.getGlobalBounds()) {
  //     continue;
  //   }
  //   // Sprawdzenie kolizji dla fireStartingTile i waterStartingTile
  //   if (spriteBounds == fireStartingTile.getGlobalBounds()) {
  //     continue;
  //   }
  //   if (spriteBounds == waterStartingTile.getGlobalBounds()) {
  //     continue;
  //   }
  //   if(spriteBounds == buttonTile.getGlobalBounds()){
  //       continue;
  //   }
  //   if (spriteBounds.intersects(playerBounds)) {
  //     player.handleCollision(spriteBounds);

  //   }
  // }
 if (sprite.getGlobalBounds().intersects(playerBounds)) {
        player.handleCollision(sprite);
        player.normalSpeedSetter();

    }

  }
}
}
