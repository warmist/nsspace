#include "clPather.h"

clPather::clPather(ITile *start,int r,ITile *end):st(start),r(r),en(end)
{
    //ctor
}

clPather::~clPather()
{
    //dtor
}
int clPather::FindVertex(ITile *bythis,std::vector <TileVertex*> &tt)
{
    for(unsigned i=0;i<tt.size();i++)
        if(tt[i]->a==bythis)return i;
    return -1;
}
int clPather::FindMinDist(std::vector <TileVertex*> &tt)
{
    unsigned dist=tt[0]->dist;
    unsigned pos=0;
    for(unsigned i=0;i<tt.size();i++)
        if(tt[i]->dist<dist)
        {
            dist=tt[i]->dist;
            pos=i;
        }
    return pos;
}
void clPather::GenList()
{
    for(int i=0;i<graph.size();i++)
        delete graph[i];
    graph.clear();
    class Collector:public TileFunct
    {
        std::vector <TileVertex*> &mytrg;
        public:
        Collector(std::vector <TileVertex*> &trg):mytrg(trg)
        {
        }
        bool Run(ITile *t)
        {
            if(((t->GetHeight()<=5)&&(t->GetHeight()>=0))||((t->GetHeight()==-1)&&(t->GetSide(E_DOWN))&&(t->GetSide(E_DOWN)->GetHeight()>5)))
                {
                    TileVertex *a;
                    a=new TileVertex;
                    a->a=t;
                    a->dist=UINT_MAX;
                    a->prev=NULL;
                    mytrg.push_back(a);
                }
            return true;
        }
    }col(graph);
    ITile *u=st;
    int dr=0;
    while(u->GetSide(E_UP)&&(dr<r))
    {
        u=u->GetSide(E_UP);
        dr++;
        Map::RunCircle(r-dr,dynamic_cast<Tile*>(u),col);
    }
    u=st;
    dr=0;
    while(u->GetSide(E_DOWN)&&(dr<r))
    {
        u=u->GetSide(E_DOWN);
        dr++;
        Map::RunCircle(r-dr,dynamic_cast<Tile*>(u),col);
    }
    Map::RunCircle(r,dynamic_cast<Tile*>(st),col);
}
void clPather::GenListSolid()
{
    for(int i=0;i<graph.size();i++)
        delete graph[i];
    graph.clear();
    class Collector:public TileFunct
    {
        std::vector <TileVertex*> &mytrg;
        public:
        Collector(std::vector <TileVertex*> &trg):mytrg(trg)
        {
        }
        bool Run(ITile *t)
        {
            if(t->GetHeight()>5)
                {
                    TileVertex *a;
                    a=new TileVertex;
                    a->a=t;
                    a->dist=UINT_MAX;
                    a->prev=NULL;
                    mytrg.push_back(a);
                }
            return true;
        }
    }col(graph);
    ITile *u=st;
    int dr=0;
    while(u->GetSide(E_UP)&&(dr<r))
    {
        u=u->GetSide(E_UP);
        dr++;
        Map::RunCircle(r-dr,dynamic_cast<Tile*>(u),col);
    }
    u=st;
    dr=0;
    while(u->GetSide(E_DOWN)&&(dr<r))
    {
        u=u->GetSide(E_DOWN);
        dr++;
        Map::RunCircle(r-dr,dynamic_cast<Tile*>(u),col);
    }
    Map::RunCircle(r,dynamic_cast<Tile*>(st),col);
}

TilePath clPather::RunBaseNoDiag()
{
    TilePath ret;
    if(FindVertex(en,graph)==-1) return ret;
    int stpos=FindVertex(st,graph);
    if(stpos==-1)return ret;
    graph[stpos]->dist=0;
    std::vector <TileVertex*> Q;
    Q=graph;
    while(Q.size()>0)
    {
        int pos=FindMinDist(Q);
        TileVertex *u=Q[pos];
        if(u->a==en) //yeah cia bisky pagreitinimas...
        {
            Q.clear();
            break;
        }
        Q.erase(Q.begin()+pos);
        for(int i=0;SIDE_NODIAG[i]!=E_LAST;i++)
        {
            int vind=FindVertex(u->a->GetSide((SIDES)SIDE_NODIAG[i]),Q);
            //int vind=FindVertex(u->a->GetSide((SIDES)i),graph);
            unsigned alt=u->dist+1;
            if(vind!=-1)
            {
                TileVertex *v=Q[vind];
                if(alt<v->dist)
                {
                    v->dist=alt;
                    v->prev=u;
                }
            }
        }
    }
    TileVertex *cur=graph[FindVertex(en,graph)];
    while(cur->prev!=NULL)
    {
    ret.push_back(cur->a);
    cur=cur->prev;
    }

    for(int i=0;i<graph.size();i++)
        delete graph[i];
    graph.clear();
    return ret;
}
TilePath clPather::RunBase()
{
    TilePath ret;
    if(FindVertex(en,graph)==-1) return ret;
    int stpos=FindVertex(st,graph);
    if(stpos==-1)return ret;
    graph[stpos]->dist=0;
    std::vector <TileVertex*> Q;
    Q=graph;
    while(Q.size()>0)
    {
        int pos=FindMinDist(Q);
        TileVertex *u=Q[pos];

        if(u->a==en) //yeah cia bisky pagreitinimas...
        {
            Q.clear();
            break;
        }

        Q.erase(Q.begin()+pos);
        for(int i=E_UP;i<E_LAST;i++)
        {
            int vind=FindVertex(u->a->GetSide((SIDES)i),Q);
            //int vind=FindVertex(u->a->GetSide((SIDES)i),graph);
            unsigned alt=u->dist+1;
            if(vind!=-1)
            {
                TileVertex *v=Q[vind];
                if(alt<v->dist)
                {
                    v->dist=alt;
                    v->prev=u;
                }
            }
        }
    }
    TileVertex *cur=graph[FindVertex(en,graph)];
    while(cur->prev!=NULL)
    {
    ret.push_back(cur->a);
    cur=cur->prev;
    }

    for(int i=0;i<graph.size();i++)
        delete graph[i];
    graph.clear();
    return ret;
}
TilePath clPather::Run()
{
    TilePath ret;
    GenList();
    ret=RunBase();
    return ret;
}
TilePath clPather::RunSolid()
{
    TilePath ret;
    GenListSolid();
    ret=RunBaseNoDiag();
    return ret;
}
