#ifndef PONGCONTACTLISTENER_H
#define PONGCONTACTLISTENER_H

#include <Box2D/Box2D.h>

class PongContactListener : public b2ContactListener
{
public:
    PongContactListener();

    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
};

#endif // PONGCONTACTLISTENER_H
