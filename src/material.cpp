#include "material.h"
float OptRez(float rr)
{
    return exp(-rr/2000.0f);
}
void Material::Save(TCODZip &file)
{
    file.putChar(color.r);
    file.putChar(color.g);
    file.putChar(color.b);
    file.putInt(MeltTemp);
    file.putInt(BoilTemp);
    //file.putInt(ResistTemp);
    file.putFloat(ResistTemp);
    file.putInt(IgniteTemp);
    file.putInt(BurnTemp);
    file.putInt(value);
    file.putString(name.c_str());
    file.putInt(flags);
    file.putInt((int)mytype);
    file.putInt(ID);
}
void Material::Load(TCODZip &file)
{
    color.r=file.getChar();
    color.g=file.getChar();
    color.b=file.getChar();
    MeltTemp=file.getInt();
    BoilTemp=file.getInt();
    //ResistTemp=file.getInt();
    ResistTemp=file.getFloat();
    IgniteTemp=file.getInt();
    BurnTemp=file.getInt();
    value=file.getInt();
    name=file.getString();
    flags=file.getInt();
    mytype=(MATTYPE)file.getInt();
    ID=file.getInt();
}
MaterialTemplate::MaterialTemplate(std::string name):prefix(name)
{
}
MaterialTemplate::~MaterialTemplate()
{
}
void MaterialTemplate::MeltTemp(range temp)
{
    mtemp=temp;
}
void MaterialTemplate::BoilTemp(range temp)
{
    boiltemp=temp;
}
void MaterialTemplate::ResistTemp(range temp)
{
    rtemp=temp;
}
void MaterialTemplate::IgniteTemp(range temp)
{
    itemp=temp;
}
void MaterialTemplate::BurnTemp(range temp)
{
    burntemp=temp;
}
void MaterialTemplate::Flags(int setflags)
{
    flags=setflags;
}
void MaterialTemplate::HSV(range h,range s,range v)
{
    cv=v;
    cs=s;
    ch=h;
}
Material *MaterialTemplate::GenMaterial(std::string name,TCODRandom *gen)
{
    Material *ret;
    ret=new Material;
    ret->name=prefix+name;
    //ret->color=TCODColor(gen->getInt(0,255),gen->getInt(0,255),gen->getInt(0,255));// geresni spalvos parinkima sudaryt
    TCODColor col;
    col.setHSV(gen->getFloat(ch.from,ch.to),gen->getFloat((float)cs.from/255.0f,(float)cs.to/255.0f),gen->getFloat((float)cv.from/255.0f,(float)cv.to/255.0f));
    ret->color=col;
    ret->BoilTemp=gen->getInt(boiltemp.from,boiltemp.to);
    ret->ResistTemp=gen->getInt(rtemp.from,rtemp.to);
    ret->BurnTemp=gen->getInt(burntemp.from,burntemp.to);
    ret->IgniteTemp=gen->getInt(itemp.from,itemp.to);
    ret->MeltTemp=gen->getInt(mtemp.from,mtemp.to);
    ret->flags=flags;

    ret->value+=ret->ResistTemp/1000;
    ret->value+=ret->MeltTemp/100;
    ret->value/=4;
    ret->ResistTemp=OptRez(ret->ResistTemp);
    if (flags&MAT_RARE)
    {
        ret->value*=2;
    }
    return ret;
}
MaterialManager::MaterialManager()
{

}
MaterialManager &MaterialManager::Get()
{
    static MaterialManager me;
    return me;
}

