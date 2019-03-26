#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include "gen.h"

class Neuron
{
public:
    Neuron();

    float value;
    std::vector<Gen*> incoming;

};

#endif // NEURON_H
