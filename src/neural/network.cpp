#include "network.h"
#include <iostream>
#include <math.h>

Network::Network()
{

}

int Network::getSize(){
    return mNeurons.size();
}

int Network::getFitness(){
    return mCopy.getFitness();
}

void Network::evaluate(NeuralInput &input){

    mNeurons[0].value = input.ballplatPositionX;
    mNeurons[1].value = input.ballSpeedX;
    mNeurons[2].value = input.ballSpeedY;
    mNeurons[3].value = input.ballPositionY;
    mNeurons[4].value = input.platformPositionY;
    mNeurons[5].value = 1.f;

    int temp = input.ballPositionY - input.platformPositionY;
    int iDist = input.ballplatPositionX * input.ballplatPositionX + temp * temp + 1;

    int dist = floor( ( 1.0 / iDist )  * 100  );
    //mCopy.setFitness( mCopy.getFitness() + dist );

    for( auto &neu : mNeurons ){

            Neuron* neuron = &(neu.second);
            float sum = 0;
            for( int j = 0; j < neuron->incoming.size(); ++j ){

                sum += neuron->incoming[j]->weight * mNeurons[neuron->incoming[j]->in].value;
            }
            if( neuron->incoming.size() > 0 ){
                neuron->value = mSigmoid( sum );
            }

    }

}

Neuron* Network::getNeuron(int i){
    return &mNeurons.at(i);
}

void Network::clearNetwork(){
    mNeurons.clear();
}

void Network::createNetwork(Genome &genome){
    mCopy = genome;


    mNeurons.clear();

    for( int i = 0; i < mInputs; ++i ){
        mNeurons.insert( std::pair<int, Neuron> (i, Neuron()) );

    }

    for( int i = 0; i < mOutputs; ++i ){
        mNeurons.insert( std::pair<int, Neuron> (i + mOutShift, Neuron()) );

    }

    for( int i = 0; i < mCopy.genes.size(); ++i ){

        Gen* gen = &(mCopy.genes[i]);

        if( gen->active ){

            int in = gen->in;
            int out= gen->out;


            if( mNeurons.find( out ) == mNeurons.end() ){

                mNeurons.insert( std::pair<int, Neuron> ( out, Neuron() ) );
            }

            Neuron *neuronOut = &mNeurons[out];

            neuronOut->incoming.push_back( gen );

        }
    }

}

float Network::getOutputValue(int i){
    auto iter = mNeurons.find( mOutShift + i );
    if( iter == mNeurons.end() ){
        return -1;
    }

    return iter->second.value;
}

float Network::mSigmoid(float sum){

    return 2.0/( 1.0 + exp( -4.9 * sum ) ) - 1.0;
}

Neuron *Network::getRandomNeuron(bool input, bool output){

    std::vector<int> liste;

    for( auto &neuronPair : mNeurons ){
        if( input && neuronPair.first < mInputs ){
            liste.push_back( neuronPair.first );
        }

        if( neuronPair.first >= mInputs && neuronPair.first < mOutShift ){
            liste.push_back( neuronPair.first );
        }

        if( output && neuronPair.first >= mOutShift ){
            liste.push_back( neuronPair.first );
        }
    }

    int n = liste.size();

    int ele = rand() % n;

    return &(mNeurons[ liste[ele] ]);

}
