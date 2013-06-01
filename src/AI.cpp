#include "AI.h"

AI::AI(Creature *t):mytrg(t)
{
    t->SetAi(this);
    //ctor
}
void AI::AddPossibleAction(Action *t)
{
    actions.push_back(t);
}

AI::~AI()
{
    for(int i=0;i<actions.size();i++)
        delete actions[i];
    //dtor
}


bool Action_Move::Run(Creature *t)
{
     if(t->TryMove(myside))
        {
            t->SetTile(t->TryMove(myside));
            return true;
        }
    return false;
}
Action_Move::Action_Move(SIDES s)
{
    myside=s;
}
ITile *Action_Move::AfterMove(Creature *t)
{
    return t->TryMove(myside);
}
std::string Action_Move::GetName()
{
    //E_UP,E_NW,E_N,E_NE,E_E,E_W,E_SW,E_S,E_SE,E_DOWN,E_LAST
    const std::string names[]={"up","nw","n","ne","e","w","sw","s","se","down"};
    std::string name;
    name+="move ";
    name+=names[myside];
    return name;
}

bool Action_Dig::Run(Creature *t)
{
    return Dig(t->GetTile(),myside);
}
Action_Dig::Action_Dig(SIDES s)
{
    myside=s;
}
ITile *Action_Dig::AfterMove(Creature *t)
{
    return t->TryMove(myside);
}
std::string Action_Dig::GetName()
{
    //E_UP,E_NW,E_N,E_NE,E_E,E_W,E_SW,E_S,E_SE,E_DOWN,E_LAST
    const std::string names[]={"up","nw","n","ne","e","w","sw","s","se","down"};
    std::string name;
    name+="move ";
    name+=names[myside];
    return name;
}

/*
bool Action_MoveSide::Run(Creature *t,int choice)
{
    if(t->TryMove((SIDES)choice))
        {
            t->SetTile(t->GetTile()->GetSide((SIDES)choice));
            return true;
        }
    return false;
}
bool Action_Take::Run(Creature *t,int choice)
{
    ItemStack *st=t->GetTile()->GetStack();
    IItem *o=st->GetItem(choice);
    if(o)
        {
            t->GetInventory().AddItem(o);
            st->RemoveItem(choice);
            return true;
        }
    return false;
}
int Action_Take::CountChoices(Creature *t)
{
    ItemStack *st=t->GetTile()->GetStack();
    if(st)
        return st->GetCount();
    else
    return 0;
}
*/
AI_Human::AI_Human(Creature *t,int *keytrg):AI(t),key(keytrg)
{

}
void AI_Human::Tick()
{
    int i=0;

    if(keybinds.find(*key)!=keybinds.end())
    {
        actions[keybinds[*key]]->Run(mytrg);
    }
};

void AI_Human::AddActionKeyd(Action *t,int key)
{
    AddPossibleAction(t);
    keybinds[key]=actions.size()-1;
}
AI_Wander::AI_Wander(Creature *t):AI(t)
{
    for(int i=E_UP;i<=E_DOWN;i++)
        AddPossibleAction(new Action_Move((SIDES)i));
    for(int i=E_UP;i<=E_DOWN;i++)
        AddPossibleAction(new Action_Dig((SIDES)i));
    curside=E_N;
}
void AI_Wander::Tick()
{
    if(rr.getInt(0,100)>33)
    {
        curside=closest(curside,rr.getInt(0,1));
    }
    if(!actions[curside]->Run(mytrg))
        //curside=closest(curside,rr.getInt(0,1));
            if(!actions[curside+E_LAST]->Run(mytrg));
                curside=closest(curside,rr.getInt(0,1));
}
bool Action_LookAround_Items::Run(Creature *t)
{
        class boomitems:public TileFunct
        {
            vItemMemory *pop;
            public:
                boomitems(vItemMemory *trg):pop(trg){};
                bool Run(ITile *t)
                {
                    ItemStack *sst=t->GetStack();
                    if(sst)
                    {
                        ItemMemory itmem;
                        int count=sst->GetCount();
                        for(int i=0;i<count;i++)
                            {
                                itmem.pos=t;
                                itmem.it=sst->GetItem(i);
                                pop->push_back(itmem);
                            }

                    }
                    return true;
                }
        }bbb(mytrgmem);
        Map::RunCircle(8,dynamic_cast<Tile*>(t->GetTile()),bbb);
        return true;
}
Action_LookAround_Items::Action_LookAround_Items(vItemMemory *memory):mytrgmem(memory)
{

}
bool Move_ToTile::Run(Creature *t)
{

}
bool Move_ToTile::Tick(Creature *t)
{

}
AI_Fighter::AI_Fighter(Creature *t):AI(t)
{
    for(int i=E_UP;i<=E_DOWN;i++)
        AddPossibleAction(new Action_Move((SIDES)i));
    curside=(SIDES)2;
}
void AI_Fighter::Tick()
{
    if(TCODRandom::getInstance()->getInt(0,100)>33)
    {
        curside=closest(curside,TCODRandom::getInstance()->getInt(0,1));//rr.getInt(0,1));
    }
    if(!actions[curside]->Run(mytrg))
        //curside=closest(curside,rr.getInt(0,1));
           {
               int a=0;
               ITile *t=mytrg->GetTile()->GetSide(curside);
               if(!t)
               return;
               IEntity *tt=t->GetOfType(a,CL_CREATURE);
               if(tt)
               {
                   Creature *p=dynamic_cast<Creature*>(tt);
                   if(p->GetPic()=='f')
                   {
                       Creature_Fighter *pp=dynamic_cast<Creature_Fighter*>(p);
                       pp->SetDead(true);
                   }
               }
           }
}
