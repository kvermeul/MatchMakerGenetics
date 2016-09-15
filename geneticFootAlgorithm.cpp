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


CompositionsAlgorithm::CompositionsAlgorithm(std::vector<std::unique_ptr<AlgorithmOption<Solution>>> & opt) : GeneticAlgorithm<Solution>(opt)
{

}

CompositionsAlgorithm::CompositionsAlgorithm(GeneticStrategy<Solution> & strategy, std::vector<std::unique_ptr<AlgorithmOption<Solution>>> & opt) : GeneticAlgorithm<Solution>(strategy,opt)
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

    std::srand(time(0));

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


void correctPlayers(std::vector<std::pair<position,player>> & daughterSolution,std::vector<player> players)
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
        //Randomize more the correction of the positions
        std::random_shuffle(players.begin(),players.end());
        std::random_shuffle(positions1.begin(),positions1.end());
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
    std::vector<Solution> crossedSolutions;

    unsigned int newPeopleFromCrossing = genStrategy.getCrossingProbability()/static_cast<double>(100) * solutions.size();


    //Sort the solutions
    std::sort(solutions.begin(),solutions.end(),composition_helper::comparePairs);


    auto best = solutions[solutions.size()-1];
    //Transform the compositions to vector
    std::vector<std::vector<std::pair<position,player>>> v;
    for(auto & solution : solutions)
    {
        auto vectorSolution = composition_helper::toVector(solution);
        v.push_back(vectorSolution);
    }

    //Total random cross over
    for(unsigned int i=0; i<2*newPeopleFromCrossing;i++)
    {
        //Define the two parents, randomly
        unsigned int firstParentIndex = std::rand()%v.size();//i%v.size();
        auto & parent1 = v[firstParentIndex];
        unsigned int indexToCross = std::rand() % v.size();
        //Check if the two parents are different to respect the genetic law
        while(firstParentIndex==indexToCross)
        {
            indexToCross = std::rand() % v.size();
        }
        auto & parent2 = v[indexToCross];

        ///////////////////////////////DISPLAY PARENT VALUES//////////////////////////////
        //std::cout<<"PARENT 1 VALUE: " <<composition_helper::calculValueSolution(solutions[firstParentIndex]) <<", PARENT 1 STRENGTH : "<<composition_helper::calculStrengthSolution(solutions[firstParentIndex])<<std::endl;
        //std::cout<<"PARENT 2 VALUE: " <<composition_helper::calculValueSolution(solutions[indexToCross]) <<", PARENT 2 STRENGTH : "<<composition_helper::calculStrengthSolution(solutions[indexToCross])<<std::endl;

        //        std::random_shuffle(parent1.begin(),parent1.end());
        //        std::random_shuffle(parent2.begin(),parent2.end());

        //Pick randomly where the cross set up
        unsigned int firstRand = std::rand() % parent1.size();
        unsigned int secondRand = std::rand() % parent1.size();

        unsigned int startCross = std::min(firstRand,secondRand);
        unsigned int endCross = std::max(firstRand,secondRand);

        std::vector<std::pair<position,player>> firstDaughterSolution;
        std::vector<std::pair<position,player>> secondDaughterSolution;
        //Represent all the players
        std::vector<player> correctionPlayers;
        for(unsigned int i = 0; i<startCross;i++)
        {
            firstDaughterSolution.push_back(parent1[i]);
            secondDaughterSolution.push_back(parent2[i]);
            correctionPlayers.push_back(parent1[i].second);
        }
        //Cross the two and keep in memory the players that have been replaced for the correction
        for(unsigned int i = startCross; i<endCross;i++)
        {
            firstDaughterSolution.push_back(parent2[i]);
            secondDaughterSolution.push_back(parent1[i]);
            correctionPlayers.push_back(parent1[i].second);
        }
        //////////////////////////////////////////////// CROSSING OVER ////////////////////////////////////////////
        for(unsigned int i = endCross;i<parent1.size();i++)
        {
            firstDaughterSolution.push_back(parent1[i]);
            secondDaughterSolution.push_back(parent2[i]);
            correctionPlayers.push_back(parent1[i].second);
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        unsigned int previousSize1 = firstDaughterSolution.size();
        unsigned int previousSize2 = secondDaughterSolution.size();

        //Correct the first solution
        //Fill the positions we'll need to replace in case of double players
        correctPlayers(firstDaughterSolution,correctionPlayers);

        //Correct the second solution
        correctPlayers(secondDaughterSolution,correctionPlayers);


        auto f1 = solutions[0].first.getFormat();
        auto f2 = solutions[0].second.getFormat();

        if(firstDaughterSolution.size()==previousSize1)
        {
            //Add the first solution

            auto comp1 = composition_helper::toCompositionPair(firstDaughterSolution,f1,f2);

            if(comp1.first.getNbPlayers()==f1.getNbPlayers()&&comp1.second.getNbPlayers()==f2.getNbPlayers())
            {
                comp1.first.calculGlobalValue();
                comp1.second.calculGlobalValue();
                if(composition_helper::checkAllPredicates(comp1,predicates))
                {
                    //std::cout<<"CHILD1 VALUE: " <<composition_helper::calculValueSolution(comp1) <<", CHILD 1 STRENGTH : "<<composition_helper::calculStrengthSolution(comp1)<<std::endl;
                    crossedSolutions.push_back(std::move(comp1));
                    if(crossedSolutions.size()==newPeopleFromCrossing)
                    {
                        break;
                    }
                }
            }
        }

        if(secondDaughterSolution.size()==previousSize2)
        {
            auto comp2 = composition_helper::toCompositionPair(secondDaughterSolution,f1,f2);
            if(comp2.first.getNbPlayers()==f1.getNbPlayers()&&comp2.second.getNbPlayers()==f2.getNbPlayers())
            {
                comp2.first.calculGlobalValue();
                comp2.second.calculGlobalValue();
                //std::cout<<"CHILD 2 VALUE: " <<composition_helper::calculValueSolution(comp2) <<", CHILD 2 STRENGTH : "<<composition_helper::calculStrengthSolution(comp2)<<std::endl;
                if(composition_helper::checkAllPredicates(comp2,predicates))
                {
                    crossedSolutions.push_back(std::move(comp2));
                    if(crossedSolutions.size()==newPeopleFromCrossing)
                    {
                        break;
                    }
                }
            }
        }
    }

    //Complete with old solutions

    //Keep the best one, no choice and put it in the first position for the mutation
    std::vector<Solution> newSolutions;
    newSolutions.insert(newSolutions.begin(),best);

    while(newSolutions.size()!=crossedSolutions.size()/2)
    {
        int complete = crossedSolutions.size();
        int index = std::rand()%complete;
        auto & crossed = crossedSolutions[index];
        newSolutions.push_back(crossed);
    }

    while(newSolutions.size()!=solutions.size())
    {
        int complete = solutions.size();
        int index = std::rand()%complete;
        auto & old = solutions[index];
        newSolutions.push_back(old);
    }

    std::sort(newSolutions.begin(),newSolutions.end(),composition_helper::comparePairs);
    std::reverse(newSolutions.begin(),newSolutions.end());

    solutions = std::move(newSolutions);
    return solutions;
}


