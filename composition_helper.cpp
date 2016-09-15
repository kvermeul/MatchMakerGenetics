#include "composition_helper.h"

#include<ctime>

#include<cstdlib>

#include <algorithm>

#include <time.h>


std::vector<composition> composition_helper::allPossibleComp(std::vector<player> & players,formation & format)
{
    std::vector<composition> result;

    auto positions = format.getPositions();


    composition c (format);
    fillComp(players,positions,c,result);

    return result;
}

void composition_helper::fillComp(std::vector<player> & players, std::vector<position> & positions,composition & comp,std::vector<composition> & comps)
{
    auto remainingPlayers = players;
    auto remainingPositions = positions;
    auto itPlay = players.begin();
    auto itRemainPlay=remainingPlayers.begin();
    auto itPos = positions.begin();
    auto itRemainPos = remainingPositions.begin();

    auto copyComp = comp;
    for(;itPlay != players.end();itPlay++,itRemainPlay++)
    {
        if(!copyComp.hasPlayer(*itPlay) && itPlay->canPlay(*itPos))
        {
            copyComp.addPlayer(*itPos,*itPlay);
            remainingPlayers.erase(itRemainPlay);
            remainingPositions.erase(itRemainPos);
            if(copyComp.getNbPlayers()==copyComp.getFormat().getNbPlayers())
            {
                comps.push_back(copyComp);
                remainingPositions = positions;
                copyComp = comp;
            }
            else
            {
                composition_helper::fillComp(remainingPlayers,remainingPositions,copyComp,comps);
                remainingPlayers = players;
                remainingPositions = positions;
                copyComp = comp;
            }

        }
    }
}

bool composition_helper::checkAllPredicates(const std::pair<composition,composition> & comp,PredicatesVector & predicates)
{
    for(auto & predicate : predicates)
    {
        if(!predicate.check(comp.first,comp.second))
        {
            return false;
        }
    }
    return true;
}

std::vector<std::pair<composition,composition>> composition_helper::randomCompList(std::vector<player> & players,std::pair<formation,formation> & formats,PredicatesVector & predicates ,unsigned int solutionsNumber, int variety)
{
    std::vector <player> totalPlayers = players;
    std::srand(time(0));
    std::random_shuffle(players.begin(),players.end());
    std::vector<std::pair<composition,composition>> result;
    std::vector<std::pair<composition,composition>> tempResult;



    auto positions1 = formats.first.getPositions();
    auto positions2 = formats.second.getPositions();


    composition c1 (formats.first);
    composition c2 (formats.second);

    int firstSolutionsNumber = solutionsNumber;
    int secondSolutionsNumber = 20;


    std::vector<composition> firstCompPosibilities;

    fillRandomComp(players,positions1,c1,firstCompPosibilities,firstSolutionsNumber,variety);

    for(auto & firstComp : firstCompPosibilities)
    {
        std::vector<player> remainingPlayers;
        for(auto it = totalPlayers.cbegin();it!=totalPlayers.cend();++it)
        {
            if(!firstComp.hasPlayer(*it))
            {
                remainingPlayers.push_back(*it);
            }
        }
        std::vector<composition> secondCompPossibilities;
        std::random_shuffle(remainingPlayers.begin(),remainingPlayers.end());
        fillRandomComp(remainingPlayers,positions2,c2,secondCompPossibilities,secondSolutionsNumber,-1);
        for(auto & secondComp : secondCompPossibilities)
        {
            auto potentialSolution = std::pair<composition,composition>(firstComp,secondComp);
            if(checkAllPredicates(potentialSolution,predicates))
            {
                potentialSolution.first.calculGlobalValue();
                potentialSolution.second.calculGlobalValue();
                tempResult.push_back(potentialSolution);
            }
        }
    }


    std::srand(time(NULL));

    if(tempResult.size()<solutionsNumber)
    {
        for(auto & solution : tempResult)
        {
            result.push_back(solution);
        }
        return result;
    }
    while(result.size()!=solutionsNumber)
    {
        int index = std::rand() % tempResult.size();
        result.push_back(tempResult[index]);
    }


    return result;
}


