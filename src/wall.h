#ifndef WALL_H
#define WALL_H

#include "ipongcollidable.h"

class Wall : public IPongCollidable
{
public:
    Wall();

    CollType getType() {return CollType::WALL;}
    void StartCollision( b2Contact *contact );

    b2Fixture *fixture;

};

#endif // WALL_H
