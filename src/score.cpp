#include "score.h"

#include "assetloader.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>

Score::Score():
    displayScore(0)
{
    glGenBuffers( 1, &vertexBufferID );
    glGenBuffers( 1, &uvBufferID );

    programID = AssetLoader::LoadShadersVertFrag( "./assets/Score.vertexshader", "./assets/Score.fragmentshader" );
    textureID = AssetLoader::loadPNG( "./assets/Number.png" );

    mvpID = glGetUniformLocation( programID, "MVP" );
    samplerID = glGetUniformLocation( programID, "myTextureSampler" );
    uvAddID = glGetUniformLocation( programID, "uvAdd");

    float zLev = -0.2f;

    m_mesh.createSquare( vec3f( -0.4f, -0.51f, zLev ), vec3f( 0.8f, 0, 0 ), vec3f( 0, 1.02f, 0 ), vec2f( 0,0 ), vec2f(0.249, 0.249) );


    glBindBuffer( GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData( GL_ARRAY_BUFFER, m_mesh.vertices.size() * sizeof( vec3f ), &m_mesh.vertices[0], GL_STATIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, uvBufferID );
    glBufferData( GL_ARRAY_BUFFER, m_mesh.uvs.size() * sizeof( vec2f ), &m_mesh.uvs[0], GL_STATIC_DRAW ) ;

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void Score::Draw(const glm::mat4 &vp){

    vec2f uv = m_getUVPos( displayScore);

    glm::mat4 mvp = vp * glm::translate(vec3f( position.x, position.y, 0 ));

    glUseProgram( programID );

    glUniformMatrix4fv( mvpID, 1, GL_FALSE, &(mvp[0][0]) );
    glUniform2f( uvAddID, uv.x, uv.y );


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID );
    glUniform1i( samplerID , 0);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

    glVertexAttribPointer(
       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       3,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);

    glVertexAttribPointer(
       1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       2,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );
    // Draw the triangle !

    glDrawArrays( GL_TRIANGLES, 0 , m_mesh.vertices.size() ); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Score::setScore(int score){
    displayScore = score;
}


void Score::m_loadTexture(){

}

vec2f Score::m_getUVPos( int pos ){

    vec2f uv;

    uv.x = pos % 4;
    uv.y = 3 - pos / 4;

    uv = uv / 4.0f;

    return uv;
}
