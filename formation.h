#ifndef FORMATION_H
#define FORMATION_H

#include <unordered_set>
#include "player.h"
#include <algorithm>

namespace std{
template<>
struct hash<pair<position,position>>{
    size_t operator()(const pair<position,position> & p) const{
        auto h1 = hash<int>()(static_cast<int>(p.first));
        auto h2 = hash<int>()(static_cast<int>(p.second));
        return h1^h2;
    }
};
}

class formation
{
private :
    int nbPlayers;
    std::vector<position> positions;
    std::unordered_set<std::pair<position,position>> formationSynergy;
public:
    formation(int);

    int getNbPlayers() const
    {
        return nbPlayers;
    }

    std::vector<position> getPositions() const
    {
        return positions;
    }

    void setPositions(std::vector<position> & newPos)
    {
        positions = newPos;
    }

    void addPosition(position p)
    {
        positions.push_back(p);
    }

    void addSynergy(position p1, position p2)
    {
        formationSynergy.insert(std::pair<position,position> (p1,p2));
    }

    std::unordered_set<std::pair<position,position>> getSynergies() const
    {
        return formationSynergy;

    }

    bool hasPosition(position p) const
    {
        return std::find(positions.cbegin(),positions.cend(),p)!=positions.cend()? true:false;
    }

    virtual ~formation();
};
#endif // FORMATION_H
