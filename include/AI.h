#ifndef AI_H
#define AI_H
//#include "libtcod.hpp"
class AI;
#include "creature.h"
#include <vector>
#include <map>
#include "map.h"
#include "clpather.h"
struct ItemMemory
{
    IItem *it;
    ITile *pos;
};
typedef std::vector<ItemMemory> vItemMemory;
namespace ActionType
{
enum Types
{
    TYPE_OTHER,TYPE_MOVE,TYPE_SENSORY,TYPE_COMPLEX
};
};
class Event
{
    public:
        virtual ~Event(){};

};
class Action
{
    public:
        virtual ~Action(){};
        virtual bool Run(Creature *t)=0;
        virtual int GetType(){return ActionType::TYPE_OTHER;};
        virtual std::string GetName(){return "";};
};
class Action_Wait:public Action
{
    public:
        bool Run(Creature *t){return true;};
        std::string GetName(){return "Wait";};
};
typedef std::vector <Action *> vpAction;
typedef std::vector <Action> vAction;
class Action_Move:public Action
{
    public:
        bool Run(Creature *t);
        Action_Move(SIDES s);
        ITile *AfterMove(Creature *t);
        int GetType(){return ActionType::TYPE_MOVE;};
        std::string GetName();
    private:
        SIDES myside;
};
class Action_Dig:public Action
{
    public:
        bool Run(Creature *t);
        Action_Dig(SIDES s);
        ITile *AfterMove(Creature *t);
        int GetType(){return ActionType::TYPE_OTHER;};
        std::string GetName();
    private:
        SIDES myside;
};
class Action_LookAround_Items:public Action
{
    public:
        bool Run(Creature *t);
        Action_LookAround_Items(vItemMemory *memory);
        std::string GetName(){return "Look around for items";};
        int GetType(){return ActionType::TYPE_SENSORY;};
    private:
        vItemMemory *mytrgmem;
};

class Move_ToTile:public Action
{
    private:
        //TilePath mypath;
        ITile *trg;
    public:
        Move_ToTile(ITile *trg):trg(trg){};
        bool Run(Creature *t);
        bool Tick(Creature *t);
        std::string GetName(){return "Path and move to tile";};
        int GetType(){return ActionType::TYPE_COMPLEX;};
};
/*
class Action
{
    public:
        virtual ~Action(){};
        bool Run(Creature *t,int choice)=0;
        int CountChoices(Creature *t)=0;
};
class Action_MoveSide:public Action;
{
    public:
        bool Run(Creature *t,int choice);
        int CountChoices(Creature *t){return E_LAST;};
};
class Action_Take:public Action
{
    public:
        bool Run(Creature *t,int choice);
        int CountChoices(Creature *t);
};*/
//Memory
//  item:
//      afew of each type, seen
//actions               need
//  move(side)
//      allsides        side clear,passible
//  take                invspace(cur unlimited) knowledge of that item?
//      fromground?     reach
//  drop
//  usetool             tool/materials
//      pick
//      buildtools
//  opendoor
//  closedoor
//complex actions
//  move(totile)
//  buildsth(gather res, build)
//  mine
//reason
//  ??
//planing
//etc
//PVZ:
//build a wall at trg:
//build = usetool buildtool
//          req buildtool (not have:take(or search and take))
//          req material (not have: take(or search and take) or pick)
//          and so on...
//at trg: req path
//actions gal dar reiktu skirstit pgl gali daryt, turi daryt, ir pan
class AI
{
    public:
        AI(Creature *t);
        virtual ~AI();

        void AddPossibleAction(Action *t);
        virtual void Tick()=0;

    protected:
        Creature *mytrg;
        vpAction actions;

    private:

};
class AI_Human:public AI
{
    public:
        AI_Human(Creature *t,int *keytrg);
        void Tick();
        void AddActionKeyd(Action *t,int key);
    private:
        std::map <int,int> keybinds;
        int  *key;
};
class AI_Wander:public AI
{
    public:
        AI_Wander(Creature *t);
        void Tick();
    private:
        SIDES curside;
        TCODRandom rr;
};
class AI_Fighter:public AI
{
    public:
        AI_Fighter(Creature *t);
        void Tick();
    private:
        TCODRandom rr;
        SIDES curside;
};
#endif // AI_H
