#include <algorithm>
#include "species.h"
#include <map>

Species::Species():
    mStaleness(0),
    mSpeciesTopFitness(0),
    mAverageFitness(0)
{

}

Genome& Species::getGenome( int n ){
    return mGenomes[n];
}

int Species::getSize(){
    return mGenomes.size();
}

#include <iostream>

void Species::killWorstHalf(){

    std::vector< std::pair<int,int> > fit;
    int half = ( mGenomes.size() / 2 ) + 1;
    for( int i = 0; i < mGenomes.size(); ++i){
        fit.push_back( std::pair< int, int >( i, mGenomes[i].getFitness()) );
    }

    for( int i = 0; i < fit.size() - 1; ++i ){
        int imax = i;
        for(int j = i + 1; j < fit.size(); ++j){
            if( fit[j].second > fit[imax].second ){
                imax = j;

            }
        }
        auto temp = fit[i];
        fit[i] = fit[imax];
        fit[imax] = temp;
    }

    auto temp = mGenomes;
    mGenomes.clear();

    for( int j = 0; j < temp.size(); ++j ){
        bool evil = false;

        for( int i = half; i < fit.size(); ++i){
            if( fit[i].first == j ){
                evil = true;
                break;
            }
        }
        if( !evil ){
            mGenomes.push_back( temp[j] );
        }
    }

}

void Species::killExceptBest(){
    auto temp = mGenomes;

    mGenomes.clear();
    Genome &best = temp[0];
    for( auto &iter : temp ){
        if( best.getFitness() < iter.getFitness() ){
            best = iter;
        }
    }

    mGenomes.push_back( best );
}

void Species::copyEverything(){
    int n = mGenomes.size();
    for(int i = 0; i <  n; ++i){
        mGenomes.push_back( mGenomes[i] );
        mGenomes.back().mutate();
    }
}


bool Species::checkStaleness(){
    if( mGenomes[0].getFitness() > mSpeciesTopFitness ){
        mStaleness = 0;
        mSpeciesTopFitness = mGenomes[0].getFitness();
        return false;
    }

    mStaleness++;

    if(mStaleness >= maxStaleness && !( mGenomes[0].getGlobalRank() == 1 )){
        return true;
    }

    return false;
}

void Species::calculateAverage(){
    float total = 0;

    for( Genome &genome : mGenomes ){
        total += genome.getGlobalRank();
    }

    mAverageFitness = total / mGenomes.size();
}

float Species::getAverageFitness(){
    return mAverageFitness;
}

Genome Species::createChild(){
    Genome copy = mGenomes[0];
    copy.mutate();
    return copy;
}

float Species::normal(Genome &intruder){
    std::map< int, bool > g1, g2;

    Genome &best = mGenomes[0];

    for( int i = 0; i < intruder.genes.size(); ++i ){
        g1.insert( std::pair< int, bool > ( intruder.genes[i].innovation, true) );
    }


    for( int i = 0; i < best.genes.size(); ++i ){

        g2.insert( std::pair< int, bool > ( best.genes[i].innovation, true) );
    }

    int disjoint = 0;
    for( auto &it : g1 ){
        auto f = g2.find( it.first );
        if( f == g2.end() ){
            disjoint++;
        }
    }

    for( auto &it: g2 ){
        auto f = g1.find( it.first );
        if( f == g1.end() ){
            disjoint++;
        }
    }

    int n = g1.size();
    if( g2.size() > n ){
        n = g2.size();
    }

    return 1.0f * disjoint / n;

}

float Species::weight(Genome &intruder){

    Genome &gen = mGenomes[0];
    std::map<int, Gen*> func;


    for( Gen &iter : intruder.genes ){
        func.insert( std::pair< int, Gen* >( iter.innovation, &iter ) );
    }
    float sum = 0;
    float coincident = 0;

    for( Gen &iter : gen.genes ){
        auto finder = func.find( iter.innovation );
        if( finder != func.end() ){
            sum += fabs( iter.weight - finder->second->weight );
            ++coincident;
        }
    }

    return sum / coincident;
}

bool Species::sameSpecies(Genome &intruder){
    float dd = Disjoint * normal( intruder );
    float dw = Weights * weight( intruder );
    return dd + dw < Threshhold;
}

void Species::addGenome(Genome &member){
    mGenomes.push_back( member );
}
