#ifndef COMPOSITION_HELPER_H
#define COMPOSITION_HELPER_H

#include "hashingextension.h"
#include "composition.h"
#include "predicate.h"

using PredicatesVector =  std::vector<Predicate<const composition &, const composition &>>;

class composition_helper
{
public:
    static std::vector<composition> allPossibleComp(std::vector<player> &,formation &);
    static void fillComp(std::vector<player> &, std::vector<position> &,composition & ,std::vector<composition> &);
    static std::vector<composition> getBestComp(std::vector<player> &,formation &);
    static std::vector<std::pair<composition,composition>> randomCompList(std::vector<player> &,std::pair<formation,formation> & ,PredicatesVector &,unsigned int,int);
    static void fillRandomComp(std::vector<player> & players, std::vector<position> & positions,composition & comp,std::vector<composition> & comps,unsigned int solutionsNumber,int variety );
    static composition getRandomComp(std::vector<player> & players,formation & format);
    static bool checkAllPredicates(const std::pair<composition,composition> &,PredicatesVector  &);
    static std::vector<std::pair<position,player>> toVector(std::pair<composition,composition> &);
    static std::pair<composition,composition> toCompositionPair (std::vector<std::pair<position,player>> &, formation & f1,formation & f2);
    static double calculValueSolution(const std::pair<composition,composition> & solution)
    {
        return std::abs(solution.first.getValue()-solution.second.getValue());
    }
    static double calculStrengthSolution(const std::pair<composition,composition> & solution)
    {
        return solution.first.getValue()+solution.second.getValue();
    }
    static bool comparePairs(const std::pair<composition,composition> & p1,const std::pair<composition,composition> & p2)
    {
        if(composition_helper::calculValueSolution(p1)==composition_helper::calculValueSolution(p2))
        {
            return p1.first.getValue() < p2.first.getValue();
        }
        else
        {
            return composition_helper::calculValueSolution(p1)>composition_helper::calculValueSolution(p2);
        }
    }

    composition_helper() = delete;

};

#endif // COMPOSITION_HELPER_H
