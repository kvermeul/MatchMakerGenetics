#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include<memory>
#include "algorithmoption.h"
#include "predicate.h"
#include <functional>

template<typename T>
class GeneticAlgorithm
{
protected:
    int crossingProbability;
    double mutationProbability;
    std::vector<std::unique_ptr<AlgorithmOption<T>>> options;
    std::function<double(T &)> evaluation;
public:

    void setEvaluate(std::function<double(T)> & eval)
    {
        evaluation = eval;
    }

    GeneticAlgorithm(int, double,std::vector<std::unique_ptr<AlgorithmOption<T>>> &);

    double evaluate(T& param )
    {
        return evaluation(param);
    }


};


template<typename T>
GeneticAlgorithm<T>::GeneticAlgorithm(int cr, double mu, std::vector<std::unique_ptr<AlgorithmOption<T>>> & opt) : crossingProbability(cr),mutationProbability(mu),options(std::move(opt))
{

}

#endif // GENETICALGORITHM_H
