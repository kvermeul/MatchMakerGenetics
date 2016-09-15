#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include "algorithmoption.h"
#include "composition_helper.h"
#include <random>

template<typename T>
class Tournament : public AlgorithmOption<T>
{
private:
    int balance;
public:
    Tournament(int,int);
    virtual std::vector<T> & doOption(std::vector<T> &,std::function<bool(const T &,const T &)>,std::function<double(const T&)>) override;

};

template<typename T>
Tournament<T>::Tournament(int v,int b) :AlgorithmOption<T>(v),balance(b)
{

}

template<typename T>
std::vector<T> & Tournament<T>::doOption(std::vector<T> & comps,std::function<bool(const T &,const T&)> compare,std::function<double(const T&)> evaluate)
{
    unsigned int numberToKeep = comps.size()*AlgorithmOption<T>::value/100;

    std::vector<T> copyComps;

    for(unsigned int i=0;i<numberToKeep;i++)
    {
        unsigned int j = std::rand() % comps.size();
        unsigned int k = std::rand() %comps.size();
        //Handle the case of the battle is between the two same elements
        while(k==j)
        {
            j =  std::rand() % comps.size();
        }
        std::random_device rd;
        std::mt19937 generator(rd());
        const double strength1 = comps.size()-k;
        const double strength2 = comps.size()-j;
        std::discrete_distribution<int> distribution{strength1,strength2};
        int winner = distribution(generator);
        if(winner==0)
        {
            copyComps.push_back(comps[k]);
        }
        else if (winner==1)
        {
            copyComps.push_back(comps[j]);
        }
    }

//    double totalDeviation = 0;
//    double strength=0;
//    for(const auto & possibility : copyComps)
//    {
//        totalDeviation += std::abs(possibility.first.getValue()-possibility.second.getValue());
//        strength+=possibility.first.getValue()+possibility.second.getValue();
//    }
//    std::cout << "TOTAL DEV TOURNAMENT: "<< totalDeviation <<std::endl;
//    std::cout << "TOTAL STR TOURNAMENT: "<< strength <<std::endl;
//    std::cout << "Number: "<< copyComps.size() <<std::endl;

    comps = std::move(copyComps);

    return comps;
}


#endif // TOURNAMENT_H
