#ifndef ELITISM_H
#define ELITISM_H

#include "algorithmoption.h"
#include "composition_helper.h"


template<typename T>
class Elitism : public AlgorithmOption<T>
{
private:
    int strength;
public:
    Elitism(int);

    virtual std::vector<T> & doOption(std::vector<T> &,std::function<bool(const T &,const T &)>,std::function<double(const T&)>) override;
};



template<typename T>
Elitism<T>::Elitism(int v) : AlgorithmOption<T>(v)
{

}

template<typename T>
std::vector<T> & Elitism<T>::doOption(std::vector<T> & comps,std::function<bool(const T &,const T &)> compare,std::function<double(const T&)> evaluate)
{
    unsigned int numberToKeep = comps.size()*AlgorithmOption<T>::value/100;

    comps = std::vector<T>(comps.begin(),comps.begin()+numberToKeep);

//    double totalDeviation = 0;
//    double strength=0;
//    for(const auto & possibility : comps)
//    {
//        totalDeviation += std::abs(possibility.first.getValue()-possibility.second.getValue());
//        strength+=possibility.first.getValue()+possibility.second.getValue();
//    }
//    std::cout << "TOTAL DEV ELITISM: "<< totalDeviation <<std::endl;
//    std::cout << "TOTAL STR ELITIMS: "<< strength <<std::endl;
//    std::cout << "Number: "<< comps.size() <<std::endl;

    return comps;
}

#endif // ELITISM_H
