#include "ball.h"

#include "assetloader.h"
#include <glm/gtx/transform2.hpp>

Ball::Ball( World &world ):
    m_world(&world)
{
    b2BodyDef bodydef;
    bodydef.fixedRotation = true;
    bodydef.angle = 0;
    bodydef.angularVelocity = 0;
    bodydef.bullet = true;
    bodydef.type = b2_dynamicBody;
    bodydef.position.SetZero();

    m_body = world.b2world.CreateBody( &bodydef );

    b2PolygonShape shape;
    shape.SetAsBox(hSize, hSize);

    b2CircleShape Cshape;
    Cshape.m_p.SetZero();
    Cshape.m_radius = hSize;

    b2FixtureDef fixdef;
    fixdef.shape = &shape;
    fixdef.density = 1;
    fixdef.isSensor = false;
    fixdef.friction = 0;
    fixdef.restitution = 0;
    fixdef.userData = this;

    m_fixture = m_body->CreateFixture( &fixdef );

    vertices.push_back( vec3f( -hSize, -hSize, zLev ) );
    vertices.push_back( vec3f(  hSize, -hSize, zLev ) );
    vertices.push_back( vec3f(  hSize,  hSize, zLev ) );
    vertices.push_back( vec3f( -hSize, -hSize, zLev ) );
    vertices.push_back( vec3f(  hSize,  hSize, zLev ) );
    vertices.push_back( vec3f( -hSize,  hSize, zLev ) );

    glGenBuffers(1, &vertexBufferID);
    glBindBuffer( GL_ARRAY_BUFFER, vertexBufferID );
    glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof( vec3f ), &vertices[0], GL_STATIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    programID = AssetLoader::LoadShadersVertFrag("./assets/Ball.vertexshader", "./assets/Ball.fragmentshader");

    mvpID = glGetUniformLocation( programID, "MVP" );
    colorID = glGetUniformLocation( programID, "iColor" );
}


void Ball::reset( int dir ){
    m_body->SetTransform( b2Vec2(1,1.0f), 0 );
    speed = 10.0f;

    b2Vec2 speedVec = speed * b2Vec2( dir ,0);

    m_body->SetLinearVelocity( speedVec );
}

void Ball::Draw(glm::mat4 &vp){

    glm::mat4 mvp = vp * glm::translate(vec3f( m_body->GetPosition().x, m_body->GetPosition().y, 0 ));

    glUseProgram( programID );

    glUniformMatrix4fv( mvpID, 1, GL_FALSE, &(mvp[0][0]) );
    glUniform4f( colorID, 0.5f, 0.2f, 0.2f, 1.0f);

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

    // Draw the triangle !

    glDrawArrays( GL_TRIANGLES, 0 , vertices.size() ); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Ball::StartCollision(b2Contact *contact){
    b2Fixture *rival;

    if( contact->GetFixtureA() == m_fixture ){
        rival = contact->GetFixtureB();
    }else if(contact->GetFixtureB() == m_fixture ){
        rival = contact->GetFixtureA();
    }else{
        return;
    }

    if( ((IPongCollidable*)(rival->GetUserData()))->getType() == CollType::PLATTFORM ){
        speed +=1.0f;
        b2Vec2 veS = m_body->GetLinearVelocity(  );
        veS.Normalize();
        m_body->SetLinearVelocity( speed * veS );
    }
}
