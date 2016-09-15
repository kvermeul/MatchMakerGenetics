#ifndef GENETICS_ALGORITHM_H
#define GENETICS_ALGORITHM_H

#include <unordered_set>
#include <algorithm>
#include "predicate.h"
#include "algorithmoption.h"
#include "geneticalgorithm.h"
#include <memory>


//namespace std{
//template<>
//struct equal_to<composition>{
//    bool operator()(const composition & c1,const composition & c2) const{
//        auto comp1Map = c1.getComp();
//        auto comp2Map = c2.getComp();
//        return comp1Map.size() == comp2Map.size() && std::equal(comp1Map.cbegin(),comp1Map.cend(),comp2Map.cbegin());
//    }
//};
//}




//namespace std{
//template<>
//struct equal_to<pair<composition,composition>>{
//    bool operator()(const pair<composition,composition> & c1,const pair<composition,composition> & c2) const{
//        if((equal_to<composition>()(c1.first,c2.first)&&std::equal_to<composition>()(c1.second,c2.second)) || (std::equal_to<composition>()(c1.first,c2.second)&&std::equal_to<composition>()(c1.second,c2.first)))
//        {
//            return true;
//        }
//        return false;
//    }
//};
//}

using Solution = std::pair<composition,composition>;

class CompositionsAlgorithm : public GeneticAlgorithm<Solution>
{
public:
    CompositionsAlgorithm(std::vector<std::unique_ptr<AlgorithmOption<Solution>>> &);
    CompositionsAlgorithm(GeneticStrategy<Solution> &, std::vector<std::unique_ptr<AlgorithmOption<Solution>>> &);


    std::vector<Solution> getAllComb(std::vector<player> &, std::pair<formation,formation> &);
    std::vector<Solution> generateInitialGeneration(std::vector<player> &,std::pair<formation,formation> &,unsigned int,std::vector<Predicate<const composition &, const composition &>>  &);
    std::vector<Solution> generateInitialGeneration(std::vector<player> &,std::pair<formation,formation> &&,unsigned int,std::vector<Predicate<const composition &, const composition &>>  &);
    std::vector<Solution> & selection(std::vector<Solution> &, std::vector<Predicate<const composition &, const composition &>>  &);
    std::vector<Solution> & crossover(std::vector<Solution> &, std::vector<Predicate<const composition &, const composition &>>  &);
    std::vector<std::pair<position,player>> correctPositions(formation & f,formation & otherF, std::vector<std::pair<position,player>> & solution);
    std::vector<Solution> & mutate(std::vector<Solution> &,formation & format1,formation & format2, std::vector<Predicate<const composition &, const composition &>>  &);
    std::vector<Solution> generateClosestElements(Solution & ,unsigned int, std::vector<Predicate<const composition &, const composition &>>  &);
    Solution mutateType1(Solution &,formation & format1,formation & format2, std::vector<Predicate<const composition &, const composition &>>  &);
    Solution & mutateType2(Solution & solutions,formation & f1,formation & f2,std::vector<Predicate<const composition &, const composition &>>  & predicates);
};









#endif // GENETICS_ALGORITHM_H
