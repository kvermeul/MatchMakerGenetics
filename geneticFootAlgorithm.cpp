#include "geneticFootAlgorithm.h"

#include<iostream>

#include "composition_helper.h"

namespace std{

template<>
struct hash<pair<position,player>>{
    std::size_t operator()(const pair<position,player>& p) const
    {
        return hash<int>()(static_cast<int>(p.first))^hash<pair<string,string>>()(std::pair<string,string>(p.second.getFirstName(),p.second.getLastName()));
    }
};
}


CompositionsAlgorithm::CompositionsAlgorithm(int cr,int mu,std::vector<std::unique_ptr<AlgorithmOption<Solution>>> & opt) : GeneticAlgorithm<Solution>(cr,mu,opt)
{

}

std::vector<std::pair<composition,composition>> CompositionsAlgorithm::getAllComb(std::vector<player> & players, std::pair<formation,formation> & formats)
{
    auto firstCompPossibilities = composition_helper::allPossibleComp(players,formats.first);

    std::vector<std::pair<composition,composition>> result;

    int added = 0;

    for(auto & firstComp : firstCompPossibilities)
    {
        std::vector<player> remainingPlayers;
        auto choosenPlayers = firstComp.getPlayers();
        for(auto & player:players)
        {
            if(std::find(choosenPlayers.begin(),choosenPlayers.end(),player)!=choosenPlayers.end())
            {
                remainingPlayers.push_back(player);
            }
        }
        auto secondCompPossibilities = composition_helper::allPossibleComp(remainingPlayers,formats.second);
        for(auto & secondComp:secondCompPossibilities)
        {
            result.push_back(std::pair<composition,composition>(firstComp,secondComp));
            ++added;
            std::cout<<added<<std::endl;
        }
    }
    return result;
}

std::vector<std::pair<composition,composition>> CompositionsAlgorithm::generateInitialGeneration(std::vector<player> & players,std::pair<formation,formation> & formats,unsigned int nbElements,std::vector<Predicate<const composition &, const composition &>>  & predicates)
{
//    auto result = composition_helper::randomCompList(players,formats,predicates,nbElements,variety);


    std::vector<std::pair<composition,composition>> result;

    while(result.size()!=nbElements)
    {
        auto comp = composition_helper::getRandomComp(players,formats.first);
        //Remove the players that have been taken in the first comp
        auto takenPlayers = comp.getPlayers();
        auto remainingPlayers = players;
        for(auto & p : takenPlayers)
        {
            remainingPlayers.erase(std::remove(remainingPlayers.begin(),remainingPlayers.end(),p),remainingPlayers.end());
        }
        auto secondComp = composition_helper::getRandomComp(remainingPlayers,formats.second);
        if(comp.getNbPlayers()==formats.first.getNbPlayers() && secondComp.getNbPlayers()==formats.second.getNbPlayers())
        {
            auto pair = std::pair<composition,composition>(comp,secondComp);
            if(composition_helper::checkAllPredicates(pair,predicates))
            {
                pair.first.calculGlobalValue();
                pair.second.calculGlobalValue();
                result.push_back(std::move(pair));
            }

        }
    }
    std::sort(result.begin(),result.end(),composition_helper::comparePairs);
    std::reverse(result.begin(),result.end());
    return result;
}


std::vector<std::pair<composition,composition>> CompositionsAlgorithm::generateInitialGeneration(std::vector<player> & players,std::pair<formation,formation> && formats,unsigned int nbElements,std::vector<Predicate<const composition &, const composition &>> & predicates)
{
    auto result = generateInitialGeneration(players,formats,nbElements,predicates);
    return result;
}

std::vector<Solution> & CompositionsAlgorithm::selection(std::vector<Solution> & solutions, std::vector<Predicate<const composition &, const composition &>>  & predicates)
{

    std::sort(solutions.begin(),solutions.end(),composition_helper::comparePairs);
    std::reverse(solutions.begin(),solutions.end());

    std::vector<Solution> copySolutions = solutions;

    std::vector<Solution> tempSolutions;



    for(auto & option : options)
    {
        auto toInsert = option->doOption(copySolutions,std::function<bool(const Solution &,const Solution &)>(composition_helper::comparePairs),std::function<double(const Solution &)>(composition_helper::calculValueSolution));
        tempSolutions.insert(std::end(tempSolutions),std::begin(toInsert),std::end(toInsert));
        copySolutions = solutions;
    }

    std::srand(time(0));

    while(tempSolutions.size()!=solutions.size())
    {
        int index = std::rand()%solutions.size();
        tempSolutions.push_back(solutions[index]);
    }
    solutions = std::move(tempSolutions);

    return solutions;

}

