#ifndef WORLD_H
#define WORLD_H

#include <SDL2/SDL.h>
#include <Box2D/Box2D.h>
#include <memory>

#include "map.h"
#include "controller.h"
#include "platform.h"
#include "ball.h"
#include "pongcontactlistener.h"
#include "score.h"
#include "aicontroller.h"
#include "neural/neuralcontroller.h"

class Map;
class Platform;
class Ball;
class Controller;

typedef std::shared_ptr<Map> SMap;
typedef std::shared_ptr<Platform> SPlatformer;
typedef std::shared_ptr<Controller> SController;
typedef std::shared_ptr<Ball> SBall;
typedef std::shared_ptr<PongContactListener> SPC;
typedef std::shared_ptr<Score> SScore;

enum class Mode{
    NOTHING,
    AI,
    NEURALVSAI
};

class World
{
private:

    //Loads MainMenu
    void m_LoadMain();
    //Loads TestWorld
    void m_LoadTest();

    Mode m_mode;
    SPC m_spc;

public:

    const int maxScore = 0;

    World();

    void Update();
    void Input( SDL_Event &keyEvent );
    void Draw();

    void increasePointsLeft();
    void increasePointsRight();

    //Loads Different WOrlds
    void Load( std::string name );
    void Load( Mode mode );
    void Unload();

    SMap map;
    SBall ball;
    SPlatformer plat_left, plat_right;
    SController controller_left, controller_right;
    SScore score_left, score_right;

    SScore generation, species, genome;

    int scoreLeft, scoreRight;
    void wincheck();
    void reset();


    b2World b2world;
};

#endif // WORLD_H
