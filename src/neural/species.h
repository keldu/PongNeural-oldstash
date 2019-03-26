#ifndef SPECIES_H
#define SPECIES_H

#include <vector>
#include <memory>

#include "genome.h"

class Species
{
    typedef std::vector<std::unique_ptr<Genome>> GenomeUVector;
    typedef std::vector<Genome> GenomeVector;
public:
    Species();

    void killWorstHalf();
    void killExceptBest();
    void copyEverything();
    int getSize();
    Genome& getGenome(int n);
    Genome createChild();
    void addGenome( Genome &member );

    float weight( Genome &intruder );
    float normal( Genome &intruder );

    bool sameSpecies( Genome &intruder );

    bool checkStaleness();
    void calculateAverage();
    float getAverageFitness();

private:
    GenomeVector mGenomes;

    int mStaleness;
    float mAverageFitness;
    int mSpeciesTopFitness;



};

#endif // SPECIES_H
