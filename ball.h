#ifndef BALL_H
#define BALL_H

#include <QMainWindow>

namespace Ui {
    class Ball;
}

class Ball : public QPoint
{

public:
    int x;
    int y;

    Ball(int x, int y);
    ~Ball();
    void step(int x, int y);
    Ball *clone(int x, int y);
};

#endif // BALL_H
