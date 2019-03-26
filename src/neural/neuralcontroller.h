#ifndef NEURALCONTROLLER_H
#define NEURALCONTROLLER_H

#include "controller.h"
#include "network.h"
#include "pool.h"

class Neuralcontroller : public Controller
{
public:
    Neuralcontroller(World &world);

    virtual void updateAction();
    void endGame(bool won);
private:
    Network mNetwork;
    Pool mPool;

    int mCurrentSpecies, mCurrentGenome;

    void mNextGenome();
    void mNextGeneration();

    Score *generation, *s, *g;
};

#endif // NEURALCONTROLLER_H
