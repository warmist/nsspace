#ifndef ITILE_H
#define ITILE_H
enum SIDES
{
    E_UP,E_NW,E_N,E_NE,E_E,E_W,E_SW,E_S,E_SE,E_DOWN,E_LAST
};

const int SIDE_NODIAG[]={E_UP,E_N,E_E,E_W,E_S,E_DOWN,E_LAST};
#include "libtcod.hpp"
#define MAXMAT 8
#include "string"
class ITile;
#include "IEntity.h"
//#include "ISpecial.h"
#include "material.h"
#include "IItem.h"
#define H_HUMAN 3
/*
int wraparound(int b)
{
    if(b<1)b=8;
    if(b>9)b=1;
    return b;
}*/
SIDES closest(SIDES b,bool ccw);
SIDES closestNoDiag(SIDES b,bool ccw);
struct Mob
{
    TCODColor col;
    int pic;
    ItemStack inventory;
    ITile *mytile;//multitilism

};

struct TileSave
{
    int num;
    bool saved;
    int mats[MAXMAT];
    int flags;
};


class ITile
{
    public:
        ITile();
        virtual ~ITile();
    virtual TileSave *GetSave()=0;
    virtual void Remove()=0;
    virtual void RemoveVoid()=0;
    virtual ITile *GetSide(SIDES s)=0;
    virtual void SetSide(ITile *t,SIDES s)=0;
    virtual Mob *GetMob()=0;
    //virtual ISpecial *GetSpecial()=0;
    //virtual void SetSpecial(ISpecial *t)=0;
    virtual void SetMob(Mob *t)=0;
    virtual void SetMaterial(Material *t,int liqh)=0;
    virtual void SetHeight(int h,Material *t)=0;
    virtual Material *GetMaterial(int h)=0;

    virtual void AddEntity(IEntity *toadd)=0;
    virtual IEntity *GetEntity(int n)=0;
    virtual void RemoveEntity(IEntity *torem)=0;
    virtual int GetEntityCount()=0;
    virtual void RemoveEntity(int n)=0;
    virtual void EntityTick()=0;
    virtual void EntityReset()=0;
    virtual void SaveEntitys(TCODZip &file)=0;
    virtual IEntity *GetOfType(int &pos,CLASSES type)=0;
    virtual bool HasFlag(int flag)=0;
    //virtual void LoadEntitys(TCODZip &file,VecLoad &bonusdata)=0;


    virtual bool see()=0;
    virtual void TransferMass(ITile *t)=0;
    virtual void TransferMassForced(ITile *t)=0;
    virtual int GetHeight()=0;
    virtual int GetSolidH()=0;

    virtual int GetTemp()=0;
    virtual void SetTemp(int t)=0;
    virtual void AddTemp(int t)=0;

    virtual bool AddTop(Material *t)=0;
    virtual Material *GetTop()=0;
    virtual void RemoveTop()=0;
    virtual int GetPic()=0;
    virtual TCODColor GetColor()=0;
    virtual ItemStack *GetStack()=0;
    virtual void SaveStack(TCODZip &file)=0;
    virtual void LoadStack(TCODZip &file)=0;
    virtual void AddItem(IItem *t)=0;
    virtual void DropStackDown()=0;
    virtual bool PassibleLight()=0;
    virtual bool PassibleLight(SIDES s)=0;
    virtual bool PassibleSolid()=0;
    virtual bool PassibleSolid(SIDES s)=0;
    virtual bool PassibleLiquid()=0;
    virtual bool PassibleLiquid(SIDES s)=0;
    virtual void StackAndPack();

    // supportas,ty kiek supportinta..

    protected:
    private:
};
bool Dig(ITile *from,SIDES to);
class TileFunct
{
    public:
        virtual ~TileFunct(){};
        virtual void Init(){};
        virtual bool Run(ITile *trg)=0;
        virtual void Finish(){};
};
class RayFunct
{
    public:
        virtual ~RayFunct(){};
        virtual void Init(ITile *str){};
        virtual bool Run(ITile *trg)=0;
        virtual void Finish(ITile *end){};
};
typedef std::vector <ITile *> ITiles;
#endif // ITILE_H