template<typename T,typename U>
bool containsSecond(std::vector<std::pair<U,T>> & v,T& e)
{
    for(auto & pair : v)
    {
        if(pair.second==e)
        {
            return true;
        }
    }
    return false;
}

template<typename T,typename U>
bool containsSecond(std::vector<std::pair<U,T>> & v,const T& e)
{
    for(auto & pair : v)
    {
        if(pair.second==e)
        {
            return true;
        }
    }
    return false;
}

template<typename T,typename U>
bool containsFirst(std::vector<std::pair<U,T>> & v,T& e)
{
    for(auto & pair : v)
    {
        if(pair.first==e)
        {
            return true;
        }
    }
    return false;
}


void correct(std::vector<std::pair<position,player>> & daughterSolution,std::vector<player> players)
{

    //Correct the first solution
    //Fill the positions we'll need to replace in case of double players
    std::vector<position> positions1;
    std::vector<player> alreadyAdded;
    std::vector<std::pair<position,player>> copySolution = daughterSolution;
    for(auto it = daughterSolution.begin(); it != daughterSolution.end();++it)
    {
        //Check if we have already the solution in the vector
        if(std::find(alreadyAdded.cbegin(),alreadyAdded.cend(),it->second)!=alreadyAdded.cend())
        {
            positions1.push_back(it->first);
            copySolution.erase(std::remove(copySolution.begin(),copySolution.end(),*it));
        }
        else
        {
            alreadyAdded.push_back(it->second);
            players.erase(std::remove(players.begin(),players.end(),it->second));
        }
    }

    daughterSolution = std::move(copySolution);

    if(positions1.size()!=0)
    {
        std::vector<composition> correctedSolution;
        formation f(positions1.size());
        for(auto & position : positions1)
        {
            f.addPosition(position);
        }
        composition toFill (f);
        composition_helper::fillRandomComp(players,positions1,toFill,correctedSolution,1,-1);
        if(!correctedSolution.empty())
        {
            auto correctedComp = correctedSolution[0].getComp();
            for(auto & pair : correctedComp)
            {
                daughterSolution.push_back(pair);
            }
        }

    }

}


