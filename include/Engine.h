#ifndef ENGINE_H
#define ENGINE_H

#include "libtcod.hpp"
#include "map.h"
#include <fstream>
#include "version.h"
#include "clPather.h"
#include "Creature.h"
class Engine
{
public:
    Engine();
    virtual ~Engine();
    void Run();
    void TileEd(char key,Tile *cur);
    Tile *Intro();
    void UpdInventory(ItemStack *t,ItemStack *gr);
protected:
private:
    Tile *TryMove(Tile *from,Tile *to);
    void UseTool(Tile *from,SIDES to);
    std::string mapname;
    Map *mymap;
    TCODConsole *main;
    TCODConsole *editor;
    TCODConsole *inventory;
    TCODConsole *onground;
    Tile *marked;
    bool Mode_Debug;
    bool Mode_NoClip;
    bool Mode_PauseSim;
    bool Editor_on;
    TCODRandom random;
    int CurrentItem;
    ItemTool *curtool;
    struct Config
    {
        int w,h;
        int mw,mh;
        int fps;
    }mycfg;
    Creature player;
    bool ControlToSide(char c,SIDES &s);
    BuildingID curbuild;
    bool cred;
    void GameTick();
    int R;
    int CurrentTile;
};

#endif // ENGINE_H
