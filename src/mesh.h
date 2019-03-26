#ifndef MESH_H
#define MESH_H

#include <vector>
#include "definitions.h"

class Mesh
{
public:
    Mesh();

    std::vector<vec3f> vertices;
    std::vector<vec2f> uvs;

    void createSquare( vec3f point, vec3f right, vec3f top, vec2f uvPos, vec2f uvSize );

    void clear();
};

#endif // MESH_H
