#ifndef PREDICATE_H
#define PREDICATE_H


#include <functional>
#include <vector>

template<typename... P>
class Predicate
{
private:
    std::function<bool(P...)> func;
public:
    Predicate(std::function<bool(P...)>f):func(f)
    {

    }

    virtual bool check(P... p)
    {
        return func(p ...);
    }
};


#endif // PREDICATE_H
