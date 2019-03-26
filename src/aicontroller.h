#ifndef AICONTROLLER_H
#define AICONTROLLER_H

#include "controller.h"

class AIController : public Controller
{
private:

    int state;
    float desiredY;

    void init_0( const b2Vec2& pos, const b2Vec2& speed );
public:
    AIController( World &world );

    void updateAction();
    void endGame(bool won);

};

#endif // AICONTROLLER_H
