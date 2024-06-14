#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QString>

namespace Ui
{
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void levelSetup(const QString &levelFilePath);
  QString findShortestTime(const QString &TimeFilePath);

private slots:
  // Funkcje obsługujące przyciski
  void on_runPushButton_clicked();

  void on_authors_pushButton_clicked();

  void on_instructions_pushButton_clicked();

  void on_level_comboBox_currentTextChanged(const QString &arg1);

  void on_pushButton_clicked();

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
