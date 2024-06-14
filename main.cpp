// #include "game.h"

// int main() {
//     Game game(800, 600);
//     game.selectLevel("assets/level3.txt");
//     game.run();
//     return 0;

// }
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  MainWindow window;
  window.setWindowTitle("Ogień  i woda");
  window.show();

  return app.exec();
}
