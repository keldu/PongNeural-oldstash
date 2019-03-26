#include "world.h"
#include "aicontroller.h"

AIController::AIController( World &world ):
    Controller(world),
    state(-1),
    desiredY(0.0f)
{

}

void AIController::updateAction(){

    b2Vec2 pos = m_world->ball->getPosition();
    b2Vec2 speed = m_world->ball->getSpeed();

    switch(state){
    case -1:
        if( m_platform->getBody()->GetPosition().x * speed.x > 0 ){
            init_0( pos, speed );
            state = 0;
        }else{
            state = 1;
        }
        break;
    case 0:
        if( m_platform->getBody()->GetPosition().x * speed.x > 0 ){
            init_0(pos, speed);
            break;
        }else{
            desiredY = 0;
            state = 1;
        }

        break;
    case 1:
        if( m_platform->getBody()->GetPosition().x * speed.x > 0 ){
            init_0( pos, speed );
            state = 0;
        }else{
            state = 1;
        }

        break;
    }

    if( m_platform->getBody()->GetPosition().y > (desiredY + 0.2f) ){
        m_platform->move( -1 );
        return;
    }

    if( m_platform->getBody()->GetPosition().y < (desiredY - 0.2f ) ){
        m_platform->move( 1 );
        return;
    }

    m_platform->move(0);

}

void AIController::init_0(const b2Vec2& pos, const b2Vec2& speed){
    b2Vec2 platPos = m_platform->getBody()->GetPosition();

    float hhX = m_platform->hX;

    if( platPos.x > 0 ){
        hhX *= -1;
    }

    float t = ( m_platform->hX + platPos.x - pos.x ) / speed.x;

    if( t < 0){
        desiredY = 0;
        return;
    }

    float y = ( pos.y + t * speed.y );
    float size = m_world->map->hY - m_world->ball->hSize;
    while( fabs( y ) > size ){
        float modSize;
        if( y < 0 ){
            modSize = -size;
        }else{
            modSize = size;
        }

        y = modSize * 2 - y;
    }

    desiredY = y;
}

void AIController::endGame(bool won){

}
