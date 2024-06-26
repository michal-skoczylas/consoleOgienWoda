#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QString>
#include "game.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void levelSetup(const QString &levelFilePath);
    QString findShortestTime(const QString &TimeFilePath);
signals:
    void gameClosed();
public slots:
    void onGameClosed();

private slots:
    // Funkcje obsługujące przyciski
    void on_runPushButton_clicked();
    
    void on_authors_pushButton_clicked();

    void on_instructions_pushButton_clicked();

    void on_level_comboBox_currentTextChanged(const QString &arg1);


private:
    Ui::MainWindow *ui;
    Game *game;
};

#endif // MAINWINDOW_H
