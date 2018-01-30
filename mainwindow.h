#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QLineEdit>
#include <QLabel>
#include <QString>
#include <QPushButton>
#include "game.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QGraphicsScene * scene;

    void clearLetters();
    void clearHangPics();

private:
    Ui::MainWindow *ui;
    QLineEdit * text;
    QLabel * instr;
    QLabel * pics;
    QLabel * info;
    QLabel * wrongLetters;
    QPushButton * reset;
    QPixmap * mypix;
    QPixmap * stages;
    QFont f;
    QString word;
    QVector<QLabel *> label;
    QVector<QLabel *> letter;
    QString ch;
    game * hangman;
    char character;

public:
    void init();

signals:
    sendChar(char ch);

public slots:
    void setCharacter();
    void drawLines(int size);
    void drawLetters(QString word);
    void drawHangman(int stage);
    void setInfo(QString inf);
    void showWrong(QString wrong);
    void clearAll();
};

#endif // MAINWINDOW_H
