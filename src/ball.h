#ifndef BALL_H
#define BALL_H

#include <vector>
#include <Box2D/Box2D.h>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include "world.h"
#include "ipongcollidable.h"
#include "definitions.h"

class World;

class Ball : public IPongCollidable
{
private:
    const float zLev = -0.5f;
    float speed = 10.0f;
    b2Body *m_body;
    b2Fixture *m_fixture;
    World *m_world;

    GLuint programID, vertexBufferID, mvpID, colorID;
    std::vector<vec3f> vertices;

public:
    const float hSize = 0.25f;
    Ball( World &world );

    CollType getType(){ return CollType::BALL;}
    void StartCollision( b2Contact *contact );

    void reset( int dir );
    void push( b2Vec2 speed );

    b2Vec2 getPosition(){return m_body->GetPosition();}
    b2Vec2 getSpeed(){return m_body->GetLinearVelocity();}

    void Draw( glm::mat4 &vp );
};

#endif // BALL_H
