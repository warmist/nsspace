#ifndef ISPECIAL_H
#define ISPECIAL_H
#include "classEnums.h"
#include "libtcod.hpp"
class ISpecial;
//#include "ITile.h"
#include "IEntity.h"
class ISpecial:public IEntity
{
public:
    ISpecial() {};
    ISpecial(ITile *t);
    virtual ~ISpecial();
    virtual int GetPic()=0;
    virtual TCODColor GetColor()=0;
    virtual void SetTile(ITile *trg)
    {
        mytile=trg;
    };
    virtual ITile *GetTile()
    {
        return mytile;
    };
    virtual bool IsDead()
    {
        return false;
    };
    virtual void Tick()
    {
        done=true;
    };
    bool IsDone()
    {
        return done;
    };
    void Reset()
    {
        done=false;
    };
    virtual int GetPriority()
    {
        return 10000;
    };
protected:
    ITile *mytile;
    bool done;
private:

};
class ISP_fire:public ISpecial
{
public:
    ISP_fire(TCODZip &file,VecLoad &bonusdata)
    {
        Load(file,bonusdata);
    };
    ISP_fire(int l,int temp,ITile *trg);
    ~ISP_fire();

    TCODColor GetColor();
    int GetPic();
    void Tick();
    virtual bool IsDead();
    void Save(TCODZip &file);
    void Load(TCODZip &file,VecLoad &bonusdata);
    CLASSES GetClass()
    {
        return CL_ISP_FIRE;
    };
private:
    int life;
    int modtemp;
};
class ISP_bolt:public ISpecial
{
public:
    ISP_bolt(TCODZip &file,VecLoad &bonusdata)
    {
        Load(file,bonusdata);
    };
    ISP_bolt(int dir,int temp,int speed,ITile *trg);
    ~ISP_bolt();

    TCODColor GetColor();
    int GetPic();
    void Tick();
    virtual bool IsDead();
    void Save(TCODZip &file);
    void Load(TCODZip &file,VecLoad &bonusdata);
    CLASSES GetClass()
    {
        return CL_ISP_BOLT;
    };
private:
    int curdir;
    int modtemp;
    int speed;
    bool alive;
};
class ISP_glow:public ISpecial
{
public:
    ISP_glow(TCODZip &file,VecLoad &bonusdata)
    {
        Load(file,bonusdata);
    };
    ISP_glow(ITile *trg,int pic,int life);
    ISP_glow(ITile *trg);
    ~ISP_glow();

    TCODColor GetColor();
    int GetPic();
    void SetPic(int p)
    {
        pic=p;
    };
    void Tick();
    virtual bool IsDead();
    void Save(TCODZip &file);
    void Load(TCODZip &file,VecLoad &bonusdata);
    CLASSES GetClass()
    {
        return CL_ISP_GLOW;
    };
private:
    int life;
    int pic;
};
#endif // ISPECIAL_H
