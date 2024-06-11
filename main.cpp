#include "game.h"

int main() {
    Game game(800, 600);
    game.selectLevel("/home/michal/QT_Apps/consoleOgienWoda/assets/level2.txt");
    game.run();
    return 0;
}
