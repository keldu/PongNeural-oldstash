#include "mesh.h"

Mesh::Mesh()
{

}

void Mesh::clear(){
    vertices.clear();
    uvs.clear();
}

void Mesh::createSquare(vec3f point, vec3f right, vec3f top, vec2f uvPos, vec2f uvSize){

    vec2f uvx( uvSize.x, 0 );
    vec2f uvy( 0, uvSize.y );

    vertices.push_back( point );
    vertices.push_back( point + right );
    vertices.push_back( point + right + top );
    vertices.push_back( point );
    vertices.push_back( point + right + top );
    vertices.push_back( point + top );

    uvs.push_back( uvPos );
    uvs.push_back( uvPos + uvx );
    uvs.push_back( uvPos + uvx + uvy );
    uvs.push_back( uvPos );
    uvs.push_back( uvPos + uvx + uvy );
    uvs.push_back( uvPos + uvy );

}
