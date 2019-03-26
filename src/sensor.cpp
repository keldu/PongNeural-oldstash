#include "sensor.h"

Sensor::Sensor():
    fixture(0L),
    hit(false)
{

}

void Sensor::StartCollision(b2Contact *contact){
    b2Fixture *rival;

    if( contact->GetFixtureA() == fixture ){
        rival = contact->GetFixtureB();
    }else if(contact->GetFixtureB() == fixture ){
        rival = contact->GetFixtureA();
    }else{
        return;
    }


    if( ((IPongCollidable*)(rival->GetUserData()))->getType() == CollType::BALL ){
        hit = true;

    }
}

bool Sensor::detect(){
    if( hit ){
        hit = false;
        return true;
    }
    return false;
}
