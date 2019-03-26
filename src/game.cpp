#include "game.h"


Game::Game():
    m_open(true)
{

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO);

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    m_window = SDL_CreateWindow("PongPing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 900, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if(m_window == NULL) {
        m_open = false;
        return;
    }

    m_context = SDL_GL_CreateContext( m_window );
    if( m_context == NULL ) {
        m_open = false;
        return;
    }

    glewExperimental = GL_TRUE;
    if( glewInit() != GLEW_OK ){
        m_open = false;
        return;
    }

    if( !initGL() ){
        m_open = false;
        return;
    }

    m_mainEvent = new SDL_Event();

    m_world = std::make_unique<World>();


    m_world->Load(Mode::NEURALVSAI);
}

bool Game::initGL(){

    glClearColor( 0.f, 0.f, 0.f, 1.f );
    glClear( GL_COLOR_BUFFER_BIT );

    //Create VAO and ignore it
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glEnable( GL_TEXTURE_2D );

    //EnableBlending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);


    return true;
}

void Game::Event(){
    while( SDL_PollEvent( m_mainEvent )  ){
        switch(m_mainEvent->type){
        case SDL_KEYDOWN:
            m_world->Input( *m_mainEvent );
            break;
        case SDL_KEYUP:
            m_world->Input( *m_mainEvent );
            break;
        case SDL_QUIT:
            m_open = false;
        default:
            break;
        }

    }
}

void Game::Loop(){


    while(m_open){

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        Event();
        Update();
        Draw();

        SDL_GL_SwapWindow( m_window );

    }

    SDL_Quit();

}

void Game::Update(){
    m_world->Update();
}

void Game::Draw(){
    m_world->Draw();
}