Solution CompositionsAlgorithm::mutateType1(Solution & solution,formation & f1,formation & f2,std::vector<Predicate<const composition &, const composition &>>  & predicates)
{


    auto copySolution = solution;

    //Find one player that can swap with current one
    auto currentSolution = composition_helper::toVector(solution);

    std::vector<int> probabilities;

    for(unsigned int i = 0;i<currentSolution.size();i++)
    {
        probabilities.push_back(1);
    }

    std::discrete_distribution<int> distrib(probabilities.cbegin(),probabilities.cend());

    //Define the gravity of the mutation, i.e how much will be changed
    unsigned int gravity = distrib(randomGenerator);
    for(unsigned int k=0;k<gravity;k++)
    {
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
    }
    copySolution = composition_helper::toCompositionPair(currentSolution,f1,f2);

    if(!composition_helper::checkAllPredicates(copySolution,predicates))
    {
        copySolution = solution;
    }
    else
    {
        copySolution.first.calculGlobalValue();
        copySolution.second.calculGlobalValue();
    }


    return copySolution;
}

//Return the first occurence of a found pair
template<typename T,typename U>
auto findByKey(std::vector<std::pair<T,U>> & pseudoMap,T key)
{
    auto it = pseudoMap.begin();
    for(auto it = pseudoMap.begin();it!=pseudoMap.end();++it)
    {
        if(it->first==key)
        {
            return it;
        }
    }
    return it;
}

