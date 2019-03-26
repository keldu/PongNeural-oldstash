#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H

#include "controller.h"

class KeyboardController: public Controller
{
public:
    KeyboardController( World &world );

    void updateAction();
};

#endif // KEYBOARDCONTROLLER_H
