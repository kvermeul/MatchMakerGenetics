#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include <memory>
#include "algorithmoption.h"
#include "geneticstrategy.h"
#include "predicate.h"
#include <functional>
#include <random>

template<typename T>
class GeneticAlgorithm
{
protected:
    //All genetics aspects
    std::vector<std::unique_ptr<AlgorithmOption<T>>> options;
    GeneticStrategy<T> genStrategy;
    std::function<double(T &)> evaluation;
    std::function<bool(const T &, const T&)> comparison ;

    std::mt19937 randomGenerator;
public:

    void setEvaluate(const std::function<double(T)> & eval)
    {
        evaluation = eval;
    }

    void setComparison(const std::function<double(T)> & comp)
    {
        comparison = comp;
    }

    void setGeneticStrategy(const GeneticStrategy<T> & strategy)
    {
        genStrategy = strategy;
    }

    GeneticStrategy<T> getGeneticStrategy() const
    {
        return genStrategy;
    }

    GeneticAlgorithm(GeneticStrategy<T> & strat,std::vector<std::unique_ptr<AlgorithmOption<T>>> &);
    GeneticAlgorithm(GeneticStrategy<T> && strat,std::vector<std::unique_ptr<AlgorithmOption<T>>> &);

    GeneticAlgorithm(std::vector<std::unique_ptr<AlgorithmOption<T>>> & );

    double evaluate(T& param )
    {
        return evaluation(param);
    }
};


template<typename T>
GeneticAlgorithm<T>::GeneticAlgorithm(GeneticStrategy<T> & strat, std::vector<std::unique_ptr<AlgorithmOption<T>>> & opt) : options(std::move(opt)),genStrategy(strat)
{
    std::random_device rd;
    randomGenerator=std::move(std::mt19937 (rd()));
}

template<typename T>
GeneticAlgorithm<T>::GeneticAlgorithm(GeneticStrategy<T> && strat, std::vector<std::unique_ptr<AlgorithmOption<T>>> & opt) : options(std::move(opt)),genStrategy(strat)
{
    std::random_device rd;
    randomGenerator=std::move(std::mt19937 (rd()));
}

template<typename T>
GeneticAlgorithm<T>::GeneticAlgorithm(std::vector<std::unique_ptr<AlgorithmOption<T>>> & opt) : options(std::move(opt)),genStrategy()
{
    std::random_device rd;
    randomGenerator=std::move(std::mt19937 (rd()));
}

#endif // GENETICALGORITHM_H
