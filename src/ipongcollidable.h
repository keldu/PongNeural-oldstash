#ifndef IPONGCOLLIDABLE_H
#define IPONGCOLLIDABLE_H

#include <Box2D/Box2D.h>

enum class CollType{
    BALL,
    PLATTFORM,
    WALL,
    SENSOR
};

class IPongCollidable
{
public:
    IPongCollidable();

    virtual CollType getType() = 0;
    virtual void StartCollision( b2Contact *contact ) = 0;

};

#endif // IPONGCOLLIDABLE_H
