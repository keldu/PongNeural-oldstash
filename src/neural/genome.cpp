#include "genome.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

Mutationchance::Mutationchance():
    mutationMutation( 1.0f ),
    linkMutation( 1.5f ),
    biasMutation( 0.5f ),
    pointMutation( 2.0f ),
    nodeMutation( 0.4f ),
    deactivationMutation(0.5f),
    activationMutation(0.5f)
{

}

void Mutationchance::internMutate(){
    float n = ( rand() % 100 ) / 100.0f;

    if( n >= mutationMutation ){
        return;
    }

    relMutate( mutationMutation );
    relMutate( pointMutation );
    relMutate( nodeMutation );
    relMutate( linkMutation );
    relMutate( biasMutation );
    relMutate( activationMutation );
    relMutate( deactivationMutation );
}

void Mutationchance::relMutate(float &relay){

    if( ( ( rand() % 100 ) / 100.0f )  < relay ){
        if( ( ( rand() % 100 ) / 100.0f ) < 0.5f ){
            relay *= 0.9f;
        }else{
            relay *= 1.111111f;
        }
    }

}

int Genome::mCurrentInnovation = 1;

Genome::Genome():
    mMaxNeurons(6),
    mGlobalRank(1),
    mFitness(1),
    mAlreadyCalculated(false)
{

}

Genome::Genome(const Genome &_gen):
    mMaxNeurons( _gen.mMaxNeurons ),
    mGlobalRank(1),
    mFitness(_gen.mFitness),
    genes(_gen.genes),
    mAlreadyCalculated(_gen.mAlreadyCalculated)
{

}

int Genome::getFitness(){
    return mFitness;
}

void Genome::setFitness(int fit){
    mFitness = fit;
    mAlreadyCalculated = true;
}

void Genome::setGlobalRank( int rank ){
    mGlobalRank = rank;
}

int Genome::getGlobalRank(){
    return mGlobalRank;
}

void Genome::mutate(){
    mFitness = 1;
    mAlreadyCalculated = false;
    float p;

    mChance.internMutate();
    p = mChance.pointMutation;
    if( ( rand() % 100 ) / 100.0f < p ){
        mPointMutate();
    }

    p = mChance.linkMutation;
    while( ( (rand()%100) / 100.0f ) < p ){
        mLinkMutate( false );
        p -= 1.0f;
    }

    p = mChance.biasMutation;
    while( ( (rand()%100) / 100.0f ) < p ){
        mLinkMutate( true );
        p -= 1.0f;
    }

    p = mChance.nodeMutation;
    while( ( (rand()%100) / 100.0f ) < p ){
        mNodeMutate();
        p -= 1.0f;
    }

    p = mChance.activationMutation;
    while( ( (rand()%100) / 100.0f ) < p ){
        mToggleMutate(true);
        p -= 1.0f;
    }

    p = mChance.deactivationMutation;
    while( ( (rand()%100) / 100.0f ) < p ){
        mToggleMutate(false);
        p -= 1.0f;
    }
}

void Genome::mLinkMutate( bool forcebias ){

    int neu1 = mGetRandomNeuron( true, false );
    int neu2 = mGetRandomNeuron( false, true );
    if( forcebias ){
        neu1 = mInputs - 1;
    }

    if( neu1 == neu2 ){
        return;
    }

    if( neu2 < neu1 ){
        int temp = neu2;
        neu2 = neu1;
        neu1 = temp;
    }

    if( mContainsLink( neu1, neu2 ) ){
        return;
    }

    Gen gen;
    gen.in  = neu1;
    gen.out = neu2;
    gen.weight = (rand() % 1000) / 500.0f - 4.0f;
    gen.active = true;
    gen.innovation = mCurrentInnovation++;
    genes.push_back( gen );

}

void Genome::mNodeMutate(){

    int n = genes.size();

    int element = rand() % n;

    if( !genes[element].active ){
        return;
    }
    genes[element].active = false;

    Gen genIn;
    genIn.active = true;
    genIn.in = genes[element].in;
    genIn.out = mMaxNeurons;
    genIn.weight = genes[element].weight;
    genIn.innovation = mCurrentInnovation++;

    genes.push_back( genIn );

    Gen genOut;
    genOut.active = true;
    genOut.in = mMaxNeurons;
    genOut.out = genes[element].out;
    genOut.weight = genes[element].weight;
    genOut.innovation = mCurrentInnovation++;

    genes.push_back( genOut );

    ++mMaxNeurons;
}

int Genome::mGetRandomNeuron(bool input, bool output){

    int number = mMaxNeurons - mInputs;
    int shift = 0;
    if( input ){
        number += mInputs;
    }else{
        shift += mInputs;
    }
    if( output ){
        number += mOutputs;
    }

    int element = rand() % number + shift;

    if( element >= mMaxNeurons ){
        element += mOutShift - mMaxNeurons;
    }

    return element;
}

void Genome::mPointMutate(){


    for( Gen &gen : genes ){
        float chance = ( rand() % 100 ) / 100.0f;
        if( chance < perturbChance ){
            gen.weight += ( rand() % 100 ) / 100.0f * gen.weight * StepSize * 2 - StepSize;
        }else{
            gen.weight = ( rand() % 100 ) / 25.0f - 2.0f;
        }
    }
}

void Genome::mToggleMutate(bool activate){

    std::vector< Gen* > cand;

    for( Gen &gen : genes ){
        if( gen.active != activate ){
            cand.push_back( &gen );
        }
    }

    if( cand.size() == 0 ){
        return;
    }
    int n = rand() % cand.size();

    Gen &man = *(cand[n]);

    man.active = !man.active;
}

bool Genome::mContainsLink(int into, int out){
    for( auto &iter : genes ){
        if( iter.in == into && iter.out == out ){
            return true;
        }
    }
    return false;
}

bool Genome::isAlreadyCalculated(){
    return mAlreadyCalculated;
}

Genome Genome::createBasicGenome(){
    Genome genome;
    genome.mutate();
    return genome;

}

Genome Genome::special(){
    Genome genome;

    Gen gen;
    gen.active = true;
    gen.in = 3;
    gen.out = mOutShift;
    gen.weight = 1.0f;
    gen.innovation = genome.mCurrentInnovation++;

    genome.genes.push_back( gen );

    gen.active = true;
    gen.in = 4;
    gen.out = mOutShift;
    gen.weight = -1.0f;
    gen.innovation = genome.mCurrentInnovation++;

    genome.genes.push_back( gen );

    gen.active = true;
    gen.in = 3;
    gen.out = mOutShift + 1 ;
    gen.weight = -1.0f;
    gen.innovation = genome.mCurrentInnovation++;

    genome.genes.push_back( gen );

    gen.active = true;
    gen.in = 4;
    gen.out = mOutShift + 1;
    gen.weight = 1.0f;
    gen.innovation = genome.mCurrentInnovation++;

    genome.genes.push_back( gen );

    return genome;
}
