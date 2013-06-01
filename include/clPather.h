#ifndef CLPATHER_H
#define CLPATHER_H
#include "ITile.h"
#include <vector>
#include "Map.h"

typedef std::vector <ITile *> TilePath;
typedef std::vector <SIDES> SidePath;
struct TileVertex
{
    ITile *a;
    unsigned dist;
    TileVertex *prev;
};
class clPather
{
    public:
        clPather(ITile *start,int r,ITile *end);//,Map *from);
        virtual ~clPather();
        TilePath Run();

        TilePath RunSolid();
    protected:
    private:
        TilePath RunBase();
        TilePath RunBaseNoDiag();
        void GenList();
        void GenListSolid();
        int FindVertex(ITile *bythis,std::vector <TileVertex*> &tt);
        int FindMinDist(std::vector <TileVertex*> &tt);
        ITile *st,*en;
        int r;
        //Map *from;
        std::vector <TileVertex *> graph;
};

#endif // CLPATHER_H