void composition_helper::fillRandomComp(std::vector<player> & players, std::vector<position> & positions,composition & comp,std::vector<composition> & comps,unsigned int solutionsNumber,int variety )
{

    auto remainingPlayers = players;
    auto remainingPositions = positions;
    auto itPlay = players.begin();
    auto itRemainPlay=remainingPlayers.begin();
    auto itPos = positions.begin();
    auto itRemainPos = remainingPositions.begin();

    auto copyComp = comp;
    for(;itPlay != players.end() && comps.size()!=solutionsNumber;itPlay++,itRemainPlay++)
    {
        if(!copyComp.hasPlayer(*itPlay) && itPlay->canPlay(*itPos))
        {
            copyComp.addPlayer(*itPos,*itPlay);
            remainingPlayers.erase(itRemainPlay);
            remainingPositions.erase(itRemainPos);
            if(copyComp.getNbPlayers()==copyComp.getFormat().getNbPlayers())
            {

                if(comps.size()<solutionsNumber)
                {
                    int rand (std::rand()%100);
                    if(rand>variety)
                    {
                        comps.push_back(copyComp);
                    }
                }
                remainingPositions = positions;
                copyComp = comp;
            }
            else
            {
                composition_helper::fillRandomComp(remainingPlayers,remainingPositions,copyComp,comps,solutionsNumber,variety);
                remainingPlayers = players;
                remainingPositions = positions;
                copyComp = comp;
            }
        }
    }
}

//Care, the comp returned can be invalid
composition composition_helper::getRandomComp(std::vector<player> & players,formation & format)
{

    std::vector<std::pair<position,std::vector<player>>> playersByPosition;

    auto positions (format.getPositions());

    //First fill the possible players
    for(auto & pos : positions)
    {
        std::vector<player> ps;
        for(auto & player : players)
        {
            if(player.canPlay(pos))
            {
                ps.push_back(player);
            }
        }
        playersByPosition.push_back(std::pair<position,std::vector<player>>(pos,ps));
    }

    composition result(format);

    //Now pick randomly the players to fill the comp
    for(auto & pair : playersByPosition)
    {
        if(pair.second.empty())
        {
            break;
        }
        else
        {
            bool found = false;
            while(!found)
            {
                unsigned int index = std::rand()%pair.second.size();
                player possiblePlayer = pair.second[index];
                if(!result.hasPlayer(possiblePlayer))
                {
                    found = true;
                    result.addPlayer(pair.first,possiblePlayer);
                }
                for(auto & el : playersByPosition)
                {
                    auto & v = el.second;
                    v.erase(std::remove(v.begin(),v.end(),possiblePlayer),v.end());
                }
            }

        }
    }
    return result;
}


std::vector<composition> composition_helper::getBestComp(std::vector<player> & players,formation & f)
{
    std::vector<composition> possibleComp = composition_helper::allPossibleComp(players,f);

    std::vector<composition> result;
    double maxValue = 0;
    for(auto& comp:possibleComp)
    {
        auto globalValue = comp.calculGlobalValue();
        if(globalValue==maxValue)
        {
            result.push_back(comp);
        }else if(globalValue>maxValue)
        {
            result.clear();
            result.push_back(comp);
            maxValue = globalValue;
        }
    }
    return result;
}

std::vector<std::pair<position,player>> composition_helper::toVector(composition & comp)
{
    std::vector<std::pair<position,player>> vectorSolution;
    auto firstComp = comp.getComp();
    for(auto & pair: firstComp)
    {
        vectorSolution.push_back(pair);
    }
    return vectorSolution;
}


std::vector<std::pair<position,player>> composition_helper::toVector(std::pair<composition,composition> & solution)
{
    //Transform the compositions to vector
    auto vectorSolution = composition_helper::toVector(solution.first);
    auto vectorSolution2 = composition_helper::toVector(solution.second);
    vectorSolution.insert(vectorSolution.end(),vectorSolution2.begin(),vectorSolution2.end());
    return vectorSolution;
}

std::pair<composition,composition> composition_helper::toCompositionPair (std::vector<std::pair<position,player>> & v, formation & f1,formation & f2)
{

    int s1 = f1.getNbPlayers();
    int s2 = f2.getNbPlayers();
    composition c1(f1);
    for(int i=0;i<s1;i++)
    {
        c1.addPlayer(v[i].first,v[i].second);
    }
    composition c2(f2);
    for(int i=0;i<s2;i++)
    {
        c2.addPlayer(v[s1+i].first,v[s1+i].second);
    }
    auto result = std::pair<composition,composition> (c1,c2);
    return result;
}



