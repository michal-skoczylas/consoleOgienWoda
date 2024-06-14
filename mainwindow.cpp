#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game.h"
#include "QString"
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Zaladowanie poziomow do comboboxa
    levelSetup("assets/levels.txt");
}

MainWindow::~MainWindow() {
    delete ui;
}



void MainWindow::on_runPushButton_clicked()
{
    //Odczyt obecnego aliasu na comboboxie
    QString selectedLevelAlias = ui->level_comboBox->currentText();
    //Odczyt daktycznych danych na comboxie
    QString selectedLevel = ui->level_comboBox->currentData().toString();
    //Konwersja na string aby uruchomic gre
    std::string stringSelectedLevel = selectedLevel.toStdString();

    //Uruchomienie gry
    Game game(800,600);
    game.selectLevel(stringSelectedLevel);
    game.run();
}

void MainWindow::levelSetup(const QString &levelFilePath){
    QFile file(levelFilePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this,"Error","Nie udalo sie otworzyc pliku z poziomami");
    }

    QTextStream in(&file);
    int levelNumber =1;
    while(!in.atEnd())
    {
        QString line = in.readLine();
        if(!line.isEmpty()){
            QString alias = QString("Level %1").arg(levelNumber++);
            //To doda do comboboxa nazwe jako Level 1 a w rzeczywistiosci bedzie tam assets/level1.txt
            ui->level_comboBox->addItem(alias,line);
        }
    }
}

void MainWindow::on_authors_pushButton_clicked()
{
  //Okno z informacja o autorach
    QMessageBox msg;
    msg.setText("Autorzy: Michał Skoczylas, Jan Rębacz");
    msg.setWindowTitle("Autorzy projektu");
    msg.exec();
}

void MainWindow::on_instructions_pushButton_clicked()
{
    //Okno z instrukcją
    QMessageBox msg;
    msg.setText("Instrukcja: \n"
                "1. Wybierz poziom z listy rozwijanej\n"
                "2. Kliknij przycisk 'Uruchom'\n"
                "3. Sterowanie:\n"
                "   - Gracz 1: W - skok, A - ruch w lewo, D - ruch w prawo\n"
                "   - Gracz 2: Strzałka w górę - skok, Strzałka w lewo - ruch w lewo, Strzałka w prawo - ruch w prawo\n"
                "4. Celem gry jest dotarcie obu graczy do końca poziomu oznaczonego monetą, dodatkowo nieobligatoryjnym celem jest zdobycie przedmiotu diament o niewyobrażalnej mocy\n"
                "   - Gracz 1: Czerwony może stawać na blokach lawy, ale musi uważać na bloki wody, w których zgaśnie i pojawi się z powrotem na starcie\n"
                "   - Gracz 2: Niebieski może stawać na blokach wody, ale musi uważać na bloki ognia, w których zmieni się w parę i pojawi się z powrotem na starcie\n"
                "   - Żaden z Graczy nie może stawać na zielonym bloku kwasu, ponieważ rozpuści się i wróci na start\n"
                "5. Powodzenia!");
    msg.setWindowTitle("Instrukcja");
    msg.exec();
}