void MaterialManager::InitStd()
{

    Material *b;
    b=new Material;
    b->BoilTemp=80000;
    b->MeltTemp=10000;
    b->ResistTemp=OptRez(98000);
    b->color=TCODColor::darkGrey;
    b->name="Stone";
    b->flags=0;
    b->mytype=MATT_STONE;
    AddMaterial(b);

    b=new Material;
    b->BoilTemp=3200;
    b->MeltTemp=600;
    b->ResistTemp=OptRez(4000);
    b->color=TCODColor::gray;
    b->name="Metal";
    b->flags=0;
    b->mytype=MATT_METAL;
    AddMaterial(b);

    b=new Material;
    b->BoilTemp=80000;
    b->MeltTemp=75000;
    b->ResistTemp=OptRez(0);
    b->color=TCODColor::darkPurple;
    b->name="Conductor";
    b->flags=0;
    b->mytype=MATT_METAL;
    AddMaterial(b);

    b=new Material;
    b->BoilTemp=200;
    b->MeltTemp=85;
    b->ResistTemp=OptRez(1000);
    b->IgniteTemp=210;
    b->BurnTemp=24;
    b->color=TCODColor::darkYellow;
    b->flags=MAT_IGNITES;
    b->name="Fat";
    b->mytype=MATT_OTHER;
    AddMaterial(b);

    b=new Material;
    b->BoilTemp=125;
    b->MeltTemp=-35;
    b->ResistTemp=OptRez(2000);
    b->BurnTemp=30;
    b->IgniteTemp=200;
    b->color=TCODColor::silver;
    b->flags=MAT_IGNITES|MAT_FLAMABLE;
    b->name="Alcohol";
    b->mytype=MATT_OTHER;
    AddMaterial(b);

    b=new Material;
    b->BoilTemp=1000;
    b->MeltTemp=-100;
    b->ResistTemp=OptRez(7000);
    b->BurnTemp=5;
    b->IgniteTemp=500;
    b->color=TCODColor::darkBrown;
    b->flags=MAT_IGNITES|MAT_FLAMABLE|MAT_LONGBURN;
    b->name="Oil";
    b->mytype=MATT_OTHER;
    AddMaterial(b);

    b=new Material;
    b->BoilTemp=100000;//niekad nevirt...
    b->MeltTemp=-200;
    b->ResistTemp=OptRez(2000);
    b->BurnTemp=-20;
    b->IgniteTemp=-50;
    b->color=TCODColor::lightBlue;
    b->flags=MAT_IGNITES;
    b->name="Liquid Nitro";
    b->mytype=MATT_OTHER;
    AddMaterial(b);

    b=new Material;
    b->BoilTemp=80000;
    b->MeltTemp=5000;
    b->ResistTemp=OptRez(5000);
    b->BurnTemp=0;
    b->IgniteTemp=0;
    b->color=TCODColor::darkBrown;
    b->flags=0;
    b->name="Insulator";
    b->mytype=MATT_STONE;
    AddMaterial(b);

    b=new Material;
    b->BoilTemp=100;
    b->MeltTemp=0;
    b->ResistTemp=OptRez(800);
    b->color=TCODColor::darkBlue;
    b->name="Water";
    b->flags=0;
    b->mytype=MATT_WATER;
    AddMaterial(b);

    b=new Material;
    b->BoilTemp=700;
    b->MeltTemp=200;
    b->ResistTemp=OptRez(5000);
    b->BurnTemp=0;
    b->IgniteTemp=250;
    b->color=TCODColor::white;
    b->flags=MAT_EXPLODE;
    b->name="C4";
    b->mytype=MATT_OTHER;
    AddMaterial(b);

    b=new Material;
    b->BoilTemp=8000;
    b->MeltTemp=1500;
    b->ResistTemp=OptRez(7000);
    b->BurnTemp=0;
    b->IgniteTemp=1500;
    b->color=TCODColor::darkBrown;
    b->flags=MAT_LIVELY;
    b->name="Soil";
    b->mytype=MATT_OTHER;
    AddMaterial(b);

     b=new Material;
    b->BoilTemp=130;
    b->MeltTemp=-5;
    b->ResistTemp=OptRez(2000);
    b->BurnTemp=0;
    b->IgniteTemp=1500;
    b->color=TCODColor::red;
    b->flags=MAT_ORGANIC;
    b->name="Blood";
    b->mytype=MATT_BLOOD;
    AddMaterial(b);

     b=new Material;
    b->BoilTemp=500;
    b->MeltTemp=100;
    b->ResistTemp=OptRez(3000);
    b->BurnTemp=5;
    b->IgniteTemp=99;
    b->color=TCODColor::red;
    b->flags=MAT_ORGANIC;
    b->name="Flesh";
    b->mytype=MATT_FLESH;
    AddMaterial(b);

    MaterialTemplate stones("stone:");
    stones.BoilTemp(range(10000,80000));
    stones.MeltTemp(range(1000,10000));
    stones.ResistTemp(range(5500,8000));
    stones.BurnTemp(range(90000,90000));
    stones.HSV(range(0,360),range(80,150),range(80,200));
    stones.Flags(0);
    TCODRandom *gen=new TCODRandom();
    for (int i=0;i<6;i++)
    {
        std::string name;
        name="a";
        name[0]+=i;
        b=stones.GenMaterial(name,gen);
        b->mytype=MATT_STONE;
        AddMaterial(b);
    }
    delete gen;
}
void MaterialManager::AddMaterial(Material *toadd)
{
    toadd->ID=GetID();
    Materials[MATT_ALL].push_back(toadd);
    Materials[toadd->mytype].push_back(toadd);
}
void MaterialManager::Save(TCODZip &file)
{
    file.putInt(GetCount());
    for (unsigned i=0;i<GetCount();i++)
        Materials[MATT_ALL][i]->Save(file);
}
void MaterialManager::Load(TCODZip &file)
{
    int cc=file.getInt();
    for (unsigned i=0;i<cc;i++)
    {
        Material *b;
        b=new Material;
        b->Load(file);
        AddMaterial(b);
    }
}
Material *MaterialManager::GetMaterial(MATTYPE t,int idx)
{
    /*if(Materials[MATT_ALL][idx]->ID!=idx)
        throw "ERROR";*/
    return Materials[t][idx];
}
Material *MaterialManager::GetRandom(TCODRandom *ran,MATTYPE t)
{

    /*if (t==MATT_ALL)
        t=(MATTYPE)ran->getInt((int)MATT_ALL+1,(int)MATT_OTHER);*/
    int size=GetCount(t);
    return GetMaterial(t,ran->getInt(0,size-1));
}
unsigned MaterialManager::GetCount(MATTYPE t)
{
    return Materials[t].size();
}

