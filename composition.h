#ifndef COMPOSITION_H
#define COMPOSITION_H

#include <iterator>
#include <iostream>
#include <algorithm>
#include <utility>
#include "formation.h"


class composition
{
private:
    double value;
    formation format;
    std::unordered_map<position,player> comp;
public:
    composition(formation &);
    composition(const formation &);

    virtual ~composition();
    double getValue() const
    {
        return value;
    }
    void addPlayer(position po,player & pl){
        comp.insert(std::pair<position,player>(po,pl));
    }
    formation getFormat()
    {
        return format;
    }

    bool hasPlayer(const player &) const;
    int getNbPlayers() const
    {
        return comp.size();
    }
    void clear()
    {
        comp.clear();
    }
    void deletePlayer(position po)
    {
        comp.erase(po);
    }
    std::unordered_map<position,player> getComp() const
    {
        return comp;
    }
    double calculGlobalValue();



    std::vector<player> getPlayers() const;
    bool operator == (const composition &other) const
    {
        return comp.size() == other.comp.size() && std::equal(comp.begin(),comp.end(),other.comp.begin());
    }
};

#endif // COMPOSITION_H
