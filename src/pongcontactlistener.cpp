#include "pongcontactlistener.h"

#include "ipongcollidable.h"

PongContactListener::PongContactListener()
{

}


void PongContactListener::BeginContact(b2Contact *contact){


}

void PongContactListener::EndContact(b2Contact *contact){
    if( contact->GetFixtureA()->GetUserData() != 0L && contact->GetFixtureB()->GetUserData() != 0L ){

        ((IPongCollidable*)( contact->GetFixtureA()->GetUserData() ))->StartCollision( contact );
        ((IPongCollidable*)( contact->GetFixtureB()->GetUserData() ))->StartCollision( contact );
    }
}
