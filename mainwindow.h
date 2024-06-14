#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void levelSetup(const QString &levelFilePath);

private slots:

    void on_runPushButton_clicked();

    void on_authors_pushButton_clicked();

    void on_instructions_pushButton_clicked();

    void on_authors_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
