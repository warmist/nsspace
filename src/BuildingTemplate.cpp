#include "BuildingTemplate.h"
BuildingTemplate *BTManager::GetTemplate(BuildingID id)
{
    return arr[id];

}
BTManager &BTManager::Get()
{
    static BTManager me;
    return me;
}
BTManager::BTManager()
{
    arr[BID_WALL]=new Build_Wall;
    arr[BID_FLOOR]=new Build_Floor;
    arr[BID_DOOR]=new Build_Door;
    arr[BID_WINDOW]=new Build_Window;
    arr[BID_GRATE]=new Build_Grate;
    //....
}
BTManager::~BTManager()
{
    for (int i=0;i<BID_LAST;i++)
        delete arr[i];
}
int Build_Wall::GetPic(ITile *t,int &data)
{
    int occ=occupied(t);

    //                0  1   2   3   4   5   6   7   8   9  10 11 12 13 14 15
    const int pics[]={9,9,9,186,9,200,201,204,9,188,187,185,205,202,203,206};
    return pics[occ];
}
int Build_Wall::occupied(ITile *t)
{
    int ret=0;
    int a=0;
    IEntity *cc;
    ITile *trg;
    trg=t->GetSide(E_N);
    if (trg)
    {
        if (!trg->PassibleSolid())
        {
            ret|=SO_N;
        }
        else
        {
            cc=trg->GetOfType(a,CL_BUILDING);//perasyt su funkcija GetBuildingOfType
            while (cc)
            {
                a++;
                if ((dynamic_cast<clBuilding*>(cc))->GetBID()==BID_WALL)
                {
                    ret|=SO_N;
                    break;
                }
                cc=trg->GetOfType(a,CL_BUILDING);
            }
        }
    }
    a=0;
    trg=t->GetSide(E_S);
    if (trg)
    {
        if (!trg->PassibleSolid())
        {
            ret|=SO_S;
        }
        else
        {
            cc=trg->GetOfType(a,CL_BUILDING);
            while (cc)
            {
                a++;
                if ((dynamic_cast<clBuilding*>(cc))->GetBID()==BID_WALL)
                {
                    ret|=SO_S;
                    break;
                }
                cc=trg->GetOfType(a,CL_BUILDING);
            }

        }
    }
    a=0;
    trg=t->GetSide(E_E);
    if (trg)
    {
        if (!trg->PassibleSolid())
        {
            ret|=SO_E;
        }
        else
        {
            cc=trg->GetOfType(a,CL_BUILDING);
            while (cc)
            {
                a++;
                if ((dynamic_cast<clBuilding*>(cc))->GetBID()==BID_WALL)
                {
                    ret|=SO_E;
                    break;
                }
                cc=trg->GetOfType(a,CL_BUILDING);
            }

        }
    }
    a=0;
    trg=t->GetSide(E_W);
    if (trg)
    {
        if (!trg->PassibleSolid())
        {
            ret|=SO_W;
        }
        else
        {
            cc=trg->GetOfType(a,CL_BUILDING);
            while (cc)
            {
                a++;
                if ((dynamic_cast<clBuilding*>(cc))->GetBID()==BID_WALL)
                {
                    ret|=SO_W;
                    break;
                }
                cc=trg->GetOfType(a,CL_BUILDING);
            }

        }
    }
    a=0;
    return ret;
}
