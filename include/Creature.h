#ifndef CREATURE_H
#define CREATURE_H
#include "IEntity.h"
class Creature;
#include "AI.h"
namespace CreatureFlags
{
enum F
{
    F_NONE=0,F_FALLING=1,F_FLYFORCED=2,F_NOCLIP=4
};
};
class Creature : public IEntity
{
    public:
        Creature();
        virtual ~Creature();

        void Save(TCODZip &file);
        void Load(TCODZip &file,VecLoad &bonusdata);

        virtual void SetTile(ITile *trg);
        ITile *GetTile();

        int GetPic();
        TCODColor GetColor();
        virtual void SetPic(int pic);
        virtual void SetColor(TCODColor color);
        int GetSpeed(){return myspeed;};
        void SetSpeed(int s){myspeed=s;};

        ItemStack &GetInventory(){return inventory;};

        ITile *TryMove(SIDES s);
        //void Move(ITile *to);
        void SetAi(AI *t);
        AI *GetAi();

        std::string GetName();
        void SetName(std::string name);

        int GetPriority(){return 100;};
        void SetCrFlags(int fl){flags=fl;};
        void Reset(){done=false;};
        bool IsDone(){return done;};
        virtual bool IsDead(){return false;};
        int GetFlags(){return IENTITY_FLAGS::F_BLOCKS_MATTER;};
        CLASSES GetClass(){return CL_CREATURE;};
        void Tick();
    protected:
        bool done;
        int mypic;
        int myspeed;
        TCODColor mycolor;
        std::string myname;
        ITile *myt;
        ItemStack inventory;
        int flags;
        AI *myai;
        int fallh;
        int curtick;
    private:

};
class Creature_Fighter:public Creature
{
    public:
        Creature_Fighter(ITile *t);
        ~Creature_Fighter();
        bool IsDead();
        void SetDead(bool dnew);
    private:
        bool dead;
};
class Creature_Snaky:public Creature
{
    public:
        Creature_Snaky(int l);
        ~Creature_Snaky();
        void SetPic(int pic);
        void SetPic(int pic,int alt);
        void SetColor(TCODColor color);
        void SetTile(ITile *trg);
    private:
        std::vector <Creature *> Tail;
        int myl;
};
#endif // CREATURE_H
