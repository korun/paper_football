#include "football_field.h"

// Конструктор
FootballField::FootballField()
{
    // Стартовая точка.
    FLD_POINT(0, 0) = new FPoint(0, 0);

    // Создаём ворота.
    FLD_POINT(-3, -25) = new FPoint(-3, -25, true);
    FLD_POINT( 3, -25) = new FPoint( 3, -25, true);
    FLD_POINT(-3,  25) = new FPoint(-3,  25, true);
    FLD_POINT( 3,  25) = new FPoint( 3,  25, true);

    // Создаём коробку (окружающие поле стены).
    for(int y = -26; y <= 26; y++){
        FLD_POINT(-20, y) = new FPoint(-20, y, true);
        FLD_POINT( 20, y) = new FPoint(20, y, true);
        if((y > -20 && y < -2) || (y > 2 && y < 20)){
            FLD_POINT(y,  26) = new FPoint(y, 26, true);
            FLD_POINT(y, -26) = new FPoint(y, -26, true);
        }
    }
}

// Деструктор
FootballField::~FootballField(){}