std::vector<std::pair<position,player>> CompositionsAlgorithm::correctPositions(formation & f,formation & otherF, std::vector<std::pair<position,player>> & solution)
{

    std::vector<position> remainingPositions = f.getPositions();

    std::vector<position> alreadyAddedPositions;

    std::vector<player> remainingPlayers;

    std::vector<std::pair<position,player>> result;

    for(auto & el : solution)
    {
        remainingPlayers.push_back(el.second);
    }

    for(auto & el : solution)
    {
        if(std::find(alreadyAddedPositions.cbegin(),alreadyAddedPositions.cend(),el.first)!=alreadyAddedPositions.cend())
        {
            auto p = *findByKey(result,el.first);
            result.erase(std::remove(result.begin(),result.end(),p));
            remainingPlayers.push_back(p.second);
            remainingPlayers.erase(std::remove(remainingPlayers.begin(),remainingPlayers.end(),el.second));
            result.push_back(el);
            formation format(remainingPositions.size());
            for(auto & pos : remainingPositions)
            {
                format.addPosition(pos);
            }
            std::vector<Predicate<const composition &, const composition &>> emptyPred;
            auto mutations = generateInitialGeneration(remainingPlayers,std::pair<formation,formation>(format,otherF),1,emptyPred);
            //Check if solution is correct
            if(!mutations.empty())
            {
                auto mutate = composition_helper::toVector(mutations[0]);
                result.insert(result.begin(),mutate.begin(),mutate.end());
                return result;
            }
            else
            {
                break;
            }

        }
        else
        {
            alreadyAddedPositions.push_back(el.first);
            result.push_back(el);
            remainingPlayers.erase(std::remove(remainingPlayers.begin(),remainingPlayers.end(),el.second));
            remainingPositions.erase(std::remove(remainingPositions.begin(),remainingPositions.end(),el.first));
        }
    }
    //No configuration after mutation has been found
    return result;
}

Solution & CompositionsAlgorithm::mutateType2(Solution & solution,formation & f1,formation & f2,std::vector<Predicate<const composition &, const composition &>>  & predicates)
{


    auto copySolution = solution;

    //Find one player that can swap with current one
    auto currentSolution = composition_helper::toVector(solution);


    std::vector<int> probabilities;

    for(unsigned int i = 0;i<currentSolution.size();i++)
    {
        probabilities.push_back(currentSolution.size()-i);
    }

    auto indexMutate = std::rand()%currentSolution.size();

    //Randomly select the new position of the player if it's compatible with the formation

    std::vector<position> possiblePositions = currentSolution[indexMutate].second.getPositions();

    auto newPosition = std::rand()%possiblePositions.size();

    bool hasMutate = false;

    //Check if the formation has the selected position
    //Select the formation and the players we'll need to correct
    std::vector<std::pair<position,player>> team;
    formation intactForm= f1;
    formation mutateForm = f2;
    if(indexMutate<currentSolution.size())
    {
        mutateForm = f1;
        intactForm = f2;
        while(!f1.hasPosition(possiblePositions[newPosition]))
        {
            newPosition = std::rand()%possiblePositions.size();
        }
    }
    else
    {
        while(!f2.hasPosition(possiblePositions[newPosition]))
        {
            newPosition = std::rand()%possiblePositions.size();
        }
    }
    team = std::vector<std::pair<position,player>>(currentSolution.cbegin(),currentSolution.cend());
    team[indexMutate] = std::pair<position,player>(possiblePositions[newPosition],team[indexMutate].second);
    //Correct the positions
    auto corrected = correctPositions(mutateForm,intactForm,team);
    if(corrected.size()==team.size())
    {
        hasMutate = true;
    }

    //Update the current solution
    /////////////////////////////////////////////////////////////////////////////////////
    if(hasMutate)
    {
        solution = composition_helper::toCompositionPair(corrected,f1,f2);
        if(!composition_helper::checkAllPredicates(solution,predicates))
        {
            solution = std::move(copySolution);
        }
        else
        {
            solution.first.calculGlobalValue();
            solution.second.calculGlobalValue();
        }
    }

    return solution;
}


