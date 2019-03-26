#include "world.h"
#include "neuralcontroller.h"
#include "neuralinput.h"
#include <math.h>
#include <ctime>
#include <queue>

#include <iostream>
Neuralcontroller::Neuralcontroller(World &world):
    Controller( world ),
    mCurrentSpecies( 0 ),
    mCurrentGenome( 0 )
{
    generation = world.generation.get();
    s = world.species.get();
    g = world.genome.get();

    generation->setScore( mPool.generation );
    s->setScore( mCurrentSpecies );
    g->setScore( mCurrentGenome );

    srand( std::time(0L) );

    Genome spec = Genome::special();
    //mPool.addToSpecies( spec );

    for(int i = 0; i < Population; ++i){
        Genome gen = spec;
        gen.mutate();
        mPool.addToSpecies( gen );
    }

    mNetwork.createNetwork( mPool.getSpecies(mCurrentSpecies).getGenome(mCurrentGenome) );

}

void Neuralcontroller::updateAction(){

    const b2Vec2& platpos = m_platform->getBody()->GetPosition();
    const b2Vec2& ballpos = m_world->ball->getPosition();
    const b2Vec2& ballspeed = m_world->ball->getSpeed();

    bool left = (m_platform->getBody()->GetPosition().x < 0 );
    int flip = left?1:-1;

    NeuralInput input;
    input.ballplatPositionX = (platpos.x - ballpos.x) * flip;
    input.ballSpeedX = ballspeed.x * flip;

    input.ballPositionY = ballpos.y;
    input.ballSpeedY = ballspeed.y;
    input.platformPositionY = platpos.y;

    mNetwork.evaluate(input);

    int n = mNetwork.getSize();
    int state = 0;

        if( mNetwork.getOutputValue(0) > 0 ){
            ++state;
        }
        if( mNetwork.getOutputValue(1) > 0 ){
            --state;
        }

    m_platform->move(state);

}

void Neuralcontroller::endGame(bool won){
    float resultingFitness = mNetwork.getFitness();
    int resultDiff = abs(m_world->scoreLeft - m_world->scoreRight);

    if( won ){
        resultDiff *= -1;
    }

    resultingFitness += m_platform->getReflects() * 10;


    mPool.getSpecies( mCurrentSpecies ).getGenome( mCurrentGenome ).setFitness( resultingFitness );
    mNetwork.clearNetwork();
    mNextGenome();

    mNetwork.createNetwork( mPool.getSpecies( mCurrentSpecies ).getGenome( mCurrentGenome ) );

}

void Neuralcontroller::mNextGenome(){

    std::cout<<"Generation: "<<mPool.generation<<"\n";
    std::cout<<"Species:    "<<mCurrentSpecies<<"\n";
    std::cout<<"Genome:     "<<mCurrentGenome<<"\n";
    std::cout<<"Fitness:    "<<mPool.getSpecies( mCurrentSpecies ).getGenome( mCurrentGenome ).getFitness()<<"\n";
    std::cout<<std::endl;

    mCurrentGenome++;
    if( mCurrentGenome >= mPool.getSpecies( mCurrentSpecies ).getSize() ){
        mCurrentSpecies++;
        mCurrentGenome = 0;
        if( mCurrentSpecies >= mPool.getSize() ){
            mNextGeneration();
        }
    }
    generation->setScore( mPool.generation );
    s->setScore( mCurrentSpecies );
    g->setScore( mCurrentGenome );

    if( mPool.getSpecies( mCurrentSpecies ).getGenome( mCurrentGenome ).isAlreadyCalculated() ){
        mNextGenome();
    }
}

void Neuralcontroller::mNextGeneration(){
    std::cerr<<"\n------------------------------------------------------------------------------\n\n";

    mCurrentSpecies = 0;
    mCurrentGenome = 0;
    mPool.killWorstHalf();
    mPool.calculateGlobalRank();
    mPool.removeStaleSpecies();
    mPool.calculateGlobalRank();
    mPool.calculateAverage();
    mPool.removeWeakSpecies();

    std::queue<Genome> children;
    for(int i = 0; i < mPool.getSize(); ++i){
        Species &species = mPool.getSpecies(i);
        children.push( species.createChild() );
    }
    mPool.killExceptBest();
    while( children.size() + mPool.getSize() < Population ){
        int s = mPool.getSize();
        int n = rand() % s;
        children.push( mPool.getSpecies(n).createChild() );
    }

    while( !children.empty() ){
        Genome& genome = children.front();
        mPool.addToSpecies( genome );
        children.pop();
    }

    mPool.generation++;

}
