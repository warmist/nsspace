#ifndef IENTITY_H
#define IENTITY_H
#include <vector>
#include "classEnums.h"
#include "libtcod.hpp"
class IEntity;
#include "ITile.h"
struct LoadData
{
    int num;
    int Exits[E_LAST];
    bool ExitsUsed[E_LAST];
    ITile *me;
};
namespace IENTITY_FLAGS
{
enum
{
    F_BLOCKS_LIGHT=1,F_BLOCKS_SOLID=2,F_BLOCKS_LIQUID=4,F_BLOCKS_MATTER=6,F_BLOCKS_ALL=7,F_FLOOR=8
};
}
typedef std::vector <LoadData> VecLoad;

class IEntity
{
    public:
        IEntity();
        virtual ~IEntity();
        virtual void Save(TCODZip &file)=0;
        virtual void Load(TCODZip &file,VecLoad &bonusdata)=0;

        virtual void SetTile(ITile *trg)=0;//pasirupint ish ano remove, i nauja add
        virtual ITile *GetTile()=0;

        virtual int GetPic()=0;
        virtual TCODColor GetColor()=0;

        virtual int GetPriority()=0;
        virtual void Tick(){};
        virtual void Reset(){};
        virtual bool IsDone(){return false;};
        virtual bool IsDead(){return false;};
        virtual int GetFlags(){return 0;};
        virtual CLASSES GetClass()=0;
    protected:
    private:
};
typedef std::vector<IEntity *> VecIEntity;
ITile *GetFromLoad(int n,VecLoad &ldata);

#endif // IENTITY_H
