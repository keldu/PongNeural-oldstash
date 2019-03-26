#include "world.h"

#include <glm/gtx/transform2.hpp>

World::World():
    b2world( b2Vec2(0,0) ),
    scoreLeft(0),
    scoreRight(0),
    m_mode(Mode::NOTHING),
    m_spc( new PongContactListener() )
{
    b2world.SetContactListener( m_spc.get() );

    float x = 20, y = 12;
    map = SMap( new Map(*this) );
    ball = SBall( new Ball(*this) );
    plat_left  = SPlatformer( new Platform(*this, b2Vec2( -x + Platform::hX, 0 ), vec4f( 0,1,0,1)) );
    plat_right = SPlatformer( new Platform(*this, b2Vec2( +x - Platform::hX, 0 ), vec4f( 1,0,1,1)) );

    score_left = SScore( new Score() );
    score_left->position = vec2f( 11, 14 );
    score_right = SScore( new Score( ) );
    score_right->position = vec2f( -11, 14 );

    generation = SScore( new Score( ) );
    generation->position = vec2f( 25, 14 );
    species = SScore( new Score( ) );
    species->position = vec2f( 25, 12 );
    genome = SScore( new Score( ) );
    genome->position = vec2f( 25, 10 );

    map->Load(x, y);
}

void World::Draw(  ){



    glm::mat4 stand = glm::ortho( -15.f * 16.0f / 9.0f, 15.f * 16.0f / 9.0f, -15.f, 15.f );

    glm::mat4 shift = stand * glm::translate( vec3f( 0, 0, 0 ) );

    map->Draw(shift);
    ball->Draw( shift );
    plat_left->Draw( shift );
    plat_right->Draw( shift );

    score_left->setScore( scoreLeft );
    score_left->Draw( stand );
    score_right->setScore( scoreRight );
    score_right->Draw( stand );

    generation->Draw( stand );
    species->Draw( stand );
    genome->Draw( stand );

}

void World::Input( SDL_Event &keyEvent ){

}

void World::Update(){


    if( m_mode == Mode::NOTHING){
        return;
    }

    controller_left->updateAction();
    controller_right->updateAction();
    map->Update();

    b2world.Step( 1.0f / 60.0f, 8, 6 );

    plat_left->Update(  map->hY );
    plat_right->Update( map->hY );

    wincheck();

}

void World::Load(std::__cxx11::string name){
    if( name == "Test" ){
        m_LoadTest();
        return;
    }

    if( name == "Main" ){
        m_LoadMain();
        return;
    }


}

void World::Load(Mode mode){

    ball->reset( 1 );

    switch (mode) {
    case Mode::AI:

        controller_left = SController( new AIController( *this ) );
        controller_left->givePlatform( *plat_left.get() );
        controller_right = SController( new AIController( *this ) );
        controller_right->givePlatform( *plat_right.get() );

        break;
    case Mode::NEURALVSAI:

        controller_left = SController( new AIController( *this ) );
        controller_left->givePlatform( *plat_left.get() );
        controller_right = SController( new Neuralcontroller( *this ) );
        controller_right->givePlatform( *plat_right.get() );

        break;
    default:
        break;
    }

    m_mode = mode;
}

void World::m_LoadMain(){

}

void World::m_LoadTest(){


}

void World::Unload(){

}

void World::increasePointsLeft(){

    ball->reset( -1 );

    scoreLeft += 1;
}

void World::increasePointsRight(){

    ball->reset( 1 );

    scoreRight+=1;

}
void World::wincheck(){
    if( scoreRight > maxScore){
        controller_left->endGame( false );
        controller_right->endGame( true );
        reset();
    }else if( scoreLeft > maxScore){
        controller_left->endGame( true );
        controller_right->endGame( false );
        reset();
    }
}

void World::reset(){
    scoreLeft  = 0;
    scoreRight = 0;

    ball->reset( 1 );
    plat_left->reset();
    plat_right->reset();

}
