#ifndef ASSETLOADER_H
#define ASSETLOADER_H

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

class AssetLoader
{
private:
    AssetLoader();

public:

    static GLuint loadPNG( const char *png_file_path );
    static GLuint LoadShadersVertFrag(const char * vertex_file_path,const char * fragment_file_path);

};

#endif // ASSETLOADER_H
