#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <string>

using std::string;

class game : public QObject
{
    Q_OBJECT
private:
    string word;
    string shots;
    string wrong;
    int length;
    int attempts = 0;
    char letter;
public:
    explicit game(QObject *parent = nullptr);
    game(string);
    int getLen() { return length; }
signals:
    void outputShot(QString sh);
    void sendStage(int stage);
    void sendWrong(QString wr);
    void sendInfo(QString inf);
public slots:
    void play();
    void getCharacter(char ch);
};

#endif // GAME_H
