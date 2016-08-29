#ifndef ALGORITHMOPTION_H
#define ALGORITHMOPTION_H

#include <unordered_set>
#include <composition.h>
#include <functional>

template <typename T>
class AlgorithmOption
{
protected:
    int value;
public:
    AlgorithmOption(int);
    virtual std::vector<T> & doOption(std::vector<std::pair<composition,composition>> &,std::function<bool(const T &, const T &)>,std::function<double(const T&)>) = 0;

};

template<typename T>
AlgorithmOption<T>::AlgorithmOption(int v) : value(v)
{

}

#endif // ALGORITHMOPTION_H
