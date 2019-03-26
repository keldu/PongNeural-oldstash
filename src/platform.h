#ifndef PLATFORM_H
#define PLATFORM_H

#include <vector>
#include <Box2D/Box2D.h>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include "world.h"
#include "definitions.h"
#include "ipongcollidable.h"

class World;

class Platform : public IPongCollidable
{
private:

    const float speed = 10.0f;

    World *m_world;

    b2Body *m_body;
    b2Fixture *m_fixture;
    b2PrismaticJoint *m_joint;

    GLuint programID, vertexBufferID, mvpID, colorID;
    std::vector<vec3f> vertices;
    vec4f m_color;
    float xBound;
    int mReflects;



public:
    Platform( World &world, b2Vec2 pos, vec4f color );
    void Draw( glm::mat4 &vp );
    b2Body *getBody(){return m_body;}


    CollType getType(){ return CollType::PLATTFORM;}
    void StartCollision( b2Contact *contact );
    void move( int state );
    void reset();
    int getReflects();

    void Update( float halfY);

    static const float hX;
    static const float hY;
    static const float zLev;
};

#endif // PLATFORM_H
