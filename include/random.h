#ifndef RANDOM_H
#define RANDOM_H
#include "libtcod.hpp"

struct range
{
    int from,to;
    range(){};
    range(int f,int t):from(f),to(t){};
};
struct dice
{
    int num,sided,plus;
};

#endif // RANDOM_H
