#include "platform.h"

#include <glm/gtx/transform2.hpp>
#include "assetloader.h"

const float Platform::hX = 0.4f;
const float Platform::hY = 2.0f;
const float Platform::zLev = -0.5f;

Platform::Platform( World &world, b2Vec2 pos, vec4f color ):
    m_world(&world),
    m_color(color),
    mReflects(0)
{
    xBound = pos.x;
    b2BodyDef bodydef;
    bodydef.fixedRotation = true;
    bodydef.angle = 0;
    bodydef.angularVelocity = 0;
    bodydef.type = b2_kinematicBody;
    bodydef.position = pos;

    m_body = world.b2world.CreateBody( &bodydef );

    b2PrismaticJointDef prismdef;
    prismdef.bodyA = m_body;
    prismdef.bodyB = world.map->getBody();

    //m_joint = (b2PrismaticJoint*)(world.b2world.CreateJoint( &prismdef ));

    b2PolygonShape shape;
    shape.SetAsBox(hX, hY);

    b2FixtureDef fixdef;
    fixdef.shape = &shape;
    fixdef.density = 1;
    fixdef.isSensor = false;
    fixdef.friction = 0;
    fixdef.restitution = 1;
    fixdef.userData = this;

    m_fixture = m_body->CreateFixture( &fixdef );

    vertices.push_back( vec3f( -hX, -hY, zLev ) );
    vertices.push_back( vec3f(  hX, -hY, zLev ) );
    vertices.push_back( vec3f(  hX,  hY, zLev ) );
    vertices.push_back( vec3f( -hX, -hY, zLev ) );
    vertices.push_back( vec3f(  hX,  hY, zLev ) );
    vertices.push_back( vec3f( -hX,  hY, zLev ) );

    glGenBuffers(1, &vertexBufferID);
    glBindBuffer( GL_ARRAY_BUFFER, vertexBufferID );
    glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof( vec3f ), &vertices[0], GL_STATIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    programID = AssetLoader::LoadShadersVertFrag("./assets/Ball.vertexshader", "./assets/Ball.fragmentshader");

    mvpID = glGetUniformLocation( programID, "MVP" );
    colorID = glGetUniformLocation( programID, "iColor" );
}

void Platform::Draw(glm::mat4 &vp){

    float tx = m_body->GetPosition().x;
    float ty = m_body->GetPosition().y;

    glm::mat4 mvp = vp * glm::translate(vec3f( tx, ty, 0 ));

    glUseProgram( programID );

    glUniformMatrix4fv( mvpID, 1, GL_FALSE, &(mvp[0][0]) );
    glUniform4f( colorID, m_color.x, m_color.y, m_color.z, m_color.w);

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

void Platform::StartCollision(b2Contact *contact){

    b2Fixture *rival;

    if( contact->GetFixtureA() == m_fixture ){
        rival = contact->GetFixtureB();
    }else if(contact->GetFixtureB() == m_fixture ){
        rival = contact->GetFixtureA();
    }else{
        return;
    }


    if( ((IPongCollidable*)(rival->GetUserData()))->getType() == CollType::BALL ){
        b2Vec2 diff = rival->GetBody()->GetPosition() - m_body->GetPosition();
        rival->GetBody()->ApplyForceToCenter( b2Vec2( 0, diff.y * 90 ), true );
        mReflects++;
    }
}
#include <math.h>

int Platform::getReflects(){
    return mReflects;
}

void Platform::move( int state ){

    state = 1.0f * state;
    m_body->SetLinearVelocity( b2Vec2(0, speed * state ) );

}

void Platform::Update( float halfY ){
    if( m_body->GetPosition().y + hY >= halfY ){
        m_body->SetTransform( b2Vec2( m_body->GetPosition().x, halfY - hY ), 0 );
    }
    if( m_body->GetPosition().y - hY <= -halfY ){
        m_body->SetTransform( b2Vec2( m_body->GetPosition().x, -halfY + hY ), 0 );
    }
}

void Platform::reset(){
    m_body->SetTransform( b2Vec2( m_body->GetPosition().x, 0 ), 0 );
    mReflects = 0;
}
