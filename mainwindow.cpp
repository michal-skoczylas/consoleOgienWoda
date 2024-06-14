#include <QStandardItemModel>
#include <QStandardItem>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game.h"
#include "QString"
#include <QMessageBox>
#include <iostream>
#include <QRegularExpression>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Zaladowanie poziomow do comboboxa
    levelSetup("assets/levels.txt");
    QIcon icon("assets/logo.png");
    setWindowIcon(icon);
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
    // Game game(800,600);
    // game.selectLevel(stringSelectedLevel);
    // game.run();
    game = new Game(800,600);
    connect(game, SIGNAL(gameClosed()), this, SLOT(onGameClosed()));
    game->selectLevel(stringSelectedLevel);
    game->run();

    delete game;
}

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
                "   - Gracze mogą być spowalniani lub przyśpieszani na blokach śliskich i z błotem\n"
                "5. Powodzenia!");
    msg.setWindowTitle("Instrukcja");
    msg.exec();
}


//
QString MainWindow::findShortestTime(const QString &TimeFilePath){
    QFile file(TimeFilePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text )){
        return "Blad podczas otwierania plikow z czasami";
    }

    QTextStream in(&file);
    QString shortestTime;

    int shortestTimeMs = std::numeric_limits<int>::max();
    //Parsowanie linijek pliku z czasami
    while(!in.atEnd()){
        QString line = in.readLine();
        QStringList timeParts = line.split(':');
        if(timeParts.size() == 3){
            int minutes = timeParts[0].toInt();
            int seconds = timeParts[1].toInt();
            int miliseconds = timeParts[2].toInt();

        //Dodanie wszystkich czasow zeby wyznaczyc czas laczny i porownac z innymi (milisekundy)
            int totalMs = (minutes * 60*1000) + (seconds *1000) + miliseconds;

            if(totalMs< shortestTimeMs){
                shortestTimeMs = totalMs;
                shortestTime =line;
            }
        }
    }
    file.close();
    return shortestTime;

}

void MainWindow::on_level_comboBox_currentTextChanged(const QString &arg1)
{
    //Indeks obecnego elementu
    int index = ui->level_comboBox->currentIndex();
    if(index<0) return;


    //Pobranie sciezki plkiu z poziomem na podstawie podstawie obecnego aliasu
    QString levelFilePath = ui->level_comboBox->itemData(index).toString();

    //Zmiana level1.txt na time1.txt np przy uzyciu regexa
    QRegularExpression re("level(\\d+)\\.txt");
    QRegularExpressionMatch match = re.match(levelFilePath);
    if(!match.hasMatch()){
        ui->best_time_label->setText("Nie udalo sie odczytac pliku");
        return;
    }
    QString levelNumber = match.captured(1);

    //Tworzenie sciezki do pliku z czasami na podstawie numeru poziomu
    QString timeFilePath = QString("assets/Time%1.txt").arg(levelNumber);

    //Uzycie funkcji w celu znalezienia najkrotszego czasu zeby znalesc i wyswietlic najelpszy czas dla danego poziomu
    QString shortestTime = findShortestTime(timeFilePath);

    //Wysweitlenie czasu w labelu
    ui->label->setText(shortestTime);

}


void MainWindow::on_pushButton_clicked()
{
    levelSetup("assets/levels.txt");

}

void MainWindow::onGameClosed(){
    std::cerr<<"Odebrano"<<std::endl;
    levelSetup("assets/levels.txt");

}
