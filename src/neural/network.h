#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <map>

#include "neuraldefinitions.h"
#include "neuron.h"
#include "neuralinput.h"
#include "genome.h"

class Network
{
public:

    Network();

    void clearNetwork();
    void createNetwork( Genome &genome );
    int getSize();
    void evaluate( NeuralInput& Input );
    Neuron *getRandomNeuron( bool input, bool output );
    int getFitness();

    float getOutputValue(int i);

    Neuron* getNeuron( int i );
private:

    std::map<int,Neuron> mNeurons;
    Genome mCopy;

    float mSigmoid( float sum);


};

#endif // NETWORK_H
