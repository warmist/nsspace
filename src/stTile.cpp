#include "stTile.h"
void Tile::CopyVisual(Tile &t)
{
    //r=t.r;
    //g=t.g;
    //b=t.b;
//    pic=t.pic;
//    walk=t.walk;
//    liqh=t.liqh;
    //t->SetMaterial(mymat);
//    mymat=t.GetMaterial();
    matterh=-1;
    for (int i=0;i<MAXMAT;i++)
    {
        mats[i]=t.mats[i];
        if (t.mats[i]!=NULL)
            matterh=i;
    }

}

Tile::Tile()
{
    for (int i=E_UP;i<E_LAST;i++)
        exits[i]=0;
//        liqh=0;
    doors=NODOORS;
    mymob=NULL;
    //mymat=NULL;
    for (int i=0;i<MAXMAT;i++)
        mats[i]=NULL;
    temp=21;
    matterh=-1;
    //myspec=NULL;
    stack=NULL;
}
Tile::Tile(Material *t)
{
    for (int i=E_UP;i<E_LAST;i++)
        exits[i]=0;
    for (int i=0;i<MAXMAT;i++)
        mats[i]=t;
    matterh=MAXMAT-1;
//        liqh=7;
    doors=NODOORS;
    mymob=NULL;
    temp=21;
    //myspec=NULL;
    stack=NULL;
}
void Tile::DropStackDown()
{
    //ItemStack *b=exits[E_DOWN]->GetStack();

    for (int i=0;i<stack->GetCount();i++)
    {
        exits[E_DOWN]->AddItem(stack->GetItem(i));
    }
    delete stack;
    stack=NULL;
}
int Tile::GetHeight()
{
    /* for (int i=MAXMAT-1;i>=0;i--)
         if (mats[i]) return i;
     return -1;*/
    return matterh;
}
void Tile::SetHeight(int h,Material *t)
{
    h--;
    matterh=h;
    for (int i=0;i<MAXMAT;i++)
        mats[i]=NULL;
    for (int i=0;i<=h;i++)

        mats[i]=t;
}
int Tile::GetSolidH()
{
    for (int i=MAXMAT-1;i>=0;i--)
        if (mats[i])
            if (IsSolid(i))
                return i;
    return -1;
}
int Tile::GetDeepness()
{
    int h=0;
    for (int i=MAXMAT-1;i>=0;i--)
        if (mats[i])
            if (IsLiquid(i))
                h++;

    return h;
}
void Tile::TransferMassForced(ITile *t)
{
    if (matterh<0) return ;
    if (//(t->GetHeight()<myh)&&
        (t->GetHeight()<MAXMAT+1))
    {
        TransferTemp(t);
        t->AddTop(mats[matterh]);
        RemoveMatter();

    }
    if(stack)
        for(unsigned i=0;i<stack->GetCount();i++)
        {
            IItem *it=stack->GetItem(i);
            if(it->GetSize()<=SIZE_CAT)
            {
                stack->RemoveItem(i);
                t->AddItem(it);
                return;
            }
        }
}
void Tile::TransferMass(ITile *t)
{
    int &myh=matterh;//GetHeight();
    if (myh<0) return ;
    if (//(t->GetHeight()<myh)&&
        (t->GetHeight()<MAXMAT+1))
        if (IsLiquid(myh)||IsGas(myh))
        {
            TransferTemp(t);
            //t->mats[t->GetHeight()+1]=mats[myh];
            t->AddTop(mats[myh]);
            RemoveMatter();
            //if (myh==0)temp=0;//actual ambient temp? neturetu but svarbu nes VV metodas taip veikia
        }
    if(stack)
        for(unsigned i=0;i<stack->GetCount();i++)
        {
            IItem *it=stack->GetItem(i);
            if(it->GetSize()<=SIZE_CAT)
            {
                stack->RemoveItem(i);
                t->AddItem(it);
                return;
            }
        }
}
void Tile::TransferTemp(ITile *t)
{

    int gh=t->GetHeight();// gaunama nuo -1 (nera medz) iki MAXMAT-1?
    //int wtemp=t->temp*(gh+1)+temp;
    //metodas 1:
    //minusai->kazur nyksta temp...
    //pliusai->gana greitas?
    t->SetTemp(t->GetTemp()*(gh+1)+temp);// jei nieko nebuvo tai gh+1==0 ir poto gh+2==1 => tik gauta temp lieka.


    /*if(t->temp%(gh+2)>=(gh/2+1))
        {
        t->temp/=gh+2; //gali but apvalinimo klaidu??
        t->temp++;
        }
    else
        {
        t->temp/=gh+2; //gali but apvalinimo klaidu??
        }*/
    t->SetTemp(t->GetTemp()/(gh+2));
    //temp-=temp/GetHeight();
}
void Tile::UpdTemps()
{
    if (GetStack())
    {
        GetStack()->SetTemp(temp);
        if (GetStack()->BrakeAll())
            StackAndPack();
    }

}
void Tile::AddTemp(int t)
{

    temp+=t;
    UpdTemps();
}
void Tile::SetTemp(int t)
{
    temp=t;
    UpdTemps();

}
void Tile::Sort()
{
    for (int i=0;i<MAXMAT;i++)
        for (int j=i+1;j<MAXMAT;j++)
        {
            Material *mm;
            if ((IsLiquid(i)&&IsSolid(j))||(IsGas(i)&&(IsLiquid(j)||IsSolid(j))))
            {
                mm=mats[i];
                mats[i]=mats[j];
                mats[j]=mm;
            }
        }
}
TCODColor Tile::GetColor()
{
    int h=GetHeight();
    if (h<0)
        return TCODColor::black;
    return mats[h]->color;
}
int Tile::GetPic()
{
    const unsigned int LEVEL_PIX[]={250,',','_','-',223,176,177,178};

    int h=GetHeight(); //performance!!!
    if (h<0)
        return ' ';
    if (IsSolid(h))
        return LEVEL_PIX[h];
    if (IsLiquid(h))
        //return '0'+h+1;
        return (h%2)?126:184;
    if (IsGas(h))
        if (h>2)
            return '%';
        else
            return '.';
    return ' ';
}

