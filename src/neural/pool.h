#ifndef POOL_H
#define POOL_H

#include <vector>
#include "species.h"

class Pool
{
    typedef std::vector<Species> SpeciesVector;

public:
    Pool();

    void createSpeciesWith(Genome &genome);
    Species& getSpecies( int n );
    void killWorstHalf();
    void killExceptBest();
    void removeStaleSpecies();
    void calculateGlobalRank();
    void calculateAverage();
    void removeWeakSpecies();
    void addToSpecies( Genome &genome );
    int getSize();

    int generation;

private:
    SpeciesVector mSpecies;

    float mTotalAverageFitness();

    int mTopFitness;

};

#endif // POOL_H