std::vector<Solution> & CompositionsAlgorithm::mutate(std::vector<Solution> & solutions ,formation & format1,formation & format2, std::vector<Predicate<const composition &, const composition &>>  & predicates)
{

    for(unsigned int i=0;i<solutions.size();i++)
    {
        bool mutate = (std::rand()%100) <= genStrategy.getMutationProbability();

        if(mutate)
        {
            auto mutateSol = mutateType1(solutions[i],format1,format2, predicates);
            //            if(i<=solutions.size()/2)
            //            {
            if(composition_helper::comparePairs(solutions[i],mutateSol))
            {
                solutions[i] = mutateSol;
            }
            //            }
            //            else
            //            {
            //                solutions[i] = mutateSol;
            //            }
        }
    }
    return solutions;
}


std::vector<Solution> CompositionsAlgorithm::generateClosestElements(Solution & solution,unsigned int nbElements,std::vector<Predicate<const composition &, const composition &>>  & predicates)
{
    std::vector<Solution> result;
    auto vecSolution = composition_helper::toVector(solution);

    auto f1 = solution.first.getFormat();
    auto f2 = solution.second.getFormat();
    //Find the closest possible elements of the solution

    bool hasAtLeastCheckedOneTimeAllMutation = false;
    while(result.size()<nbElements)
    {
        //Make the i element mutate
        for(unsigned int i=0; i<vecSolution.size();i++)
        {
            //Find players that can play to these positions and put them in the map
            for(unsigned int j = 0; j<vecSolution.size();j++)
            {
                auto newSolution = vecSolution;
                //Check if the other player can swap with the current one in position
                if(i!=j && newSolution[j].second.canPlay(newSolution[i].first) && newSolution[i].second.canPlay(newSolution[j].first))
                {
                    auto temp = newSolution[i];
                    newSolution[i] = std::pair<position,player>(newSolution[i].first,newSolution[j].second);
                    newSolution[j] = std::pair<position,player>(newSolution[j].first,temp.second);

                    auto newCompSolution = composition_helper::toCompositionPair(newSolution,f1,f2);
                    if(!hasAtLeastCheckedOneTimeAllMutation)
                    {
                        if(std::find(result.cbegin(),result.cend(),newCompSolution)==result.cend() && composition_helper::checkAllPredicates(newCompSolution,predicates))
                        {
                            newCompSolution.first.calculGlobalValue();
                            newCompSolution.second.calculGlobalValue();
                            result.push_back(newCompSolution);
                            if(result.size()==nbElements)
                            {
                                break;
                            }
                        }
                    }
                    else
                    {
                        if(composition_helper::checkAllPredicates(newCompSolution,predicates))
                        {
                            newCompSolution.first.calculGlobalValue();
                            newCompSolution.second.calculGlobalValue();
                            result.push_back(newCompSolution);
                            if(result.size()==nbElements)
                            {
                                break;
                            }
                        }
                    }
                }
            }
        }
        hasAtLeastCheckedOneTimeAllMutation = true;
    }

    std::sort(result.begin(),result.end(),composition_helper::comparePairs);

    return result;


}
