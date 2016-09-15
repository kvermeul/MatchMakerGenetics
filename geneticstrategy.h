#ifndef GENETICSTRATEGY_H
#define GENETICSTRATEGY_H

#include <functional>
#include <vector>

template<typename T>
class GeneticStrategy
{
    int crossingProbability;
    double mutationProbability;
    int numberGenerationBeforeRedistribution;
    //How to redistribute genetics after n generation
    std::function<std::vector<T>(std::vector<T> &)> geneticRedistribution;
public:
    GeneticStrategy(int cr, double mP,int nGBR,std::function<std::vector<T>(std::vector<T> &)> distrib):crossingProbability(cr), mutationProbability(mP),numberGenerationBeforeRedistribution(nGBR),geneticRedistribution(distrib)
    {

    }

    GeneticStrategy()
    {

    }

    int getNumberGenerationBeforeRedistribution() const
    {
        return numberGenerationBeforeRedistribution;
    }

    int getCrossingProbability() const
    {
        return crossingProbability;
    }

    double getMutationProbability() const
    {
        return mutationProbability;
    }

    std::vector<T> redistribute(std::vector<T> & currentGen)
    {
        return geneticRedistribution(currentGen);
    }



};

#endif // GENETICSTRATEGY_H
