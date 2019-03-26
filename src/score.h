#ifndef SCORE_H
#define SCORE_H

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>

#include "mesh.h"

class Score
{
private:

    Mesh m_mesh;

    GLuint textureID, mvpID, samplerID, uvBufferID, vertexBufferID, programID, uvAddID;

    void m_loadTexture();

    vec2f m_getUVPos( int pos );

public:

    vec2f position;
    int displayScore;

    void Draw( const glm::mat4 &vp );
    void setScore( int score );

    Score();
};

#endif // SCORE_H
