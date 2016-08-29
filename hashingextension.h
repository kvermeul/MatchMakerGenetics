#ifndef HASHINGEXTENSION_H
#define HASHINGEXTENSION_H

#include "composition.h"

namespace std{
template<>
struct hash<composition>{
    size_t operator()(const composition & c) const{
        auto comp1Map = c.getComp();
        size_t h1 = 0;
        for(auto & element: comp1Map)
        {
            h1 += (hash<position>()(element.first)^hash<pair<string,string>>()(pair<string,string>(element.second.getFirstName(),element.second.getLastName())))<<1;
        }
        return h1;
    }
};
}


namespace std{
template<>
struct hash<pair<composition,composition>>{
    size_t operator()(const pair<composition,composition> & p) const{
        size_t h1 = hash<composition>()(p.first);
        size_t h2 = hash<composition>()(p.second);
        return h1^h2;
    }
};
}


#endif // HASHINGEXTENSION_H
