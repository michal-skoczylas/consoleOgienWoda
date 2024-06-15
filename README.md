# consoleOgienWoda

## Opis gry

W grze gramy jako Ogień i Woda. Sterujemy za pomocą klawiszy W, A, S, D oraz strzałkowych. Na mapie znajdują się różne kafelki i obiekty.

## Lista kafelków i obiektów

- **WaterTile**: Kafelek, po którym może poruszać się woda, ale ogień umiera po jego dotknięciu.
- **LavaTile**: Kafelek, po którym może poruszać się ogień, ale woda umiera po jego dotknięciu.
- **AcidTile**: Kafelek z kwasem, który zabija dowolnego gracza po jego dotknięciu.
- **SlipperyTile**: Kafelek, który zwiększa prędkość graczy podczas chodzenia.
- **MudTile**: Kafelek, który zmniejsza prędkość graczy podczas chodzenia.
- **gemTile**: Znajdźka, po której znalezieniu czas przejścia poziomu skraca się.
- **button i receiver tile**: Receiver tile znika dopiero po przejściu przez button tile, co służy do blokowania przejść za którymi np. znajduje się znajdźka.

## System tworzenia poziomów

Jeden z elementów nad którym poświęiliśmy najwięcej czasu jest generator map. Stworzony przez nas ma możliwość bardzo szybkiego i przejrzystego edytowania map i dodawania nowych elementów.
Używamy do tego parsera pliku z poziomem, odczytując i  generując nowe tile krok po kroku. Wykorzystujemy do tego mapę par:

```cpp
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
}

```
Po którym następuje switch case w który możemy dowolnie dodawać elementy:

```cpp
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
```
## Miejsce startowe graczy
Gracze mogą się pojawić albo razem na jedyn tile'u, lub na osobnych dzięki czemu moża urozmaicić rozgrywkę

### Udźwiękowienie gry
W grze znajduje się tło muzyczne do całości rozgrywki, a także dźwięki śmierci i skoku, a także zebrania bonusowego diamentu.

### Tło poziomu
W grze występuje losowe tło poziomów

## GUI
W grze obecne jest GUI wykonane przy użyciu frameworku Qt. Składa się ono z :
- **ComboBox**: Służy do wyboru poziomów, wyświetla wszystkie poziomy, jednak te jeszcze przez gracza nieukończone są niedostępne i odblokowywują się po przejściu porpzedniego
- **Przyciski**: W UI znajdują się przyciski z instrujcą, przycisk do uruchamiania poziomu, i przycisk który zawiera informacje o autorach
- **Labele**: Wyświetlają automatycznie najlepszy czas z wybranego poziomu


### Zastosowanie sygnałów i slotów
W aplikacji zostały zastosowane sygały i sloty do obsługi aktualnie dostępnych poziomów
Sygnał jest emitowany podczas zamykania okna gry (game.cpp):

```cpp
Game::~Game(){
    emit gameClosed();
}
```
Następnie jest odbierany w mainwindow.cpp

```cpp
void MainWindow::on_runPushButton_clicked()
{
    //Odczyt obecnego aliasu na comboboxie
    QString selectedLevelAlias = ui->level_comboBox->currentText();
    //Odczyt daktycznych danych na comboxie
    QString selectedLevel = ui->level_comboBox->currentData().toString();
    //Konwersja na string aby uruchomic gre
    std::string stringSelectedLevel = selectedLevel.toStdString();

    //Uruchomienie gry
    // Game game(800,600);
    // game.selectLevel(stringSelectedLevel);
    // game.run();
    game = new Game(800,600);
    connect(game, SIGNAL(gameClosed()), this, SLOT(onGameClosed()));
    game->selectLevel(stringSelectedLevel);
    game->run();

    delete game;
}

void MainWindow::onGameClosed(){
    std::cerr<<"Odebrano"<<std::endl;
    levelSetup("assets/levels.txt");

}
```

Funckja levelSetup() odpowiada za wczytywanie obecnie dostępnych poziomów do comboboxa

```cpp
void MainWindow::levelSetup(const QString &levelFilePath){
    QFile file(levelFilePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this,"Error","Nie udalo sie otworzyc pliku z poziomami");
    }
    //Wczytanie ktore poziomy zostaly ukonczone
    QFile completedFile("assets/completed.txt");
    int completedLevels = 0;
    if(completedFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&completedFile);
        while(!in.atEnd()){
            in.readLine();
            completedLevels++;
        }
        completedFile.close();
    }

    QTextStream in(&file);
    int levelNumber =1;
    QStandardItemModel *model = new QStandardItemModel(this);
    while(!in.atEnd())
    {
        QString line = in.readLine();
        if(!line.isEmpty()){
            QString alias = QString("Level %1").arg(levelNumber);
            QStandardItem *item = new QStandardItem(alias);
            item->setData(line, Qt::UserRole);
            //Ustawienie na niedostępny, jeśli poziom nie został ukończony i to nie jest pierwszy poziom
            if(levelNumber > completedLevels + 1){
                item->setEnabled(false);
            }
            model->appendRow(item);
            levelNumber++;
        }
    }
    ui->level_comboBox->setModel(model);
}
```

