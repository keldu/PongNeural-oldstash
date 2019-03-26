#ifndef MAP_H
#define MAP_H

#include <vector>
#include <Box2D/Box2D.h>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include <glm/glm.hpp>

#include "world.h"
#include "mesh.h"

#include "sensor.h"
#include "wall.h"

class World;

typedef std::shared_ptr<Sensor> SSensor;
typedef std::shared_ptr<Wall> SWall;

class Map
{
private:

    const float hborderSize = 0.25f;
    const float hmiddleLine = 0.02f;

    //Graphics
    GLuint programID, textureID, vertexBufferID, uvBufferID, samplerID, mvpID;
    Mesh m_mesh;

    //Physics
    b2Body *m_body;
    b2Fixture *m_top, *m_bot, *m_sensorLeft, *m_sensorRight;

    World *m_world;

    Wall wall[2];
    Sensor sensor[2];

    void Clear();



public:
    Map( World &world );

    //Halfsize
    void Load( float hx, float hy );
    void Draw( glm::mat4 vp );

    void Update();

    float hX, hY;

    b2Body *getBody();
};

#endif // MAP_H
