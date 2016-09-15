#ifndef GENERICPREDICATE_H
#define GENERICPREDICATE_H

#include <functional>

class GenericPredicate
{
private:
    std::function<bool()> func;
public:
    GenericPredicate(std::function<bool()> f) : func(f)
    {

    }
};


#endif // GENERICPREDICATE_H
