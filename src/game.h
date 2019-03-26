#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include <memory>
#include "world.h"

class Game
{
private:

    SDL_Window* m_window;
    SDL_Event* m_mainEvent;
    SDL_GLContext m_context;

    bool initGL();

    bool m_open;

    std::shared_ptr<World> m_world;

public:
    Game();

    void Loop();
    void Event();
    void Update();
    void Draw();

};

#endif // GAME_H
