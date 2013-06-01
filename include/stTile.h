#ifndef STTILE_H_INCLUDED
#define STTILE_H_INCLUDED
#include "ITile.h"
#include <vector>
//#include <slist>
#include <fstream>
/*
^0
123
4 5
678
v9
*/


enum DoorState
{
    NODOORS,DOORSCLOSED,DOORSOPEN
};
struct Tile :public ITile
{
private:
    Material *mats[MAXMAT];
    ItemStack *stack;
    VecIEntity EntityList;
public:

    //char liqh;
    //bool walk;
    //int r,g,b;
    Tile *exits[E_LAST];
    TileSave *savedata;
    DoorState doors;
    Mob *mymob;
    //ISpecial *myspec;
    int temp;
    int matterh;
    //int myh;
    //int energy;
    //TileBonus *bonus;
    Tile();
    Tile(Material *t);
    TileSave *GetSave(){return savedata;};
    int GetHeight();
    void SetHeight(int h,Material *t);
    int GetSolidH();
    int GetDeepness();
//    ISpecial *GetSpecial(){return myspec;};
//    void SetSpecial(ISpecial *t){myspec=t;};
    void TransferMass(ITile *t);
    void TransferMassForced(ITile *t);
    void TransferTemp(ITile *t);
    void Sort();
    int GetPic();
    TCODColor GetColor();

    void Remove();
    void RemoveVoid();
    Tile *GetSide(SIDES s);
    void SetSide(ITile *t,SIDES s);
    Mob *GetMob();
    void SetMob(Mob *t);
    void CopyVisual(Tile &t);
    void SetMaterial(Material *t,int liqh=7);
    Material *GetMaterial(int h=0);
    bool IsLiquid(int ind);
    bool IsGas(int ind);
    bool IsSolid(int ind);
    bool IsLiquid();
    bool IsGas();
    bool IsSolid();
    bool walk();
    bool see();
    Material *GetTop();

    void AddEntity(IEntity *toadd);
    IEntity *GetEntity(int n=0){return EntityList[n];};
    int GetEntityCount(){return EntityList.size();};
    void RemoveEntity(int n){EntityList.erase(EntityList.begin()+n);};
    void RemoveEntity(IEntity *torem);
    void EntityTick();
    void EntityReset();
    void SaveEntitys(TCODZip &file);
    void LoadEntitys(TCODZip &file,VecLoad &bonusdata);
    IEntity *GetOfType(int &pos,CLASSES type);
    bool HasFlag(int flag);

    bool AddTop(Material *t);
    void RemoveMatter();
    void RemoveTop(){RemoveMatter();};

    int GetTemp(){return temp;};

    void AddTemp(int t);
    void SetTemp(int t);
    ItemStack *GetStack();
    void AddItem(IItem *t);
    void DropStackDown();
    void SaveStack(TCODZip &file);
    void LoadStack(TCODZip &file);

    bool PassibleLight();
    bool PassibleLight(SIDES s);
    bool PassibleSolid();
    bool PassibleSolid(SIDES s);
    bool PassibleLiquid();
    bool PassibleLiquid(SIDES s);

    void UpdTemps();
};
typedef std::vector <Tile *> tiles;
bool inset(Tile *t,tiles &set);

#endif // STTILE_H_INCLUDED
