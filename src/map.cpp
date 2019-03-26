#include "map.h"

#include "assetloader.h"

Map::Map( World &world ):
    m_world(&world),
    m_top(0L),
    m_bot(0L),
    m_sensorLeft(0L),
    m_sensorRight(0L)
{

    b2BodyDef bodydef;
    bodydef.type = b2_staticBody;
    bodydef.angle = 0;
    bodydef.position.SetZero();
    bodydef.fixedRotation = true;

    m_body = world.b2world.CreateBody( &bodydef );

    m_mesh.clear();
    glGenBuffers( 1, &vertexBufferID );
    glGenBuffers( 1, &uvBufferID );

    programID = AssetLoader::LoadShadersVertFrag( "./assets/Pong.vertexshader", "./assets/Pong.fragmentshader" );
    textureID = AssetLoader::loadPNG("./assets/Field.png");

    mvpID = glGetUniformLocation( programID, "MVP" );
    samplerID = glGetUniformLocation(programID, "myTextureSampler");


    glBindBuffer( GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData( GL_ARRAY_BUFFER, m_mesh.vertices.size() * sizeof( vec3f ), &m_mesh.vertices[0], GL_STATIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, uvBufferID );
    glBufferData( GL_ARRAY_BUFFER, m_mesh.uvs.size() * sizeof( vec2f ), &m_mesh.uvs[0], GL_STATIC_DRAW ) ;

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void Map::Clear(){

    if( wall[0].fixture ){
        m_body->DestroyFixture( wall[0].fixture );
        wall[0].fixture = 0L;
    }
    if( wall[1].fixture ){
        m_body->DestroyFixture( wall[1].fixture );
        wall[1].fixture = 0L;
    }
    if( sensor[0].fixture ){
        m_body->DestroyFixture( sensor[0].fixture );
        sensor[0].fixture = 0L;
    }
    if( sensor[1].fixture ){
        m_body->DestroyFixture( sensor[1].fixture );
        sensor[1].fixture = 0L;
    }

    m_mesh.clear();
}

void Map::Load(float hx, float hy){
    Clear();

    hX = hx;
    hY = hy;

    b2FixtureDef fixdef;
    fixdef.density = 1;
    fixdef.friction = 1;
    fixdef.isSensor = false;
    fixdef.restitution = 1;
    fixdef.userData = &wall[0];

    b2PolygonShape shape;
    //TOP
    shape.SetAsBox( hborderSize + hx, hborderSize, b2Vec2(0, hy + hborderSize), 0 );
    fixdef.shape = &shape;
    wall[0].fixture = m_body->CreateFixture( &fixdef );


    //BOT
    shape.SetAsBox( hborderSize + hx, hborderSize, b2Vec2(0, - hy - hborderSize), 0 );
    fixdef.shape = &shape;
    fixdef.userData = &wall[1];
    wall[1].fixture = m_body->CreateFixture( &fixdef );

    fixdef.isSensor = true;

    //LEFT
    shape.SetAsBox( hborderSize, hborderSize + hy, b2Vec2( -hx -hborderSize, 0 ),0 );
    fixdef.shape = &shape;
    fixdef.userData = &sensor[0];
    sensor[0].fixture = m_body->CreateFixture( &fixdef );


    //RIGHT
    shape.SetAsBox( hborderSize, hborderSize + hy, b2Vec2( hx + hborderSize, 0 ),0 );
    fixdef.shape = &shape;
    fixdef.userData = &sensor[1];
    sensor[1].fixture = m_body->CreateFixture( &fixdef );

    //DRAWING STUFF IN ANGRY MOOD, BECAUSE THE RENDERER IS ANGRY

    float zLev = -0.9f;

    m_mesh.createSquare( vec3f( -hx, -hy, zLev ), vec3f( hx - hmiddleLine, 0, 0 ), vec3f( 0, 2 * hy, 0 ), vec2f( 0.75, 0.75 ), vec2f( 0.249, 0.249 ) );
    m_mesh.createSquare( vec3f( hmiddleLine, -hy, zLev ), vec3f( hx - hmiddleLine, 0, 0 ), vec3f( 0, 2 * hy, 0 ), vec2f( 0.75, 0.75 ), vec2f( 0.249, 0.249 ) );

    //MIDDLE LINE
    const int occur = 7;
    float YStep = 2.0f * hy / (float)occur;
    float oldY = -hy;
    for(int i = 0; i < occur; ++i){

        float Y = YStep + oldY;
        m_mesh.createSquare( vec3f( -hmiddleLine, oldY, zLev ), vec3f( 2 * hmiddleLine, 0, 0 ), vec3f( 0, Y - oldY, 0 ), vec2f( 0.0, 0.0 ), vec2f( 0.249, 0.249 ) );

        oldY = Y;
    }

    const float verg = 3;
    const float verg_1 = verg - 1;

    m_mesh.createSquare( vec3f( -hx - hborderSize * verg, hy, zLev ), vec3f( 2 *( hx + hborderSize * verg ), 0, 0 ), vec3f( 0, hborderSize, 0 ), vec2f( 0.75, 0.0 ), vec2f( 0.249, 0.249 ) );
    m_mesh.createSquare( vec3f( -hx - hborderSize * verg, -hy -hborderSize, zLev ), vec3f( 2 *( hx + hborderSize * verg ), 0, 0 ), vec3f( 0, hborderSize, 0 ), vec2f( 0.75, 0.0 ), vec2f( 0.249, 0.249 ) );
    m_mesh.createSquare( vec3f( -hx - hborderSize * verg, -hy, zLev ), vec3f( ( hborderSize ), 0, 0 ), vec3f( 0, 2 * hy, 0 ), vec2f( 0.75, 0.0 ), vec2f( 0.249, 0.249 ) );
    m_mesh.createSquare( vec3f(  hx + hborderSize * verg_1, -hy, zLev ), vec3f( ( hborderSize ), 0, 0 ), vec3f( 0, 2 * hy, 0 ), vec2f( 0.75, 0.0 ), vec2f( 0.249, 0.249 ) );


    glBindBuffer( GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData( GL_ARRAY_BUFFER, m_mesh.vertices.size() * sizeof( vec3f ), &m_mesh.vertices[0], GL_STATIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, uvBufferID );
    glBufferData( GL_ARRAY_BUFFER, m_mesh.uvs.size() * sizeof( vec2f ), &m_mesh.uvs[0], GL_STATIC_DRAW ) ;

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void Map::Draw(glm::mat4 vp){

    glUseProgram( programID );

    glUniformMatrix4fv( mvpID, 1, GL_FALSE, &(vp[0][0]) );
    glActiveTexture(GL_TEXTURE0);
    GLuint samplerID = glGetUniformLocation( programID, "myTextureSampler" );
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
        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        2,                                // size : U+V => 2
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // Draw the triangle !

    glDrawArrays( GL_TRIANGLES, 0 , m_mesh.vertices.size() ); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

b2Body *Map::getBody(){
    return m_body;
}

void Map::Update(){
    if( sensor[0].detect() ){
        m_world->increasePointsLeft();
    }

    if( sensor[1].detect() ){
        m_world->increasePointsRight();
    }
}
