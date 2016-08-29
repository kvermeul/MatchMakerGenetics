#include "composition.h"

composition::composition(formation & f):format(f)
{

}

composition::composition(const formation & f):format(f)
{

}

composition::~composition()
{

}

bool composition::hasPlayer(const player & p) const{
    for(auto it = begin(comp);it != end(comp);++it)
    {
        if(it->second==p)
        {
            return true;
        }
    }
    return false;
}




double composition::calculGlobalValue()
{

    double result = 0;
    std::unordered_map<position,double> synergyCoeffs;

    std::vector<position> positions = format.getPositions();

    for(auto init = positions.begin();init != positions.end();++init)
    {
        synergyCoeffs.insert(std::pair<position,double>(*init,1.0));
    }


    auto syners = format.getSynergies();

    for(auto it = syners.begin();it != syners.end();++it)
    {
        auto p1 = comp.find(it->first);
        auto p2 = comp.find(it->second);


        if(p1==comp.end() || p2==comp.end())
        {
            std::cerr<<"COMPOSITION NOT FULL, PLEASE PROVIDE ALL PLAYERS"<<std::endl;
            return(0);
        }
        else
        {
            auto syn = p1->second.getPlayerSynergies().find(std::pair<std::string,std::string>(p2->second.getFirstName(),p2->second.getLastName()));
            if(syn !=p1->second.getPlayerSynergies().end())
            {
                synergyCoeffs[it->first] *= syn->second;
                synergyCoeffs[it->second] *= syn-> second;
            }
        }
    }

    for(auto itComp = comp.begin();itComp != comp.end();++itComp)
    {
        std::unordered_map<position,int> map = itComp->second.getValues();
        int baseValue = map.find(itComp->first)->second;
        result += synergyCoeffs[itComp->first]* baseValue;
    }

    value = result;
    return result;
}


std::vector<player> composition::getPlayers() const
{
    std::vector<player> result;
    for(auto & pair:comp)
    {
        result.push_back(pair.second);
    }
    return result;
}

