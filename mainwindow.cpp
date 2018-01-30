#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QIcon>

std::mt19937_64 generator(time(NULL));

int random(int min, int max);

inline void delay(int msecs)
{
    QTime dieTime = QTime::currentTime().addMSecs(msecs);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents( QEventLoop::ExcludeUserInputEvents, 100 );
}

const int SIZE = 10;

const string words[SIZE] = {"cytoplasm", "Feynman", "quarks", "ichthyostega",
                    "Noradrenaline", "phospholipids", "synapse", "Maxwell",
                    "Galaxy", "Andromeda"};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //tworzenie sceny
    ui->setupUi(this);
    init();

}

void MainWindow::drawLetters(QString word)
{
    clearLetters();
    int i = 0;
    int size = word.length();
    int beg;
    if (size <= 6)
        beg = 300;
    else if (size > 6 && size <= 9)
        beg = 200;
    else
        beg = 100;
    for (int x = beg; x < beg + word.size() * 50; x += 50, i++)
    {
        letter.push_back(new QLabel(0));
        scene->addWidget(letter[i]);
        letter[i]->setGeometry(QRect(x, 0, 50, 50));
        letter[i]->setAlignment(Qt::AlignCenter);
        letter[i]->setFrameStyle(QFrame::NoFrame);
        letter[i]->setStyleSheet("QLabel { color : lightgray; background-color: transparent; }");
        letter[i]->setText((QString)word[i]);
        letter[i]->setFont(f);
    }
    update();
}

void MainWindow::drawLines(int size)
{
    int i = 0;
    int beg;
    if (size <= 6)
        beg = 300;
    else if (size > 6 && size <= 9)
        beg = 200;
    else
        beg = 100;
    for (int x = beg; x < beg + size * 50; x += 50, i++)
    {
        label.push_back(new QLabel(0));
        scene->addWidget(label[i]);
        label[i]->setPixmap(mypix->scaled(50, 10, Qt::KeepAspectRatio));
        label[i]->setStyleSheet("QLabel { background-color: transparent; }");
        label[i]->setGeometry(QRect(x, 40, 50, 50));
        label[i]->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
        label[i]->setAlignment(Qt::AlignCenter);
    }
    //pozycja instrukcji i okna wpisywania liter
    instr->setGeometry(QRect(beg + size * 50 + 100, 0, 250, 100));
    text->setGeometry(QRect(beg + size * 50 + 180, 50, 100, 100));

    //pozycja hangmena
    pics = new QLabel(0);
    pics->setPixmap(stages->scaled(250, 250, Qt::KeepAspectRatio));
    pics->setGeometry(QRect(850, 200, 200, 200));
    pics->setStyleSheet("QLabel { background-color: transparent; }");
    scene->addWidget(pics);
}

void MainWindow::drawHangman(int stage)
{
    int xpos = 850;
    int ypos = 200;
    QString st = ":/stage";
    delete stages;
    delete pics;
    switch(stage)
    {
    case 1:
        st.append("1.png");
        break;
    case 2:
        st.append("2.png");
        break;
    case 3:
        st.append("3.png");
        break;
    case 4:
        st.append("4.png");
        break;
    case 5:
        st.append("5.png");
        break;
    case 6:
        st.append("6.png");
        break;
    }
    stages = new QPixmap(st);
    pics = new QLabel(0);
    pics->setPixmap(stages->scaled(250, 250, Qt::KeepAspectRatio));
    pics->setGeometry(QRect(xpos, ypos, 250, 250));
    pics->setStyleSheet("QLabel { background-color: transparent; }");
    scene->addWidget(pics);
    update();
}

void MainWindow::setInfo(QString inf)
{
    info->setText(inf);
    delay(1000);
    info->setText("");
}

void MainWindow::showWrong(QString wrong)
{
    wrongLetters->setText(wrong);
}

void MainWindow::clearAll()
{
    delete text;
    delete instr;
    delete pics;
    delete info;
    delete wrongLetters;
    //delete reset;
    delete mypix;
    delete stages;
    delete hangman;
    for (auto x : letter)
        delete x;
    letter.erase(letter.begin(), letter.end());
    for (auto x : label)
        delete x;
    label.erase(label.begin(), label.end());

    init();

}

