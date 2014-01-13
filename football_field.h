#ifndef FFIELD_H
#define FFIELD_H

#include "fpoint.h"
#include "ball.h"

#define PX_SCALE     15
#define FIELD_WIDTH  600
#define FIELD_HEIGHT 780
#define PW_CENTER    (FIELD_WIDTH  / PX_SCALE / 2)
#define PH_CENTER    (FIELD_HEIGHT / PX_SCALE / 2)
#define FLD_POINT(X, Y) (points[PW_CENTER + (X)][PH_CENTER + (Y)])

class FootballField
{

public:
    const signed char KEYS[10][2] = {
        { 0,  0}, // 0 - unused
        { 1, -1}, // 1
        { 0, -1}, // 2
        {-1, -1}, // 3
        { 1,  0}, // 4
        { 0,  0}, // 5 - unused
        {-1,  0}, // 6
        { 1,  1}, // 7
        { 0,  1}, // 8
        {-1,  1}  // 9
    };

    FPoint *points[FIELD_WIDTH / PX_SCALE + 1][FIELD_HEIGHT / PX_SCALE + 1]; /* [x, y] */
    std::vector<signed char> steps;
    bool show_ball = true;

    FootballField();
    ~FootballField();
    bool try_step(int key);

protected:
    Ball ball;

private:
    bool try_penalty(int key);
    bool can_step(signed char x, signed char y);
    bool diagstep(signed char x, signed char y);
    bool can_diagstep(signed char bx, signed char by, signed char ex, signed char ey);
    bool can_move_1x();
    bool can_move_3x();
    bool penalty_kick();
    signed char   current_step = 1;
    unsigned char current_player;
    bool          penalty_mode = false;
    bool          gameover     = false;
};

#endif // FFIELD_H
