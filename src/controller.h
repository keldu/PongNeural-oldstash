#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "world.h"
#include "platform.h"

class Platform;

class Controller
{
protected:

    Platform *m_platform;
    World *m_world;

public:
    Controller( World &world );

    virtual void givePlatform( Platform &olat );
    virtual void updateAction() = 0;
    virtual void endGame( bool won ) = 0;

    int points;
};

#endif // CONTROLLER_H