void MainWindow::clearLetters()
{
   for (auto x : letter)
       delete x;
   letter.erase(letter.begin(), letter.end());
}

void MainWindow::init()
{
    scene = new QGraphicsScene(this);
    text = new QLineEdit();
    instr = new QLabel();
    mypix = new QPixmap(":/Chalk line.PNG");
    stages = new QPixmap(":/stage0.png");
    info = new QLabel();
    wrongLetters = new QLabel();
    reset = new QPushButton();
    ui->graphicsView->setScene(scene);
    scene->addEllipse(0, 0, 1, 1);  //punkt odniesienia
    //QMainWindow::showFullScreen();
    //inicjalizacja okna wpisywania literek
    f = QFont("Papyrus", 30, QFont::Bold);
    text->setStyleSheet("background-color: transparent; color : white; font: 18pt Papyrus; border: none;");
    text->setAlignment(Qt::AlignCenter);
    scene->addWidget(text);

    scene->addWidget(instr);
    instr->setText("Put character below");
    instr->setStyleSheet("background-color: transparent; color : white; font: 18pt Papyrus;");
    instr->setAlignment(Qt::AlignCenter);

    //tworzenie obietu game
    hangman = new game(words[random(0, SIZE - 1)]);

    //tworzenie linii pod literkami
    drawLines(hangman->getLen());

    //inicjalizacja połączenia wprowadzenia litery i początku gry
    connect(text, QLineEdit::returnPressed, this, MainWindow::setCharacter);
    connect(text, QLineEdit::returnPressed, hangman, game::play);
    connect(this, SIGNAL(sendChar(char)), hangman, SLOT(getCharacter(char)));
    connect(hangman, game::outputShot, this, MainWindow::drawLetters);

    //połączenie między game a rysowaniem hangmana
    connect(hangman, SIGNAL(sendStage(int)), this, SLOT(drawHangman(int)));

    //inicjalizacja i połączenie okien informacji dodatkowych
    connect(hangman, SIGNAL(sendInfo(QString)), this, SLOT(setInfo(QString)));
    connect(hangman, SIGNAL(sendWrong(QString)), this, SLOT(showWrong(QString)));

    QFont g("Papyrus", 20, QFont::Bold);
    info->setGeometry(500, -300, 700, 100);
    info->setAlignment(Qt::AlignCenter);
    info->setFrameStyle(QFrame::NoFrame);
    info->setStyleSheet("QLabel { color : lightgray; background-color: transparent; }");
    info->setFont(g);
    scene->addWidget(info);

    wrongLetters->setGeometry(100, 300, 700, 100);
    wrongLetters->setFrameStyle(QFrame::NoFrame);
    wrongLetters->setStyleSheet("QLabel { color : lightgray; background-color: transparent; }");
    wrongLetters->setFont(g);
    scene->addWidget(wrongLetters);
    wrongLetters->setText("Wrong letters: ");

    //tworzenie przycisku reset
    reset->setGeometry(0, -250, 400, 150);
    reset->setStyleSheet("QPushButton { color : lightgray; background-color: transparent; }");
    reset->setIcon(QIcon(":/sponge.png"));
    reset->setText("New Game");
    reset->setFont(g);
    reset->setIconSize(QSize(150, 150));
    scene->addWidget(reset);
    connect(reset, SIGNAL(pressed()), this, SLOT(clearAll()));
}

MainWindow::~MainWindow()
{
    delete text;
    delete instr;
    delete pics;
    delete info;
    delete wrongLetters;
    delete reset;
    delete mypix;
    delete stages;
    delete hangman;
    for (auto x : letter)
        delete x;
    letter.erase(letter.begin(), letter.end());
    for (auto x : label)
        delete x;
    label.erase(label.begin(), label.end());
    delete scene;
}

void MainWindow::setCharacter()
{
    QString str(text->text());
    character = str.toStdString()[0];
    emit sendChar(character);
}

int random(int min, int max)
{
    std::uniform_int_distribution<> distribution(min, max);
    return distribution(generator);
}
