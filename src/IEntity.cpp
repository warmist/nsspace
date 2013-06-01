#include "IEntity.h"

IEntity::IEntity()
{
    //ctor
}

IEntity::~IEntity()
{
    //dtor
}
ITile *GetFromLoad(int n,VecLoad &ldata)
{
    for(VecLoad::iterator i=ldata.begin();i!=ldata.end();i++)
        if(i->num==n)
            return i->me;
    return NULL;
}
