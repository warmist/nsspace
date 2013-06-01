#include "clBuilding.h"
clBuilding *GetBuildingOfType(ITile *t,BuildingID BID)
{
    int a=0;
    IEntity *cc;
    clBuilding *ret;
    cc=t->GetOfType(a,CL_BUILDING);
            while (cc)
            {
                a++;
                ret=dynamic_cast<clBuilding*>(cc);
                if (ret->GetBID()==BID)
                {
                    return ret;
                }
                cc=t->GetOfType(a,CL_BUILDING);
            }
    return NULL;
}
clBuilding::clBuilding(BuildingTemplate *t,Material *mat):t(t),mat(mat)
{
    data=0;
    //ctor
}

clBuilding::~clBuilding()
{
    ITile *trg;
    if(!mytile)
        return;
    if(mytile->GetHeight()<0)
    {
        int i=0;
        i++;
        trg=mytile;//->GetSide(E_DOWN);
        while((trg->GetHeight()<0)&&(trg->GetSide(E_DOWN))&&(trg->GetSide(E_DOWN)->PassibleSolid()))
        {
            i++;
            trg=trg->GetSide(E_DOWN);
        }
        trg->AddTop(mat);
        return;
    }
    mytile->AddTop(mat);
    //dtor
}
BuildingID clBuilding::GetBID()
{
    return t->GetBuildingID();
}
int clBuilding::GetPriority()
{
    return t->GetPriority();
}
void clBuilding::Tick()
{
    t->Tick(mytile,data);
    mypic=t->GetPic(mytile,data);
    done=true;
}
void clBuilding::Save(TCODZip &file)
{
    file.putChar(CL_BUILDING);
    file.putInt(mytile->GetSave()->num);
    file.putInt(t->GetBuildingID());
    file.putInt(mypic);
    file.putInt(data);
    file.putInt(mat->ID);
}
void clBuilding::Load(TCODZip &file,VecLoad &bonusdata)
{
     mytile=GetFromLoad(file.getInt(),bonusdata);
    if(mytile==NULL)
        throw "Error loading file";
    int bid=file.getInt();
    t=BTManager::Get().GetTemplate((BuildingID)bid);
    if(t==NULL)
        throw "Error loading building template";
    mypic=file.getInt();
    data=file.getInt();
    int matid=file.getInt();
    mat=MaterialManager::Get().GetMaterial(MATT_ALL,matid);
}
void clBuilding::SetTile(ITile *trg)
{
    mytile=trg;
    if(trg)
    mypic=t->GetPic(trg,data);
}
int clBuilding::GetFlags()
{
    return t->GetFlags(data);
}
ITile *clBuilding::GetTile()
{
    return mytile;
}
int clBuilding::GetPic()
{
    //t->GetPic(mytile);
    return mypic;
//    t->GetPic();
}
TCODColor clBuilding::GetColor()
{
    return mat->color;
}
 bool clBuilding::IsDead()
{
    /*if(mytile->GetHeight()<0)
    {
    if(!mytile->GetSide(E_DOWN))
        return true;
    else
        if(mytile->GetSide(E_DOWN)->PassibleMatter())
            return true;
    }*/
    return mat->MeltTemp<mytile->GetTemp();
};
