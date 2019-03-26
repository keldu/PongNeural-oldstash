#ifndef SENSOR_H
#define SENSOR_H

#include "ipongcollidable.h"

class Sensor : public IPongCollidable
{
private:
    bool hit;

public:
    Sensor();

    bool detect();

    CollType getType(){ return CollType::SENSOR;}
    void StartCollision( b2Contact *contact );

    b2Fixture *fixture;

};

#endif // SENSOR_H
