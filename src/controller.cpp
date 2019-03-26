#include "world.h"
#include "controller.h"

Controller::Controller( World &world ):
    m_world(&world),
    points(0)
{

}

void Controller::givePlatform(Platform &olat){
    m_platform = &olat;
}
