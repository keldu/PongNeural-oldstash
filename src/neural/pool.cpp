#include "pool.h"
#include <algorithm>

#include <iostream>

Pool::Pool():
    generation(0)
{

}

void Pool::createSpeciesWith( Genome &genome ){
    mSpecies.push_back( Species() );
    mSpecies.back().addGenome( genome );
}

Species& Pool::getSpecies(int n){
    return mSpecies[n];
}

int Pool::getSize(){
    return mSpecies.size();
}

void Pool::killWorstHalf(){
    for( auto &iter : mSpecies ){
        iter.killWorstHalf();
    }
}

void Pool::killExceptBest(){
    for( auto &iter : mSpecies ){
        iter.killExceptBest();
    }
}

void Pool::removeStaleSpecies(){
    auto temp = mSpecies;
    mSpecies.clear();

    for( auto &iter : temp ){

        if( !iter.checkStaleness() ){
            mSpecies.push_back( iter );
        }

    }
}

void Pool::calculateGlobalRank(){
    std::vector<Genome*> globals;

    for( int i = 0; i < mSpecies.size(); ++i){
        Species &local = mSpecies[i];
        for( int j = 0; j < local.getSize(); ++j ){
            Genome &genes = (local.getGenome(j));
            globals.push_back( &genes );
        }
    }

    std::sort( globals.begin(), globals.end(), [](Genome *a, Genome *b){ return a->getFitness() < b->getFitness(); } );
    for(int i = 0; i < globals.size(); ++i){
        Genome &genes = *(globals[i]);
        genes.setGlobalRank( i + 1 );
    }

}

void Pool::calculateAverage(){
    for( Species &iter : mSpecies ){
        iter.calculateAverage();
    }
}

void Pool::removeWeakSpecies(){
    auto temp = mSpecies;
    float sum = mTotalAverageFitness();
    mSpecies.clear();
    int n = temp.size();
    for( Species& iter : temp ){
        float breed = ( iter.getAverageFitness() / sum  ) * n;
        if( breed >= 1 ){
            mSpecies.push_back( iter );
        }
    }
}

float Pool::mTotalAverageFitness(){
    float total = 0;

    for( Species &iter : mSpecies ){
        total += iter.getAverageFitness();
    }

    return total;

}


void Pool::addToSpecies(Genome &genome){

    for( Species &iter : mSpecies ){
        if( iter.sameSpecies( genome ) ){
            iter.addGenome( genome );
            return;
        }
    }

    createSpeciesWith( genome );

}
