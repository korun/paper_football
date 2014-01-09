#ifndef FFIELD_H
#define FFIELD_H

#include "fpoint.h"
#include "ball.h"

#define PX_SCALE     15
#define FIELD_WIDTH  600
#define FIELD_HEIGHT 780
#define PW_CENTER    (FIELD_WIDTH  / PX_SCALE / 2 + 1)
#define PH_CENTER    (FIELD_HEIGHT / PX_SCALE / 2 - 1)
#define FLD_POINT(X, Y) (points[PW_CENTER + (X)][PH_CENTER + (Y)])

class FootballField
{

public:
    //const int GATE1[5][2] = {{2, -26}, {1, -26}, {0, -26}, {-1, -26}, {-2, -26}};
    //const int GATE2[5][2] = {{2, 26}, {1, 26}, {0, 26}, {-1, 26}, {-2, 26}};
    FPoint *points[FIELD_WIDTH / PX_SCALE][FIELD_HEIGHT / PX_SCALE]; /* [x, y] */
    std::vector<signed char> steps;

    FootballField();
    ~FootballField();
};

#endif // FFIELD_H
