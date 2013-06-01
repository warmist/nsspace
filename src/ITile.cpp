#include "ITile.h"



ITile::ITile()
{
    //ctor
}

ITile::~ITile()
{
    //dtor
}

SIDES closest(SIDES b,bool ccw)
{
    //                  E_UP,E_NW,E_N ,E_NE,E_E ,E_W ,E_SW,E_S ,E_SE,E_DOWN,E_LAST
    const int arrCCW[]={E_UP,E_W ,E_NW,E_N ,E_NE,E_SW,E_S ,E_SE,E_E,E_DOWN};
    const int arrCW[]= {E_UP,E_N ,E_NE,E_E ,E_SE,E_NW,E_W ,E_SW,E_S,E_DOWN};
    if (ccw)return (SIDES)arrCCW[(int)b];
    else
        return (SIDES)arrCW[(int)b];
}

void ITile::StackAndPack()
{
    ItemStack *st=GetStack();
    if (st)
    {
        st->StackAll();


        Material *raw=st->MakesRaw();
        while (raw)
        {
            AddTop(raw);
            raw=st->MakesRaw();
        }
        st->UnstackSingles();
    }
}
bool Dig(ITile *from,SIDES to)
{
    TCODRandom rr;
    ITile *t=from->GetSide(to);
    if(!t)
        return false;
    SIDES back=(SIDES)(E_DOWN-to);
    int blast[]={back,closest(back,false),closest(back,true)};
    if (t->GetSolidH()<0)
        return false;
    //if (!t->IsSolid(t->GetHeight()))
    //    return;
    Material *top=t->GetTop();
    if(t->GetTop()->MeltTemp<t->GetTemp())
        return false;
    int i=10;
    while (i>0)
    {
        int r=rr.getInt(0,1);
        if (r==0)
        {
            t->AddItem(new RawMaterial(top,SIZE_BACKPACK));
            i--;
            Material *raw=t->GetStack()->MakesRaw();
            if (raw)
                t->AddTop(raw);
        }
        else
        {

            r=rr.getInt(0,2);
            ITile *ss=t->GetSide((SIDES)blast[r]);
            if (ss&&ss->PassibleSolid())
            {
                ss->AddItem(new RawMaterial(top,SIZE_BACKPACK));
                Material *raw=ss->GetStack()->MakesRaw();
                if (raw)
                    ss->AddTop(raw);
                i--;
            }
        }
    }

    t->RemoveTop();
    return true;
}
