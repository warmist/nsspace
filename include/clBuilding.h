#ifndef CLBUILDING_H
#define CLBUILDING_H
#include "material.h"
#include "IEntity.h"
class clBuilding;
#include "buildingTemplate.h"
#include "classEnums.h"
#define MAX_DROP 5000
// max drop jei portalas ish virsaus i apacia...
class clBuilding:public IEntity
{
public:
    clBuilding(TCODZip &file,VecLoad &bonusdata){Load(file,bonusdata);};
    clBuilding(BuildingTemplate *t,Material *mat);
    ~clBuilding();

    void Save(TCODZip &file);
    void Load(TCODZip &file,VecLoad &bonusdata);

    void SetTile(ITile *trg);
    ITile *GetTile();

    int GetPic();
    TCODColor GetColor();
    BuildingID GetBID();

    int GetPriority();
    void Tick();

    void Reset()
    {done=false;};
    bool IsDone()
    {return done;};
    bool IsDead();
    CLASSES GetClass(){return CL_BUILDING;};
    int GetFlags();
    int &GetData(){return data;};

protected:
private:
    int mypic;
    BuildingTemplate *t;
    Material *mat;
    ITile *mytile;
    int data;
    //ITiles turetu but!
    bool done;
};
clBuilding *GetBuildingOfType(ITile *t,BuildingID BID);
#endif // CLBUILDING_H