std::vector<Solution> & CompositionsAlgorithm::crossover(std::vector<Solution> & solutions, std::vector<Predicate<const composition &, const composition &>>  & predicates)
{
    std::srand(time(NULL));

    std::vector<Solution> newSolutions;

    unsigned int newPeopleFromCrossing = crossingProbability/static_cast<double>(100) * solutions.size();


    //Sort the solutions
    std::sort(solutions.begin(),solutions.end(),composition_helper::comparePairs);

    std::reverse(solutions.begin(),solutions.end());

    //Transform the compositions to vector
    std::vector<std::vector<std::pair<position,player>>> v;
    for(auto & solution : solutions)
    {
        auto vectorSolution = composition_helper::toVector(solution);
        v.push_back(vectorSolution);
    }

    while(newSolutions.size()!=newPeopleFromCrossing)
    {
        //Define the two parents, randomly
        unsigned int firstParentIndex = std::rand()%v.size();
        auto & parent1 = v[firstParentIndex];
        unsigned int indexToCross = std::rand() % v.size();
        //Check if the two parents are different to respect the genetic law
        while(firstParentIndex==indexToCross)
        {
            indexToCross = std::rand() % v.size();
        }
        auto & parent2 = v[indexToCross];

        //Pick randomly where the cross set up
        unsigned int firstRand = std::rand() % parent1.size();
        unsigned int secondRand = std::rand() % parent1.size();

        unsigned int startCross = std::min(firstRand,secondRand);
        unsigned int endCross = std::max(firstRand,secondRand);

        std::vector<std::pair<position,player>> firstDaughterSolution;
        std::vector<std::pair<position,player>> secondDaughterSolution;
        std::vector<player> correction;
        for(unsigned int i = 0; i<startCross;i++)
        {
            firstDaughterSolution.push_back(parent1[i]);
            secondDaughterSolution.push_back(parent2[i]);
            correction.push_back(parent1[i].second);

        }
        //Cross the two and keep in memory the players that have been replaced for the correction
        for(unsigned int i = startCross; i<endCross;i++)
        {
            firstDaughterSolution.push_back(parent2[i]);
            secondDaughterSolution.push_back(parent1[i]);
            correction.push_back(parent1[i].second);
        }

        for(unsigned int i = endCross;i<parent1.size();i++)
        {
            firstDaughterSolution.push_back(parent1[i]);
            secondDaughterSolution.push_back(parent2[i]);
            correction.push_back(parent1[i].second);
        }


        unsigned int previousSize1 = firstDaughterSolution.size();
        unsigned int previousSize2 = secondDaughterSolution.size();

        //Correct the first solution
        //Fill the positions we'll need to replace in case of double players
        correct(firstDaughterSolution,correction);

        //Correct the second solution
        correct(secondDaughterSolution,correction);

        if(firstDaughterSolution.size()==previousSize1&&secondDaughterSolution.size()==previousSize2)
        {
            //Add the new solutions
            formation f1 = solutions[0].first.getFormat();
            formation f2 = solutions[0].second.getFormat();
            auto comp1 = composition_helper::toCompositionPair(firstDaughterSolution,f1,f2);
            auto comp2 = composition_helper::toCompositionPair(secondDaughterSolution,f1,f2);

            if(comp1.first.getNbPlayers()==f1.getNbPlayers()&&comp1.second.getNbPlayers()==f2.getNbPlayers())
            {
                comp1.first.calculGlobalValue();
                comp1.second.calculGlobalValue();
                if(composition_helper::checkAllPredicates(comp1,predicates))
                {
                    newSolutions.push_back(std::move(comp1));
                    if(newSolutions.size()==newPeopleFromCrossing)
                    {
                        break;
                    }
                }
            }

            if(comp2.first.getNbPlayers()==f1.getNbPlayers()&&comp2.second.getNbPlayers()==f2.getNbPlayers())
            {
                comp2.first.calculGlobalValue();
                comp2.second.calculGlobalValue();
                if(composition_helper::checkAllPredicates(comp2,predicates))
                {
                    newSolutions.push_back(std::move(comp2));
                    if(newSolutions.size()==newPeopleFromCrossing)
                    {
                        break;
                    }
                }
            }
        }
    }

    //Complete with old solutions

    //Keep the best one, no choice and put it in the first position for the mutation
    newSolutions.insert(newSolutions.begin(),solutions[0]);

    while(newSolutions.size()!=solutions.size())
    {
        int complete = solutions.size();
        int index = std::rand()%complete;
        auto & old = solutions[index];
        newSolutions.push_back(old);
    }

    solutions = std::move(newSolutions);
    return solutions;
}


std::vector<Solution> & CompositionsAlgorithm::mutate(std::vector<Solution> & solutions,formation & f1,formation & f2,std::vector<Predicate<const composition &, const composition &>>  & predicates)
{
    std::srand(time(NULL));

    for(unsigned int i=1;i<solutions.size();i++)
    {

        bool mutate = (std::rand()%100) <= mutationProbability;
        if(mutate)
        {
            auto copySolution = solutions[i];

            //Find one player that can swap with current one
            auto currentSolution = composition_helper::toVector(solutions[i]);
            unsigned int indexMutate = std::rand()%currentSolution.size();
            std::unordered_map<int,std::pair<position,player>> possibilitiesToMutate;
            std::vector<int> indexes;
            for(unsigned int j=0 ;j<currentSolution.size();j++)
            {
                if(currentSolution[j].second.canPlay(currentSolution[indexMutate].first) && currentSolution[indexMutate].second.canPlay(currentSolution[j].first) && currentSolution[j] != currentSolution[indexMutate])
                {
                    possibilitiesToMutate.insert(std::pair<int,std::pair<position,player>>(j,currentSolution[j]));
                    indexes.push_back(j);
                }
            }

            unsigned int choosenMutation = std::rand()%indexes.size();

            unsigned int correspondingKey = indexes[choosenMutation];

            //Swap the two players
            auto temp = currentSolution[indexMutate];
            currentSolution[indexMutate] = std::pair<position,player>(currentSolution[indexMutate].first,possibilitiesToMutate.find(correspondingKey)->second.second);
            currentSolution[correspondingKey] = std::pair<position,player>(possibilitiesToMutate.find(correspondingKey)->second.first,temp.second);
            solutions[i] = composition_helper::toCompositionPair(currentSolution,f1,f2);
            if(!composition_helper::checkAllPredicates(solutions[i],predicates))
            {
                solutions[i] = copySolution;
            }
            else
            {
                solutions[i].first.calculGlobalValue();
                solutions[i].second.calculGlobalValue();
            }
        }

    }


    return solutions;
}







