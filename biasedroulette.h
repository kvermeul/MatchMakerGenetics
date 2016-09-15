#ifndef BIASEDROULETTE_H
#define BIASEDROULETTE_H

#include "algorithmoption.h"
#include "composition_helper.h"

/*
 *Corresponds to the pairs of values in the roulette : 10%/5
 */
template<typename T>
class BiasedRoulette : public AlgorithmOption<T>
{
public:
    BiasedRoulette(int);
    virtual std::vector<T> & doOption(std::vector<T> &,std::function<bool(const T &, const T&)>,std::function<double(const T&)>) override;

};


template<typename T>
BiasedRoulette<T>::BiasedRoulette(int v):AlgorithmOption<T>(v)
{

}



template<typename T>
std::vector<T> & BiasedRoulette<T>::doOption(std::vector<T> & comps,std::function<bool(const T &,const T &)> compare,std::function<double(const T&)> evaluate)
{

    unsigned int numberToKeep = comps.size()*AlgorithmOption<T>::value/100;

    std::vector<T> toFillVec;

    std::vector<int> probabilities;

    for(unsigned int i = 0;i<comps.size();i++)
    {
        probabilities.push_back(comps.size()-i);
    }


    std::random_device rd;
    std::mt19937 generator(rd());

    std::discrete_distribution<int> distrib(probabilities.cbegin(),probabilities.cend());

    while(toFillVec.size()!=numberToKeep)
    {
        int index = distrib(generator);
        toFillVec.push_back(comps[index]);
    }


//    double totalDeviation = 0;
//    double strength=0;
//    for(const auto & possibility : toFillVec)
//    {
//        totalDeviation += std::abs(possibility.first.getValue()-possibility.second.getValue());
//        strength+=possibility.first.getValue()+possibility.second.getValue();
//    }
//    std::cout << "TOTAL DEV BIASED: "<< totalDeviation <<std::endl;
//    std::cout << "TOTAL STR BIASED: "<< strength <<std::endl;
//    std::cout << "Number: "<< toFillVec.size() <<std::endl;

    comps = std::move(toFillVec);

    return comps;
}

#endif // BIASEDROULETTE_H