void Tile::Remove()
{
    for (int i=E_UP;i<E_LAST;i++)
        if (exits[i])exits[i]->exits[E_DOWN-i]=exits[E_DOWN-i];
}
void Tile::RemoveVoid()
{

    for (int i=E_UP;i<E_LAST;i++)
        if (exits[i])
            exits[i]->exits[E_DOWN-i]=NULL;
}
Tile *Tile::GetSide(SIDES s)
{
    return exits[s];
}
void Tile::SetSide(ITile *t,SIDES s)
{
    exits[s]=dynamic_cast<Tile*>(t);
}
Mob *Tile::GetMob()
{
    return mymob;
}
void Tile::SetMob(Mob *t)
{
    mymob=t;
}
void Tile::SetMaterial(Material *t,int liqh)
{
    //for (int i=0;i<liqh;i++)
    mats[liqh]=t;
    if (t!=NULL)
    {
        if (liqh>matterh)matterh=liqh;
    }
    else
    {
        if (liqh==matterh)matterh--;
    }
    //mymat=t;
};
Material *Tile::GetMaterial(int h)
{
    return mats[h];
    // return mymat;
};

bool Tile::IsLiquid(int ind)
{
    if (!mats[ind])return false;
    return (temp >mats[ind]->MeltTemp)&&(temp<mats[ind]->BoilTemp);
}
bool Tile::IsGas(int ind)
{
    if (!mats[ind])return false;
    return (temp>=mats[ind]->BoilTemp);
}
bool Tile::IsSolid(int ind)
{
    if (!mats[ind])return false;
    return (temp<=mats[ind]->MeltTemp);
}
bool Tile::IsLiquid()
{
    int top=GetHeight();
    if (top<0)
        return false;
    return IsLiquid(top);
    //   return (temp > mymat->MeltTemp)&&(temp<mymat->BoilTemp);
};
bool Tile::IsGas()
{
    int top=GetHeight();
    if (top<0)
        return false;
    return IsGas(top);
    //return (temp>=mymat->BoilTemp);
};
bool Tile::IsSolid()
{
    int top=GetHeight();
    if (top<0)
        return false;
    return IsSolid(top);
    //return (temp<=mymat->MeltTemp);
};
bool Tile::walk()
{
    if (doors==DOORSCLOSED)return false;
    int h=GetHeight();
    if (h<0)
        if ((exits[E_DOWN])&&(!exits[E_DOWN]->walk()))
            return true;
    return !(IsSolid()&&(GetHeight()>5));
    //return !(IsSolid()&&(liqh>5));
}
bool Tile::see()
{
    if (doors==DOORSCLOSED)return false;
    return (GetSolidH()<5);
    //return !(IsSolid()&&(liqh>4));
}
Material *Tile::GetTop()
{
    int h=GetHeight();
    if (h>=0)
        return mats[h];
    return NULL;
}
IEntity *Tile::GetOfType(int &pos,CLASSES type)
{
    for (unsigned i=pos;i<EntityList.size();i++)
    {
        if (EntityList[i]->GetClass()==type)
        {
            pos=i;
            return EntityList[i];
        }
    }
    return NULL;
}
void Tile::AddEntity(IEntity *toadd)
{
    int prior=toadd->GetPriority();
    for (VecIEntity::iterator i=EntityList.begin();i!=EntityList.end();i++)
    {
        if ((*i)->GetPriority()<prior)
        {
            EntityList.insert(i,toadd);
            return;
        }
    }
    EntityList.push_back(toadd);
}
bool Tile::AddTop(Material *t)
{

    if (matterh>=MAXMAT-1)
        return false;
    matterh++;
    mats[matterh]=t;

    return true;
}
void Tile::RemoveMatter()
{
    mats[matterh]=NULL;
    matterh--;
}
void Tile::AddItem(IItem *t)
{
    if (stack==NULL)
        stack=new ItemStack();
    stack->AddItem(t);
}
ItemStack *Tile::GetStack()
{
    //if(stack==NULL)
    //    stack=new ItemStack();//ItemStack(this)?
    if ((stack!=NULL)&&
            (stack->GetCount()==0))
    {
        delete stack;
        stack=NULL;
    }
    return stack;
}
void Tile::RemoveEntity(IEntity *torem)
{
    for (VecIEntity::iterator i=EntityList.begin();i!=EntityList.end();i++)
        if (torem==*i)
        {
            EntityList.erase(i);
            return;
        }
}
void  Tile::EntityTick()
{
    //PERDARYT kad veiktu...
    /*for (VecIEntity::reverse_iterator i=EntityList.rbegin();i!=EntityList.rend();i++)
    {

        (*i)->Tick();
        if((*i)->IsDead())
        {
            delete *i;
            EntityList.erase(i.base());
        }
        }
    }*/
    if (EntityList.size()==0)
        return;
    for(unsigned i=0;i<EntityList.size();i++)
        EntityList[i]->Tick();

    unsigned i=0;
    while(i<EntityList.size())
    {
        if (EntityList[i]->IsDead())
        {
            delete EntityList[i];
            EntityList.erase(EntityList.begin()+i);
        }
        else
        i++;
    }


}
void  Tile::EntityReset()
{
    for (VecIEntity::iterator i=EntityList.begin();i!=EntityList.end();i++)
        (*i)->Reset();
}
void Tile::SaveStack(TCODZip &file)
{
    if (GetStack())
    {
        file.putChar(1);
        stack->Save(file);
    }
    else
        file.putChar(0);
}
void Tile::LoadStack(TCODZip &file)
{
    char c=file.getChar();
    if (c)
    {
        if (!stack)
            stack=new ItemStack();
        stack->Load(file);
    }
    else
    {
        if (stack)
            delete stack;
        stack=NULL;
    }
}
bool inset(Tile *t,tiles &set)
{
    return set.end()!=find(set.begin(),set.end(),t);
};
void Tile::SaveEntitys(TCODZip &file)
{
    file.putInt(EntityList.size());
    for (unsigned i=0;i<EntityList.size();i++)
        EntityList[i]->Save(file);
}
bool Tile::HasFlag(int flag)
{
     if (EntityList.size()>0)
    {
        for (unsigned i=0;i<EntityList.size();i++)
            if ((EntityList[i]->GetFlags()&flag)==flag)
                return true;
    }
    return false;
}
bool Tile::PassibleLight()
{
    /*if (EntityList.size()>0)
    {
        for (int i=0;i<EntityList.size();i++)
            if (EntityList[i]->GetFlags()&IENTITY_FLAGS::F_BLOCKS_LIGHT)
                return false;
    }*/
    if(HasFlag(IENTITY_FLAGS::F_BLOCKS_LIGHT))
    if(!HasFlag(IENTITY_FLAGS::F_FLOOR))
        return false;
    return GetSolidH()<=4;
}
bool Tile::PassibleSolid()
{
    /*if (EntityList.size()>0)
    {
        for (int i=0;i<EntityList.size();i++)
            if (EntityList[i]->GetFlags()&IENTITY_FLAGS::F_BLOCKS_MATTER)
                return false;
    }*/
    if(HasFlag(IENTITY_FLAGS::F_BLOCKS_SOLID))
    if(!HasFlag(IENTITY_FLAGS::F_FLOOR))
        return false;
    return GetSolidH()<=5;
}
bool Tile::PassibleLiquid()
{
    /*if (EntityList.size()>0)
    {
        for (int i=0;i<EntityList.size();i++)
            if (EntityList[i]->GetFlags()&IENTITY_FLAGS::F_BLOCKS_MATTER)
                return false;
    }*/
    if(HasFlag(IENTITY_FLAGS::F_BLOCKS_LIQUID))
    if(!HasFlag(IENTITY_FLAGS::F_FLOOR))
        return false;
    return GetSolidH()<=5;
}
bool Tile::PassibleLight(SIDES s)
{
    if (!exits[s])
        return false;
    switch (s)
    {
    case E_UP:
        //return exits[s]->PassibleLight();// patikrint ar neturi grindu
        if(exits[s]->HasFlag(IENTITY_FLAGS::F_BLOCKS_LIGHT|IENTITY_FLAGS::F_FLOOR))return false;
        break;
    case E_DOWN:
    {
       if(HasFlag(IENTITY_FLAGS::F_BLOCKS_LIGHT|IENTITY_FLAGS::F_FLOOR))return false;
        //return exits[s]->PassibleLight();
    }
    break;
    }
    return exits[s]->PassibleLight();
}
bool Tile::PassibleSolid(SIDES s)
{
    if (!exits[s])
        return false;
    switch (s)
    {
    case E_UP:
        //return exits[s]->PassibleLight();// patikrint ar neturi grindu
        if(exits[s]->HasFlag(IENTITY_FLAGS::F_BLOCKS_SOLID|IENTITY_FLAGS::F_FLOOR))return false;
        break;
    case E_DOWN:
    {
       if(HasFlag(IENTITY_FLAGS::F_BLOCKS_SOLID|IENTITY_FLAGS::F_FLOOR))return false;
        //return exits[s]->PassibleLight();
    }
    break;
    }
    return exits[s]->PassibleSolid();
}
bool Tile::PassibleLiquid(SIDES s)
{
    if (!exits[s])
        return false;
    switch (s)
    {
    case E_UP:
        //return exits[s]->PassibleLight();// patikrint ar neturi grindu
        if(exits[s]->HasFlag(IENTITY_FLAGS::F_BLOCKS_LIQUID|IENTITY_FLAGS::F_FLOOR))return false;
        break;
    case E_DOWN:
    {
       if(HasFlag(IENTITY_FLAGS::F_BLOCKS_LIQUID|IENTITY_FLAGS::F_FLOOR))return false;
        //return exits[s]->PassibleLight();
    }
    break;
    }
    return exits[s]->PassibleLiquid();
}
