#include "Creature.h"

Creature::Creature()
{
    myt=NULL;
    myai=NULL;
    //ctor
    fallh=0;
    myspeed=1;
    curtick=0;
    flags=CreatureFlags::F_NOCLIP;
}
void Creature::SetAi(AI *t)
{
    myai=t;
}
AI *Creature::GetAi()
{
    return myai;
}
Creature::~Creature()
{
    if(myai)
    delete myai;
    //dtor
}
void Creature::Tick()
{
    if(done)return;
    done=true;

    if((flags&CreatureFlags::F_FALLING)&(!(flags&CreatureFlags::F_NOCLIP)))
    {
        if((myt->PassibleSolid(E_DOWN)))
        {
            SetTile(myt->GetSide(E_DOWN));
            fallh++;
        }
        else
        {
            flags^=CreatureFlags::F_FALLING;
            //do dmg?
            fallh=0;
        }
    }
    else
    {
        if(myt->PassibleSolid(E_DOWN))
        {
            flags^=CreatureFlags::F_FALLING;
        }
        //AITICK
        if(myai)
        {
        if(curtick==myspeed)
        {
            myai->Tick();
            curtick=0;
        }
        else
        {
            curtick++;
        }
        }
    }
}
void Creature::Save(TCODZip &file)
{
    file.putInt(CL_CREATURE);
    file.putInt(mypic);
    file.putInt(mycolor.r);
    file.putInt(mycolor.b);
    file.putInt(mycolor.g);
    file.putString(myname.c_str());
    file.putInt(myt->GetSave()->num);
    file.putInt(flags);
    inventory.Save(file);
}
void Creature::Load(TCODZip &file,VecLoad &bonusdata)
{
    mypic=file.getInt();
    mycolor.r=file.getInt();
    mycolor.g=file.getInt();
    mycolor.b=file.getInt();
    myname=file.getString();
    myt=GetFromLoad(file.getInt(),bonusdata);
    if(!myt)
        throw "Tile not found!!";
    flags=file.getInt();
    inventory.Load(file);
}
ITile *Creature::TryMove(SIDES s)
{
    if(!myt->GetSide(s))
        return NULL;
    if(flags&CreatureFlags::F_NOCLIP)
        return myt->GetSide(s);
    if(myt->PassibleSolid(s))
        return myt->GetSide(s);
    if(myt->GetHeight()>=5)
       // if(myt->GetSide(s)->GetSide(E_UP))
            if(myt->GetSide(s)->PassibleSolid(E_UP))
                return myt->GetSide(s)->GetSide(E_UP);
    return false;
}
void Creature::SetTile(ITile *trg)
{
    if(myt)
        myt->RemoveEntity(this);
    myt=trg;
    if(myt)
        myt->AddEntity(this);
}
ITile *Creature::GetTile()
{
    return myt;
}
int Creature::GetPic()
{
    return mypic;
}
TCODColor Creature::GetColor()
{
    return mycolor;
}
void Creature::SetPic(int pic)
{
    mypic=pic;
}
void Creature::SetColor(TCODColor color)
{
    mycolor=color;
}
std::string Creature::GetName()
{
    return myname;
}
void Creature::SetName(std::string name)
{
    myname=name;
}
Creature_Snaky::Creature_Snaky(int l):myl(l)
{
    for(int i=0;i<l;i++)
    {
        Tail.push_back(new Creature);
    }
}
Creature_Snaky::~Creature_Snaky()
{
    for(int i=0;i<myl;i++)
        delete Tail[i];
}
void Creature_Snaky::SetPic(int pic,int alt)
{
    Creature::SetPic(pic);
    for(int i=0;i<myl;i++)
        Tail[i]->SetPic(alt);
}
void Creature_Snaky::SetPic(int pic)
{
    Creature::SetPic(pic);
    for(int i=0;i<myl;i++)
        Tail[i]->SetPic(pic);
}
void Creature_Snaky::SetColor(TCODColor color)
{
    Creature::SetColor(color);
    for(int i=0;i<myl;i++)
        Tail[i]->SetColor(TCODColor::lerp(color,TCODColor::darkRed,(float)i/(float)myl));
}
void Creature_Snaky::SetTile(ITile *trg)
{
    ITile *t=myt;
    ITile *lt;
    Creature::SetTile(trg);
    for(int i=0;i<myl;i++)
    {
        lt=Tail[i]->GetTile();
        Tail[i]->SetTile(t);
        t=lt;
    }
}
Creature_Fighter::Creature_Fighter(ITile *t):Creature()
{
    SetPic('f');
    SetColor(TCODColor::orange);
    SetTile(t);
    dead=false;
    flags=0;
    new AI_Fighter(this);
}
Creature_Fighter::~Creature_Fighter()
{
    //TCODRandom rr;
    if(myt)
    {
        myt->AddItem(new RawMaterial(MaterialManager::Get().GetRandom(TCODRandom::getInstance(),MATT_FLESH),SIZE_BACKPACK));
        myt->AddItem(new RawMaterial(MaterialManager::Get().GetRandom(TCODRandom::getInstance(),MATT_BLOOD),SIZE_BACKPACK));
        myt->StackAndPack();
    }
}
bool Creature_Fighter::IsDead()
{
    return dead;
}
void Creature_Fighter::SetDead(bool dnew)
{
    dead=dnew;
}
