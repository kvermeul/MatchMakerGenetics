#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <unordered_map>
#include <functional>
#include <vector>
enum class position {G,DC,DCG,DCD,DG,DD,MDC,MC,MCD,MCG,MOC,MG,MD,AG,AD,BU,BUD,BUG};

namespace std{

template<>
struct hash<position>{
    std::size_t operator()(const position& p) const
    {
        return (hash<int>()(static_cast<int>(p)));
    }
};
}



namespace std{
template<>
struct hash<pair<string,string>>{
    size_t operator()(const pair<string,string> & p) const{
        auto h1 = hash<string>()(p.first);
        auto h2 = hash<string>()(p.second);
        return h1^(h2<<1);
    }
};
}

class player
{
private :
    std::string first_name;
    std::string last_name;
    std::unordered_map<position,int> values;
    using synergyMap = std::unordered_map<std::pair<std::string,std::string>,double>;
    synergyMap player_synergies;
public:
    player(std::string, std::string);
    virtual ~player();
    void set_first_name(std::string new_first_name)
    {
        first_name = new_first_name;
    }

    void set_last_name(std::string new_last_name)
    {
        last_name = new_last_name;
    }

    std::string getFirstName() const
    {
        return first_name;
    }

    std::string getLastName() const
    {
        return last_name;
    }

    synergyMap getPlayerSynergies() const
    {
        return player_synergies;
    }

    void add_positions(std::vector<std::pair<position,int>> possiblePositions)
    {
        for(auto it=possiblePositions.begin();it!=possiblePositions.end();++it)
        {
            add_position(it->first,it->second);
        }
    }

    void add_position(position p, int value)
    {
        values.insert(std::pair<position,int> (p,value));
    }
    void add_synergy(player & p ,double d)
    {
        std::pair<std::string,std::string> id (p.getFirstName(),p.getLastName());
        player_synergies.insert(std::pair<std::pair<std::string,std::string>,double>(id,d));
    }
    bool operator == (const player &other) const
    {
        return first_name == other.first_name &&last_name ==other.last_name;
    }

    std::unordered_map<position,int> getValues() const
    {
        return values;
    }

    bool canPlay(position) const;

    std::vector<position> getPositions()
    {
        std::vector<position> positions;
        for(auto & el : values)
        {
            positions.push_back(el.first);
        }
        return positions;
    }

};



#endif // PLAYER_H
