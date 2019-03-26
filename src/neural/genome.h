#ifndef GENOME_H
#define GENOME_H

#include <vector>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include "neuraldefinitions.h"
#include "gen.h"

struct Mutationchance{
public:

    Mutationchance();
    float deactivationMutation;
    float activationMutation;

    float linkMutation;
    float biasMutation;

    float nodeMutation;
    float pointMutation;
    float mutationMutation;

    void internMutate();

private:

    void relMutate( float &relay );
};

class Genome
{
    typedef std::vector<Gen> GenVector;

public:
    Genome();
    Genome( const Genome& _gen );

    void setFitness( int fit );
    int getFitness();
    void setGlobalRank( int rank );
    int getGlobalRank();
    void mutate();
    bool isAlreadyCalculated();

    static Genome createBasicGenome();
    static Genome special();

    GenVector genes;
protected:
    int mFitness;
    int mMaxNeurons;
    int mGlobalRank;
    bool mAlreadyCalculated;
    Mutationchance mChance;

    void mLinkMutate(bool forcebias);
    void mNodeMutate();
    void mPointMutate();
    void mToggleMutate( bool activate );

    int mGetRandomNeuron(bool input, bool output);
    bool mContainsLink(int into, int out);

    static int mCurrentInnovation;

};

#endif // GENOME_H
