#ifndef MAP_H
#define MAP_H
#include "libtcod.hpp"
#include <math.h>
#include "clmobile.h"
#include "stTile.h"
#include <fstream>
#include <list>
#include <limits.h>
#include "ISpecial.h"
#include "clBuilding.h"
#include "creature.h"
// mazai itakos turi (fps'am)
#define MODEL_ACCURATE

const int CONST_LAVA_TEMP=8000;
enum PLACE_TYPES
{
    E_PLACE_FOUNTAIN,E_PLACE_LAVA,E_PLACE_EATER
};
struct SpPlace
{
    ITile *t;
    PLACE_TYPES placetype;

};
bool MakeIndexed(ITile *upleft,int w,int h,ITiles &ret);
bool IsNormal(ITile *upleft,int w,int h);
bool IsNormal(ITile *upleft,int w,int h,ITiles &ret);
class Map
{
public:
    Map(std::string file,int &startile);
    Map(int w,int h);
    Map(int w,int h,int z);
    virtual ~Map();
    void PrintMap(TCODConsole *t,int x,int y,int w,int h,int r,Tile *st);
    void PrintMapXX(TCODConsole *t,int x,int y,int w,int h,int r,Tile *st);
    void PrintMapX(TCODConsole *t,int x,int y,int w,int h,int r,Tile *st);
    void PrintMapRot(TCODConsole *t,int x,int y,int w,int h,int r,Tile *st,int rot);
    Tile *GetSomeTile();
    void SetTemp(int temp)
    {
        for(unsigned i=0;i<map.size();i++)
            map[i]->temp=temp;
    }
    Tile *GetNum(int num)
    {
        return map[num];
    };
    int FindTile(Tile *a)
    {
        for (unsigned i=0;i<map.size();i++)
            if (map[i]==a)
                return i;
        return -1;
    }
    void SetTile(Tile *t,int stdtile);
    void SetMat(Tile *t,unsigned stdmat);
    void SetMat(Tile *t,unsigned stdmat,int h);
    unsigned LookupMat(Material *t);
    void RemoveTile(Tile *t);
    static void ConnectMap(tiles &mmap,int w,int h);

    void RandConnect(Tile *t,SIDES from)
    {
        t->exits[E_DOWN-from]=GetSomeTile();
    }
    void RandDoorConnect(Tile *t,SIDES from)
    {
        std::vector <int> ilist;
        for (unsigned i=0;i<map.size();i++)
            if (map[i]!=t)
                if (map[i]->doors!=NODOORS)
                    ilist.push_back(i);
        if (ilist.size()<1)
            return;
        int a=rand()%ilist.size();
        Tile *trg=map[ilist[a]];

        t->exits[E_DOWN-from]=trg->exits[E_DOWN-from];
        trg->exits[E_DOWN-from]->exits[from]=t;


        /*
        SIDES a=closest(from,true);
        SIDES b=closest(from,false);


        */

    }
    void InitStd();
    void InitStdTiles();
    void InitStdMat();
    int GetStdTileC()
    {
        return stdtiles.size();
    };
    void lqdSim();
    void RegTiles(tiles &list);
    void RegTile(Tile *t)
    {
        map.push_back(t);
    };
    void SaveMap(std::string path,int starttile);
    void LoadMap(std::string path,int &starttile);
    void SaveMapText(std::string path,int starttile);
    void LoadMapText(std::string path,int &starttile);
    void SaveMapZiped(std::string path,int starttile);
    void LoadMapZiped(std::string path,int &starttile);
    void LoadProcedural(TCODZip &file);
    tiles CollectEnclosed(tiles &closure);
    tiles FloodFill(Tile *st,TileFunct &ff);
    void ConnectClose(Tile *a,Tile *b,SIDES dir);
    void ConnectImmediate(Tile *a,Tile *b,SIDES dir);
    void AddPlace(ITile *trg, PLACE_TYPES t)
    {
        SpPlace a;
        a.t=trg;
        a.placetype=t;
        places.push_back(a);
    }
    static void RunCircle(int r,Tile *st,TileFunct &ff);
    void RunRandomRay(int r,SIDES to,Tile *st,RayFunct &ff,int prob);
    void ExplodeTile(Tile *trg);
    void ExplodeCircle(int r,Tile *cur);
    //Material *GetMat(int indx);
protected:
private:
    int m_w,m_h;
    tiles map;
    tiles stdtiles;
    //materialVec stdMaterials;
   // materialVec stdStones;
    //materialVec stdMetals;
    Material *water;
    //materialVec stdLiquids
    std::vector <SpPlace> places;
    TCODRandom *random;
};

#endif // MAP_H
