#include "game.h"
#include <QDebug>

game::game(QObject *parent) : QObject(parent) { }

game::game(string s) : word(s),  shots(s.size(), ' '), wrong(" ")
{
    length = s.size();
}

void game::play()
{
    if (attempts < 6 && shots != word)
    {
        unsigned int loc = word.find(letter);
        if (wrong.find(letter) != string::npos || shots.find(letter) != string::npos) //litera juz byla
            emit sendInfo("You already pick this letter...");
        else if (loc == string::npos)
        {
            attempts++;
            emit sendStage(attempts);
            wrong += letter;
        }
        else
        {
            shots[loc] = letter;
            loc = word.find(letter, loc + 1);
            while (loc != string::npos)
            {
                shots[loc] = letter;
                loc = word.find(letter, loc + 1);
            }
            emit outputShot(QString::fromStdString(shots));
        }

        if (shots != word)
        {
            QString s = "Wrong letters: ";
            s += QString::fromStdString(wrong);
            emit sendWrong(s);
        }
    }
}

void game::getCharacter(char ch)
{
    letter = ch;
}
