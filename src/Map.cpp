#include "Map.h"
#define MAXLEVEL 4
#include <iostream>
int XYtoINDX(int w,int x,int y)
{
    return x+y*w;
}
bool CheckConnected(ITile *a,ITile *b,SIDES s)
{
    if((a->GetSide(s)==b)&&(b->GetSide((SIDES)(E_DOWN-s))==a))
        return true;
    return false;
}
bool MakeIndexed(ITile *upleft,int w,int h,ITiles &ret)
{
    if (!upleft)return false;
    ITile *cur=upleft;
    ITile *curf=upleft;
    for (unsigned j=0;j<h;j++)
    {
        if(!curf)return false;
        cur=curf;
        for (unsigned i=0;i<w;i++)
        {
            if(!cur)return false;
            ret.push_back(cur);
            cur=cur->GetSide(E_E);

        }
        curf=curf->GetSide(E_S);

    }
    return true;
}
bool IsNormal(ITile *upleft,int w,int h)
{
    ITiles list;
    return IsNormal(upleft,w,h,list);

}
bool IsNormal(ITile *upleft,int w,int h,ITiles &ret)
{
    ret.clear();
    if(!MakeIndexed(upleft,w,h,ret))
        return false;
    for(int i=0;i<w;i++)
    {
        if(!CheckConnected(ret[XYtoINDX(w,i,0)],ret[XYtoINDX(w,i,1)],E_S))
            return false;
        if(h>1)
        if(!CheckConnected(ret[XYtoINDX(w,i,h-2)],ret[XYtoINDX(w,i,h-1)],E_S))
            return false;
        if(i!=0)
            {
                 if(!CheckConnected(ret[XYtoINDX(w,i,0)],ret[XYtoINDX(w,i-1,1)],E_SW))
                    return false;
                if(!CheckConnected(ret[XYtoINDX(w,i,h-2)],ret[XYtoINDX(w,i-1,h-1)],E_SW))
                    return false;
            }
        if(i!=w-1)
            {
                if(!CheckConnected(ret[XYtoINDX(w,i,0)],ret[XYtoINDX(w,i+1,1)],E_SE))
                    return false;
                if(!CheckConnected(ret[XYtoINDX(w,i,h-2)],ret[XYtoINDX(w,i+1,h-1)],E_SE))
                    return false;
            }
    }
    for(int j=0;j<h;j++)
    {
        if(!CheckConnected(ret[XYtoINDX(w,0,j)],ret[XYtoINDX(w,1,j)],E_E))
            return false;
        if(w>1)
        if(!CheckConnected(ret[XYtoINDX(w,w-2,j)],ret[XYtoINDX(w,w-1,j)],E_E))
            return false;
        if(j!=0)
            {
                 if(!CheckConnected(ret[XYtoINDX(w,0,j)],ret[XYtoINDX(w,1,j-1)],E_NE))
                    return false;
                if(!CheckConnected(ret[XYtoINDX(w,w-2,j)],ret[XYtoINDX(w,w-1,j-1)],E_NE))
                    return false;
            }
        if(j!=h-1)
            {
                if(!CheckConnected(ret[XYtoINDX(w,0,j)],ret[XYtoINDX(w,1,j+1)],E_SE))
                    return false;
                if(!CheckConnected(ret[XYtoINDX(w,w-2,j)],ret[XYtoINDX(w,w-1,j+1)],E_SE))
                    return false;
            }
    }

    for(int i=1;i<w-1;i++)
        for(int j=1;j<h-1;j++)
        {
            if(!CheckConnected(ret[XYtoINDX(w,i,j)],ret[XYtoINDX(w,i+1,j)],E_E))
                    return false;
            if(!CheckConnected(ret[XYtoINDX(w,i,j)],ret[XYtoINDX(w,i-1,j)],E_W))
                    return false;
            if(!CheckConnected(ret[XYtoINDX(w,i,j)],ret[XYtoINDX(w,i,j+1)],E_S))
                    return false;
            if(!CheckConnected(ret[XYtoINDX(w,i,j)],ret[XYtoINDX(w,i,j-1)],E_N))
                    return false;
            if(!CheckConnected(ret[XYtoINDX(w,i,j)],ret[XYtoINDX(w,i+1,j+1)],E_SE))
                    return false;
            if(!CheckConnected(ret[XYtoINDX(w,i,j)],ret[XYtoINDX(w,i+1,j-1)],E_NE))
                    return false;
            if(!CheckConnected(ret[XYtoINDX(w,i,j)],ret[XYtoINDX(w,i-1,j+1)],E_SW))
                    return false;
            if(!CheckConnected(ret[XYtoINDX(w,i,j)],ret[XYtoINDX(w,i-1,j-1)],E_NW))
                    return false;
        }

    return true;
}
void Map::SaveMap(std::string path,int starttile)
{
    Map::SaveMapZiped(path,starttile);
    //Map::SaveMapText(path,starttile);
}
void Map::SaveMapZiped(std::string path,int starttile)
{
    TCODZip file;
    for (unsigned i=0;i<map.size();i++)
    {
        map[i]->savedata=new TileSave();
        map[i]->savedata->num=i;
        for (int j=0;j<MAXMAT;j++)
            if (map[i]->GetMaterial(j))
                map[i]->savedata->mats[j]=map[i]->GetMaterial(j)->ID;
            else
                map[i]->savedata->mats[j]=-1;
    }
    file.putInt(0);
    file.putInt(starttile);
    //
    //file.putInt(stdMaterials.size());
    //for (int i=0;i<stdMaterials.size();i++)
    //{
    /*file.putInt(stdMaterials[i]->color.r);
    file.putInt(stdMaterials[i]->color.g);
    file.putInt(stdMaterials[i]->color.b);
    file.putInt(stdMaterials[i]->MeltTemp);
    file.putInt(stdMaterials[i]->BoilTemp);
    file.putInt(stdMaterials[i]->ResistTemp);*/
    //   stdMaterials[i]->Save(file);
    //}
    MaterialManager::Get().Save(file);
    //
    file.putInt(map.size());
    for (unsigned i=0;i<map.size();i++)
    {
        //out<<map[i]->savedata->num<<'-';
        file.putInt(map[i]->savedata->num);
        for (unsigned j=E_UP;j<E_LAST;j++)
        {
            if (map[i]->exits[j])
                //out<<map[i]->exits[j]->savedata->num;
                file.putInt(map[i]->exits[j]->savedata->num);
            else
                file.putInt(INT_MAX);
            //out<<'.';
            //out<<' ';
        }

        //out<<map[i]->pic<<' '<<map[i]->r<<' '<<map[i]->g<<' '<<map[i]->b<<' '<<(map[i]->walk)?1:0;
        //file.putInt(map[i]->pic);
        for (int j=0;j<MAXMAT;j++)
            file.putInt(map[i]->savedata->mats[j]);
        file.putInt(map[i]->temp);
        //file.putInt(map[i]->r);
        //file.putInt(map[i]->g);
        //file.putInt(map[i]->b);
        //file.putInt((map[i]->walk)?1:0);
        // file.putInt((int)map[i]->doors);
        //out<<' '<<(unsigned)map[i]->doors;
        //if(i!=map.size()-1)
        //out<<std::endl;
    }
    for (unsigned i=0;i<map.size();i++)
    {
        map[i]->SaveEntitys(file);
        map[i]->SaveStack(file);
    }
    for (int i=0;i<map.size();i++)
    {
        delete map[i]->savedata;
    }
    file.saveToFile(path.c_str());
}
void Map::SaveMapText(std::string path,int starttile)
{
    // DO NOT USE
    throw "DO NOT USE";

}

void Map::LoadMap(std::string path,int &starttile)
{
    //LoadMapText(path,starttile);
    LoadMapZiped(path,starttile);
}
void Map::LoadProcedural(TCODZip &file)
{

    int w,h;
    w=file.getInt();
    h=file.getInt();
    for (int i=0;i<w*h;i++)
    {
        Tile *ttile;
        if ((i%w==0)||(i<w)||(i%w==w-1)||(i>(w*(h-1))))
            ttile=new Tile(*stdtiles[1]);
        else
            ttile=new Tile(*stdtiles[0]);
        map.push_back(ttile);
    }
    ConnectMap(map,w,h);
    ////////
    int numMin=file.getInt();
    int numMax=file.getInt();
    if (numMax>0)
    {
        int subs;
        if (numMax-numMin==0)
            subs=numMin;
        else
            subs=rand()%(numMax-numMin)+numMin;
        if (subs!=0)
            for (int j=0;j<subs;j++)
            {

                int ww=rand()%(w-3)+4;
                int hh=rand()%(h-3)+4;
                tiles submap;
                for (int i=0;i<ww*hh;i++)
                {

                    Tile *ttile;
                    if ((i%ww==0)||(i<ww)||(i%ww==ww-1)||(i>(ww*(hh-1))))
                        ttile=new Tile(*stdtiles[1]);
                    else
                        ttile=new Tile(*stdtiles[0]);
                    submap.push_back(ttile);
                }
                ConnectMap(submap,ww,hh);
                bool first;
                first=true;
                int side=rand()%(E_LAST-E_UP-2)+E_UP+1;
                int portal=rand()%submap.size();
                while ((!submap[portal])||(!submap[portal]->exits[side])||first)
                {
                    portal=rand()%submap.size();
                    first=false;
                }
                //submap[portal]->CopyVisual(*stdtiles[3]);
                int portal2=rand()%map.size();
                //SetTile(map[portal2],3);

                /*
                map[portal2]->exits[side]=submap[portal];
                submap[portal]->exits[E_LAST-side]=map[portal2];
                */
                //ConnectClose(map[portal2],submap[portal],(SIDES)side);
                ConnectClose(map[portal2],submap[portal]->exits[side],(SIDES)side);


                for (int i=0;i<submap.size();i++)
                    map.push_back(submap[i]);
                submap.clear();
            }
    }


}
void Map::LoadMapZiped(std::string path,int &starttile)
{
    TCODZip file;

    file.loadFromFile(path.c_str());
    //std::vector <LoadData> ll;
    VecLoad ll;
    int type=file.getInt();
    if (type==1)
    {
        InitStd();
        LoadProcedural(file);
        return;
    }
    starttile=file.getInt();
    //
    MaterialManager::Get().Load(file);
    //int matcount=file.getInt();
    //for (int i=0;i<matcount;i++)
    //{
    //   Material *a;
    //   a=new Material;
    /*a->color.r=file.getInt();
    a->color.g=file.getInt();
    a->color.b=file.getInt();
    a->MeltTemp=file.getInt();
    a->BoilTemp=file.getInt();
    a->ResistTemp=file.getInt();*/
    //    a->Load(file);
    //    stdMaterials.push_back(a);
    //}
    //
    int size=file.getInt();
    for (int i=0;i<size;i++)
    {
        LoadData pp;
        pp.num=file.getInt();
        for (int j=E_UP;j<E_LAST;j++)
        {
            pp.ExitsUsed[j]=false;
            int r=file.getInt();
            if (r!=INT_MAX)
            {
                pp.ExitsUsed[j]=true;
                pp.Exits[j]=r;
            }
        }
        pp.me=new Tile();
        for (int j=0;j<MAXMAT;j++)
        {
            int gg=file.getInt();
            if (gg!=-1)
                //SetMat(pp.me,gg,j);
                pp.me->SetMaterial(MaterialManager::Get().GetMaterial(MATT_ALL,gg),j);
            else
                pp.me->SetMaterial(NULL,j);
        }
        pp.me->SetTemp(file.getInt());
        //pp.me->pic=file.getInt();
//        pp.me->r=file.getInt();
        //   pp.me->g=file.getInt();
        //   pp.me->b=file.getInt();
        //    pp.me->walk=(file.getInt()==1);
        //pp.me->doors=(DoorState)file.getInt();
        ll.push_back(pp);
    }

    for (unsigned i=0;i<ll.size();i++)
    {
        for (int j=E_UP;j<E_LAST;j++)
            if (ll[i].ExitsUsed[j])
                ll[i].me->SetSide(ll[ll[i].Exits[j]].me,(SIDES)j);
        map.push_back(dynamic_cast<Tile*>(ll[i].me));
    }
    for (unsigned i=0;i<map.size();i++)
    {
        int c=file.getInt();
        for (unsigned j=0;j<c;j++)
        {
            char ent=file.getChar();
            //file.putChar(ent);//?
            switch (ent)
            {
            case CL_ISP_BOLT:
                map[i]->AddEntity(new ISP_bolt(file,ll));
                break;
            case CL_ISP_FIRE:
                map[i]->AddEntity(new ISP_fire(file,ll));
                break;
            case CL_ISP_GLOW:
                map[i]->AddEntity(new ISP_glow(file,ll));
                break;
            case CL_BUILDING:
                map[i]->AddEntity(new clBuilding(file,ll));
                break;
            default:
                throw "Error loading entitys";
            }
        }
        map[i]->LoadStack(file);
    }
}
void Map::LoadMapText(std::string path,int &starttile)
{
    throw "DO NOT USE";
}
void Map::ConnectMap(tiles &mmap,int w,int h)
{
    for (int i=1;i<w-1;i++)
        for (int j=1;j<h-1;j++)
        {
            Tile *cur=mmap[i+j*w];
            cur->exits[E_UP]=0;
            cur->exits[E_DOWN]=0;
            cur->exits[E_N]=mmap[i+(j-1)*w];
            cur->exits[E_S]=mmap[i+(j+1)*w];
            cur->exits[E_NE]=mmap[i+1+(j-1)*w];
            cur->exits[E_SE]=mmap[i+1+(j+1)*w];
            cur->exits[E_NW]=mmap[i-1+(j-1)*w];
            cur->exits[E_SW]=mmap[i-1+(j+1)*w];
            cur->exits[E_E]=mmap[i+1+j*w];
            cur->exits[E_W]=mmap[i-1+j*w];
        }

    for (int i=0;i<w;i++)
    {
        Tile *cur=mmap[i];
        cur->exits[E_S]=mmap[i+w];
        if (i!=0)
        {
            cur->exits[E_SW]=mmap[i-1+w];
            cur->exits[E_W]=mmap[i-1];
        }
        if (i!=w-1)
        {
            cur->exits[E_SE]=mmap[i+1+w];
            cur->exits[E_E]=mmap[i+1];
        }

        cur=mmap[i+(h-1)*w];
        cur->exits[E_N]=mmap[i+(h-2)*w];
        if (i!=0)
        {
            cur->exits[E_NW]=mmap[i+(h-2)*w-1];
            cur->exits[E_W]=mmap[i+(h-1)*w-1];
        }
        if (i!=w-1)
        {
            cur->exits[E_NE]=mmap[i+(h-2)*w+1];
            cur->exits[E_E]=mmap[i+(h-1)*w+1];
        }

    }

    for (int j=0;j<h;j++)
    {
        Tile *cur=mmap[j*w];
        cur->exits[E_E]=mmap[j*w+1];
        if (j!=0)
        {
            cur->exits[E_N]=mmap[(j-1)*w];
            cur->exits[E_NE]=mmap[(j-1)*w+1];
        }
        if (j!=h-1)
        {
            cur->exits[E_S]=mmap[(j+1)*w];
            cur->exits[E_SE]=mmap[(j+1)*w+1];
        }

        cur=mmap[w-1+j*w];
        cur->exits[E_W]=mmap[w+j*w-2];
        if (j!=0)
        {
            cur->exits[E_N]=mmap[w+(j-1)*w-1];
            cur->exits[E_NW]=mmap[w+(j-1)*w-2];
        }

        if (j!=h-1)
        {
            cur->exits[E_S]=mmap[w+(j+1)*w-1];
            cur->exits[E_SW]=mmap[w+(j+1)*w-2];
        }

    }

}
void Map::InitStdTiles()
{
    MaterialManager &mm=MaterialManager::Get();
    Tile *a;
    a=new Tile();
    a->SetHeight(1,mm.GetMaterial(MATT_ALL,1));
    stdtiles.push_back(a);

    a=new Tile();
    a->SetHeight(MAXMAT,mm.GetMaterial(MATT_ALL,1));
    stdtiles.push_back(a);

    a=new Tile();
    a->SetHeight(MAXMAT,mm.GetMaterial(MATT_ALL,2));
    stdtiles.push_back(a);

    a=new Tile();
    a->SetHeight(0,mm.GetMaterial(MATT_ALL,1));
    stdtiles.push_back(a);

    a=new Tile();
    a->SetHeight(MAXMAT,mm.GetMaterial(MATT_ALL,0));
    stdtiles.push_back(a);

    a=new Tile();
    a->SetHeight(1,mm.GetMaterial(MATT_ALL,3));
    stdtiles.push_back(a);

}
void Map::InitStdMat()
{
    MaterialManager &mm=MaterialManager::Get();
    mm.InitStd();
    water=mm.GetMaterial(MATT_WATER,0);

    //stdMaterials.push_back(b);


}

void Map::InitStd()
{

    InitStdMat();
    InitStdTiles();

}
Map::Map(std::string file,int &starttile)
{
    //InitStd();
    random=new TCODRandom;
    LoadMap(file,starttile);
    InitStdTiles();
}
Map::Map(int w,int h,int z)
{
    random=new TCODRandom;
    InitStd();
    std::vector <tiles> layers;
    tiles mp;//,lmp;
    for ( int j=0;j<z;j++)
    {

        for (int i=0;i<w*h;i++)
            if (j==0)
                mp.push_back(new Tile(*stdtiles[0]));
            else
                mp.push_back(new Tile(*stdtiles[3]));
        ConnectMap(mp,w,h);
        layers.push_back(mp);
        if (layers.size()>1)
            for (int i=0;i<w*h;i++)
                ConnectImmediate(layers[j-1][i],mp[i],E_UP);
        RegTiles(mp);

        mp.clear();


    }
    Material *stone=MaterialManager::Get().GetRandom(random,MATT_STONE);
    float f[2];
    static float hurst=TCOD_NOISE_DEFAULT_HURST;
    static float lacunarity=TCOD_NOISE_DEFAULT_LACUNARITY;
    TCODNoise *noise = new TCODNoise(2,hurst,lacunarity);
    float scale=0.025f;
    for (int i=0;i<w;i++)
        for (int j=0;j<h;j++)
        {
            int cz;
            f[0]=i*scale;
            f[1]=j*scale;

            cz=(noise->getPerlin(f)+1)*(float)z*3.5f;
            //if(cz<z)
            for (int a=0;a<cz/7;a++)
                layers[a][i+j*w]->SetHeight(7,stone);//CopyVisual(*stdtiles[2]);
            Tile *cur=layers[cz/7][i+j*w];
            cur->SetHeight(cz%7,stone);
            /*for (int a=0;a<cz%7;a++)
            {
                //int h=cur->GetHeight()
                cur->SetMaterial([a]=stdMaterials[1];
            }*/
        }
    delete noise;
    noise = new TCODNoise(1,hurst,lacunarity);
    TCODRandom *ran=new TCODRandom;
    int cz=7;
    Material *lavamat=MaterialManager::Get().GetMaterial(MATT_STONE);
    int numstones=MaterialManager::Get().GetCount(MATT_STONE);
    for (int i=0;i<numstones;i++)
    {
        Material *tlavastone=MaterialManager::Get().GetMaterial(MATT_STONE,i);
        //std::cout<<i<<" "<<stdStones[i]->name<<" "<<stdStones[i]->MeltTemp<<"\n";
        if (tlavastone->MeltTemp<CONST_LAVA_TEMP)
        {

            lavamat=tlavastone;
            break;
        }
    }
    int maxrad=15;
    int minrad=5;
    int l=0;
    int cx;
    int cy;
    cx=ran->getInt(0,w);
    cy=ran->getInt(0,h);
    const float scale2=0.5;
    while ((cz>0)&&(l<z))
    {
        f[0]=(float)l*scale2;
        int r;
        r=(noise->getPerlin(f)+1)*(maxrad-minrad)+minrad;
        if (l==0)
        {
            class lava:public TileFunct
            {
                Map *mymap;
            public:
                lava(Map *t)
                {
                    mymap=t;
                }
                bool Run(ITile *t)
                {
                    mymap->AddPlace(t,E_PLACE_LAVA);
                    return true;
                }

            }mylava(this);
            RunCircle(r,layers[l][cx+cy*w],mylava);
        }

        class lava:public TileFunct
        {
            Material *mymat;
        public:
            lava(Material *t)
            {
                mymat=t;
            }
            bool Run(ITile *t)
            {
                int h=t->GetHeight();
                t->SetHeight(h+1,mymat);
                t->SetTemp(8000);
                return true;
            }

        }mylava(lavamat);
        RunCircle(r,layers[l][cx+cy*w],mylava);

        l++;
        cz=layers[l][cx+cy*w]->GetHeight();
    }
    delete ran;
    //map.resize(w*h,stdtiles[0]);
    //for (int i=0;i<w;i++)
    //    SetTile(map[i+m_h/2*m_w],1);
    /*for(int i=1;i<w-1;i++)
        for(int j=1;j<h-1;j++)
        {
            Tile *cur=map[i+j*w];
            cur->exits[E_UP]=0;
            cur->exits[E_DOWN]=0;
            cur->exits[E_N]=map[i+(j-1)*w];
            cur->exits[E_S]=map[i+(j+1)*w];
            cur->exits[E_NE]=map[i+1+(j-1)*w];
            cur->exits[E_SE]=map[i+1+(j+1)*w];
            cur->exits[E_NW]=map[i-1+(j-1)*w];
            cur->exits[E_SW]=map[i-1+(j+1)*w];
            cur->exits[E_E]=map[i+1+j*w];
            cur->exits[E_W]=map[i-1+j*w];
        }*/

}
Map::Map(int w,int h):m_w(w),m_h(h)
{
    random=new TCODRandom;
    InitStd();

    for (int i=0;i<w*h;i++)
        map.push_back(new Tile(*stdtiles[0]));


    //for(int i=0;i<w*h;i++)
    //   map[i]->SetHeight(random->getInt(0,7),MaterialManager::Get().GetMaterial(MATT_STONE,0));
    //map.resize(w*h,stdtiles[0]);
    //for (int i=0;i<w;i++)
    //    SetTile(map[i+m_h/2*m_w],1);
    /*for(int i=1;i<w-1;i++)
        for(int j=1;j<h-1;j++)
        {
            Tile *cur=map[i+j*w];
            cur->exits[E_UP]=0;
            cur->exits[E_DOWN]=0;
            cur->exits[E_N]=map[i+(j-1)*w];
            cur->exits[E_S]=map[i+(j+1)*w];
            cur->exits[E_NE]=map[i+1+(j-1)*w];
            cur->exits[E_SE]=map[i+1+(j+1)*w];
            cur->exits[E_NW]=map[i-1+(j-1)*w];
            cur->exits[E_SW]=map[i-1+(j+1)*w];
            cur->exits[E_E]=map[i+1+j*w];
            cur->exits[E_W]=map[i-1+j*w];
        }*/
    ConnectMap(map,w,h);
    class Boom:public TileFunct
    {
    public:
        bool Run(ITile *t)
        {
            t->SetHeight(7,MaterialManager::Get().GetMaterial(MATT_STONE,0));
        }
    }boom1;
    for (int i=0;i<10;i++)
        RunCircle(random->getInt(3,10),map[random->getInt(0,map.size()-1)],boom1);
    //ctor
}
Tile *Map::GetSomeTile()
{
    return map[rand()%map.size()];
}

void Map::RegTiles(tiles &list)
{
    for (unsigned i=0;i<list.size();i++)
        map.push_back(list[i]);
}
void Map::SetTile(Tile *t,int stdtile)
{
    /*t->pic=stdtiles[stdtile]->pic;
    t->r=stdtiles[stdtile]->r;
    t->g=stdtiles[stdtile]->g;
    t->b=stdtiles[stdtile]->b;
    t->walk=stdtiles[stdtile]->walk;*/
    t->CopyVisual(*stdtiles[stdtile]);
}

void Map::RemoveTile(Tile *t)
{

    tiles::iterator a=find(map.begin(),map.end(),t);
    map.erase(a);
    delete t;
}

inline void PrintTile(TCODConsole *t,int tx,int ty,Tile *cur)//OPTIMATIZUOTI!!!
{
    int leveled=0;
    int h=cur->GetHeight();
    while ((h<0)&&(leveled<MAXLEVEL))
    {
        if (cur->GetMob()!=NULL)
        {
            Mob *tt=cur->GetMob();
            t->setForegroundColor(TCODColor::lerp(tt->col,TCODColor::black,(float)leveled/MAXLEVEL));
            t->putChar(tx,ty,tt->pic,TCOD_BKGND_SET);
            return;
        }
        if (cur->GetEntityCount()!=0)
        {
            //ISpecial *tt=cur->GetSpecial();
            IEntity *tt=cur->GetEntity();
            t->setForegroundColor(TCODColor::lerp(tt->GetColor(),TCODColor::black,(float)leveled/MAXLEVEL));
            t->putChar(tx,ty,tt->GetPic(),TCOD_BKGND_SET);
            if (tt->GetPriority()<0)
            {
                if (cur->GetDeepness()==0)
                    return;
            }
            else return;
        }
        if (cur->GetStack()!=NULL)
        {
            IItem *tt=cur->GetStack()->GetItem(0);//GetBiggestItem();
            t->setForegroundColor(TCODColor::lerp(tt->GetColor(),TCODColor::black,(float)leveled/MAXLEVEL));
            t->putChar(tx,ty,tt->GetPic(),TCOD_BKGND_SET);
            return;
        }
        if (cur->exits[E_DOWN])
        {

            cur=cur->exits[E_DOWN];
            h=cur->GetHeight();
            leveled++;
        }
        else
        {
            t->setForegroundColor(cur->GetColor());
            t->putChar(tx,ty,cur->GetPic(),TCOD_BKGND_SET);
            return;
        }
    }
    if (cur->GetMob()!=NULL)
    {
        Mob *tt=cur->GetMob();
        t->setForegroundColor(TCODColor::lerp(tt->col,TCODColor::black,(float)leveled/MAXLEVEL));
        t->putChar(tx,ty,tt->pic,TCOD_BKGND_SET);
        return;
    }
    if (cur->GetEntityCount()!=0)
    {
        //ISpecial *tt=cur->GetSpecial();
        IEntity *tt=cur->GetEntity();
        t->setForegroundColor(TCODColor::lerp(tt->GetColor(),TCODColor::black,(float)leveled/MAXLEVEL));
        t->putChar(tx,ty,tt->GetPic(),TCOD_BKGND_SET);
        if (tt->GetPriority()<0)
        {
            if (cur->GetDeepness()==0)
                return;
        }
        else return;
    }
    if (cur->GetStack()!=NULL)
    {
        IItem *tt=cur->GetStack()->GetItem(0);//GetBiggestItem();
        t->setForegroundColor(TCODColor::lerp(tt->GetColor(),TCODColor::black,(float)leveled/MAXLEVEL));
        t->putChar(tx,ty,tt->GetPic(),TCOD_BKGND_SET);
        return;
    }
    if (h>=0)
    {
        TCODColor base=TCODColor::lerp(cur->GetColor(),TCODColor::black,(float)leveled/MAXLEVEL);
        if (cur->temp<400)
        {
            //if(leveled)
            t->setForegroundColor(base);
            //else
            //    t->setForegroundColor(cur->GetTop()->color);
        }
        else
        {

            float coff=(float)cur->temp/8000;

            if (coff>1.0f)
            {
                coff-=1.0f;
                if (coff>1.0f)coff=1.0f;
                t->setForegroundColor(TCODColor::lerp(TCODColor::darkRed,TCODColor::white,coff));
            }
            else
                t->setForegroundColor(TCODColor::lerp(base,TCODColor::darkRed,coff));
        }
    }

    t->putChar(tx,ty,cur->GetPic(),TCOD_BKGND_SET);

    if((cur->GetSide(E_NW))&&(!IsNormal(cur->GetSide(E_NW),3,3)))
        t->setBack(tx,ty,TCODColor::red);
    else
        t->setBack(tx,ty,TCODColor::black);

}
tiles Map::CollectEnclosed(tiles &closure)
{
    tiles ret;
    Tile *st=closure[0];
    Tile *cur=st;
    Tile *last=NULL;
    SIDES s=E_W;
    SIDES n;

    int steps=0;
    while ((cur!=st)||(steps==0))
    {
        steps++;

        while (!inset(cur->GetSide(s),closure))
        {
            s=closest(s,false);// gal du kart pasukt!
            if (cur->GetSide(s)==last)
                throw "Klaida!";
        }
        cur=cur->GetSide(s);
        n=closest(closest(s,false),false);
        Tile *norm=cur->GetSide(n);
        if (norm)
        {
            //if(norm->GetSpecial()==NULL)
            //norm->SetSpecial(new ISP_glow(norm));

            if ((!inset(norm,ret))&&(!inset(norm,closure)))
            {
                ret.push_back(norm);
            }
        }

        last=cur;
        if (steps>closure.size())
            throw "Neuzhdaras closure?";
    }

    tiles list=ret;
    while (list.size()>0)
    {
        cur=list[list.size()-1];
        for (int i=0;i<E_LAST;i++)
        {
            last=cur->GetSide((SIDES)i);
            if (last)
                if ((!inset(last,list))&&(!inset(last,closure))&&(!inset(last,ret)))
                {
                    ret.push_back(last);
                    list.push_back(last);
                }
        }
        list.pop_back();
    }
    return ret;
}
void Map::PrintMapRot(TCODConsole *t,int x,int y,int w,int h,int r,Tile *st,int rot)
{
    if (st==NULL) return;
    int cx=x+w/2;
    int cy=y+h/2;
    Tile *cur=st;
    int r2=r*r;
    PrintTile(t,cx,cy,cur);
    SIDES curside;
    for (int my=-r;my<=r;my++)
    {
        int xs=sqrt(r2-my*my);//Galima greiciau su bresham circle'u
        for (int mx=-xs;mx<=xs;mx++)
        {
            TCODLine::init(0,0,mx,my);
            cur=st;
            int lx,ly,xx,yy;
            xx=0;
            yy=0;
            lx=0;
            ly=0;
            int h,w;
            h=t->getHeight();
            w=t->getWidth();
            while (!TCODLine::step(&xx,&yy))
            {

                if (((xx+cx)>w)||((xx+cx)<0)||
                        ((yy+cy)>h)||((yy+cy)<0))
                    break;
                int dx=xx-lx;
                int dy=yy-ly;


                if (dx>0)
                    if (dy>0)
                        curside=E_SE;
                    else if (dy==0)
                        curside=E_E;
                    else
                        curside=E_NE;
                else if (dx==0)
                    if (dy>0)
                        curside=E_S;

                    else
                        curside=E_N;
                else
                    if (dy>0)
                        curside=E_SW;
                    else if (dy==0)
                        curside=E_W;
                    else
                        curside=E_NW;
                for (int i=0;i<rot;i++)
                    curside=closest(curside,false);
                cur=cur->exits[curside];
                if (cur==NULL) break;

                PrintTile(t,xx+cx,yy+cy,cur);
                if (!cur->PassibleLight())break;
                //if (cur->doors==DOORSCLOSED)break;

                lx=xx;
                ly=yy;
            }
        }
    }
}
void Map::PrintMapXX(TCODConsole *t,int x,int y,int w,int h,int r,Tile *st)
{
    std::vector <bool> visited2;
    visited2.resize(4*(r+1)*(r+1));
    for (int i=0;i<visited2.size();i++)
        visited2[i]=false;
    if (st==NULL) return;
    int cx=x+w/2;
    int cy=y+h/2;
    Tile *cur=st;
    int r2=r*r;
    PrintTile(t,cx,cy,cur);

    for (int my=-r;my<=r;my++)
    {
        int xs=sqrt(r2-my*my);//Galima greiciau su bresham circle'u
        for (int mx=-xs;mx<=xs;mx++)
        {
            TCODLine::init(0,0,mx,my);
            cur=st;
            int lx,ly,xx,yy;
            xx=0;
            yy=0;
            lx=0;
            ly=0;
            int h,w;
            h=t->getHeight();
            w=t->getWidth();
            while (!TCODLine::step(&xx,&yy))
            {

                if (((xx+cx)>w)||((xx+cx)<0)||
                        ((yy+cy)>h)||((yy+cy)<0))
                    break;
                int dx=xx-lx;
                int dy=yy-ly;


                if (dx>0)
                    if (dy>0)
                        cur=cur->exits[E_SE];
                    else if (dy==0)
                        cur=cur->exits[E_E];
                    else
                        cur=cur->exits[E_NE];
                else if (dx==0)
                    if (dy>0)
                        cur=cur->exits[E_S];

                    else
                        cur=cur->exits[E_N];
                else
                    if (dy>0)
                        cur=cur->exits[E_SW];
                    else if (dy==0)
                        cur=cur->exits[E_W];
                    else
                        cur=cur->exits[E_NW];
                if (cur==NULL) break;
                if (!visited2[xx+r+(yy+r)*r*2])
                    PrintTile(t,xx+cx,yy+cy,cur);
                if (!cur->PassibleLight())break;
                //if (cur->doors==DOORSCLOSED)break;
                visited2[xx+r+(yy+r)*r*2]=true;
                lx=xx;
                ly=yy;
            }
        }
    }
}
void Map::PrintMapX(TCODConsole *t,int x,int y,int w,int h,int r,Tile *st)
{
    if (st==NULL) return;
    int cx=x+w/2;
    int cy=y+h/2;
    Tile *cur=st;
    int r2=r*r;
    PrintTile(t,cx,cy,cur);

    for (int my=-r;my<=r;my++)
    {
        int xs=sqrt(r2-my*my);//Galima greiciau su bresham circle'u
        for (int mx=-xs;mx<=xs;mx++)
        {
            TCODLine::init(0,0,mx,my);
            cur=st;
            int lx,ly,xx,yy;
            xx=0;
            yy=0;
            lx=0;
            ly=0;
            int h,w;
            h=t->getHeight();
            w=t->getWidth();
            while (!TCODLine::step(&xx,&yy))
            {

                if (((xx+cx)>w)||((xx+cx)<0)||
                        ((yy+cy)>h)||((yy+cy)<0))
                    break;
                int dx=xx-lx;
                int dy=yy-ly;


                if (dx>0)
                    if (dy>0)
                        cur=cur->exits[E_SE];
                    else if (dy==0)
                        cur=cur->exits[E_E];
                    else
                        cur=cur->exits[E_NE];
                else if (dx==0)
                    if (dy>0)
                        cur=cur->exits[E_S];

                    else
                        cur=cur->exits[E_N];
                else
                    if (dy>0)
                        cur=cur->exits[E_SW];
                    else if (dy==0)
                        cur=cur->exits[E_W];
                    else
                        cur=cur->exits[E_NW];
                if (cur==NULL) break;

                PrintTile(t,xx+cx,yy+cy,cur);
                if (!cur->PassibleLight())break;
                //if (cur->doors==DOORSCLOSED)break;
                lx=xx;
                ly=yy;
            }
        }
    }
}
void Map::PrintMap(TCODConsole *t,int x,int y,int w,int h,int r,Tile *st)
{
    // Tile *tst=st;//&map[m_w/2+m_h/2*m_w];
    float step=0.001;
    int cx=x+w/2;
    int cy=y+h/2;
    if (st==NULL)
        return;
    Tile *cur=st;
    /*
    if (cur->liqh==0)
    {
        t->setForegroundColor(TCODColor(cur->r,cur->g,cur->b));
        t->putChar(cx,cy,cur->pic,TCOD_BKGND_SET);
    }
    else
    {
        t->setForegroundColor(TCODColor(13,103,196));
        t->putChar(cx,cy,'0'+cur->liqh,TCOD_BKGND_SET);
    }*/
    PrintTile(t,cx,cy,cur);
    for (float phi=0;phi<6.28;phi+=step)
    {
        TCODLine::init(0,0,r*cos(phi),r*sin(phi));
        cur=st;
        int lx,ly,xx,yy;
        xx=0;
        yy=0;
        lx=0;
        ly=0;
        int h,w;
        h=t->getHeight();
        w=t->getWidth();

        while (!TCODLine::step(&xx,&yy))
        {
            if (((xx+cx)>w)||((xx+cx)<0)||
                    ((yy+cy)>h)||((yy+cy)<0))
                break;
            int dx=xx-lx;
            int dy=yy-ly;
            //dx/=abs(dx);
            //dy/=abs(dy);

            if (dx>0)
                if (dy>0)
                    cur=cur->exits[E_SE];
                else if (dy==0)
                    cur=cur->exits[E_E];
                else
                    cur=cur->exits[E_NE];
            else if (dx==0)
                if (dy>0)
                    cur=cur->exits[E_S];
            //else if (dy==0)
            //  ;//NOP
                else
                    cur=cur->exits[E_N];
            else
                if (dy>0)
                    cur=cur->exits[E_SW];
                else if (dy==0)
                    cur=cur->exits[E_W];
                else
                    cur=cur->exits[E_NW];
            if (cur==NULL) break;
            /*
            if (cur->liqh==0)
            {
                if (cur->pic==' ')
                {
                    if (cur->exits[E_DOWN])
                    {
                        Tile *tt=cur->exits[E_DOWN];
                        t->setForegroundColor(TCODColor(tt->r,tt->g,tt->b));
                        t->putChar(xx+cx,yy+cy,tt->pic,TCOD_BKGND_SET);
                    }
                }
                else
                {
                    t->setForegroundColor(TCODColor(cur->r,cur->g,cur->b));
                    t->putChar(xx+cx,yy+cy,cur->pic,TCOD_BKGND_SET);
                }
            }
            else
            {
                t->setForegroundColor(TCODColor(13,103,196));
                t->putChar(xx+cx,yy+cy,'0'+cur->liqh,TCOD_BKGND_SET);
            }*/
            PrintTile(t,xx+cx,yy+cy,cur);
            if (!cur->see())break;
            //if (cur->doors==DOORSCLOSED)break;
            lx=xx;
            ly=yy;

            //draw
        }
        //t->setBack(cx,cy,TCODColor::red,TCOD_BKGND_SET);
        //t->putChar(cx,cy,' ',TCOD_BKGND_LIGHTEN);
    }
    /*
    for(int i=0;(i<m_w)&&(i<w);i++)
        for(int j=0;(j<m_h)&&(j<h);j++)
        {
            Tile a=map[i+j*m_w];
            t->setForegroundColor(TCODColor(a.r,a.g,a.b));
            t->putChar(x+i,y+j,a.pic,TCOD_BKGND_SET);
        }*/
}
void Map::RunRandomRay(int r,SIDES to,Tile *st,RayFunct &ff,int prob)
{
    if (!st)
        return;
    ff.Init(st);
    ITile *lcur=st;
    ITile *ppcur=st;
    SIDES pp=to;
    SIDES cs=pp;
    int d=0;
    bool wcw=random->getInt(0,1);
    while ((d<r)&&(ppcur))
    {
        if (random->getInt(0,100)<=prob)
        {
            if ((cs==E_UP)||(cs==E_DOWN))
                cs=(SIDES)random->getInt(E_UP+1,E_DOWN-1);
            else
                cs=closest(cs,wcw);
            wcw=!wcw;//random->getInt(0,1);
        }
        else
        {
            cs=pp;
            wcw=random->getInt(0,1);
        }
        lcur=ppcur;
        ppcur=ppcur->GetSide(cs);
        if (ppcur)
            if (!ff.Run(ppcur))
                break;
        d++;
    }

    ff.Finish(lcur);
}
void Map::RunCircle(int r,Tile *st,TileFunct &ff)
{
    //std::vector <Tile *> visited;

    std::vector <bool> visited2; //optimatizuot!!
    visited2.resize(4*(r+1)*(r+1));
    //std::cout<<visited2.size()<<std::endl;
    for (int i=0;i<visited2.size();i++)
        visited2[i]=false;

    //bool *=new bool[r*r];
    if (st==NULL) return;
    ff.Init();
    Tile *cur=st;
    int r2=r*r;

    ff.Run(cur);

    //visited.push_back(cur);
    if (r!=0)
    {
        visited2[r+r*r*2]=true;
        for (int my=-r;my<=r;my++)
        {
            int xs=sqrt(r2-my*my);//Galima greiciau su bresham circle'u
            for (int mx=-xs;mx<=xs;mx++)
            {
                TCODLine::init(0,0,mx,my);
                cur=st;
                int lx,ly,xx,yy;
                xx=0;
                yy=0;
                lx=0;
                ly=0;

                while (!TCODLine::step(&xx,&yy))
                {
                    int dx=xx-lx;
                    int dy=yy-ly;


                    if (dx>0)
                        if (dy>0)
                            cur=cur->exits[E_SE];
                        else if (dy==0)
                            cur=cur->exits[E_E];
                        else
                            cur=cur->exits[E_NE];
                    else if (dx==0)
                        if (dy>0)
                            cur=cur->exits[E_S];

                        else
                            cur=cur->exits[E_N];
                    else
                        if (dy>0)
                            cur=cur->exits[E_SW];
                        else if (dy==0)
                            cur=cur->exits[E_W];
                        else
                            cur=cur->exits[E_NW];
                    if (cur==NULL) break;


                    //if (!cur->see())break;
                    //if (cur->doors==DOORSCLOSED)break;
                    //if (find(visited.begin(),visited.end(),cur)==visited.end())
                    if (!visited2[xx+r+(yy+r)*r*2])
                        if (!ff.Run(cur))
                            break;
                    //visited.push_back(cur);
                    visited2[xx+r+(yy+r)*r*2]=true;

                    lx=xx;
                    ly=yy;
                }
            }
        }
    }
    ff.Finish();
}
tiles Map::FloodFill(Tile *st,TileFunct &ff)
{
    tiles ret;
    std::list <Tile *> unprocessed;
    unprocessed.push_back(st);
    while (!unprocessed.empty())
    {
        Tile *cur=*unprocessed.begin();
        unprocessed.pop_front();
        if (!inset(cur,ret))
        {
            if (ff.Run(cur))
            {
                ret.push_back(cur);
                for (int i=0;i<E_LAST;i++)
                    if (cur->GetSide((SIDES)i))
                        if (!inset(cur->GetSide((SIDES)i),ret))
                            unprocessed.push_back(cur->GetSide((SIDES)i));
            }
        }
    }
    return ret;
}
void Map::SetMat(Tile *t,unsigned stdmat)
{

    stdmat%=MaterialManager::Get().GetCount();
    t->SetMaterial(MaterialManager::Get().GetMaterial(MATT_ALL,stdmat));
}
void Map::SetMat(Tile *t,unsigned stdmat,int h)
{
    stdmat%=MaterialManager::Get().GetCount();
    t->SetMaterial(MaterialManager::Get().GetMaterial(MATT_ALL,stdmat),h);

}
Map::~Map()
{
    delete random;
    //dtor
}
void Map::ExplodeCircle(int r,Tile *cur)
{
    class boom:public TileFunct
    {
        std::vector <ITile*> list;
        Map *tt;
    public:
        boom(Map *t)
        {
            tt=t;
        }
        void Init()
        {
            list.clear();
        }
        bool Run(ITile *t)
        {
            bool h=t->PassibleLight();
            if (t->GetHeight()>=0)
                list.push_back(t);
            if (!h)
                return false;
            return true;
        }
        void Finish()
        {
            for (int i=0;i<list.size();i++)
                tt->ExplodeTile(dynamic_cast<Tile*>(list[i]));
        }
    }bomb(this);
    //mymap->RunCircle(r,cur,flame);
    int rr=0;
    cur->RemoveTop();
    while ((cur->GetTop())&&(cur->GetTop()->flags&MAT_EXPLODE))
    {
        rr++;
        cur->RemoveTop();
    }

    RunCircle(rr+r,cur,bomb);
}
void Map::ExplodeTile(Tile *trg)
{
    Tile *cur=trg;

    if (cur->GetTop())
    {
        Material *ttop=cur->GetTop();
        //if (ttop->flags&MAT_EXPLODE)
        //{
        //cur->RemoveTop();
        //ExplodeCircle(2+random->getInt(0,2),cur);
        //}
        {
            class rrfunct:public RayFunct
            {

                //int stmass;
                Material *mat;
                TCODRandom rr;
            public:
                int mass;
                rrfunct(int m,Material *mm)
                {
                    //stmass=m;
                    mass=m;
                    mat=mm;
                }
                void Init(ITile *trg)
                {
                    //mass=stmass;
                }
                bool Run(ITile *trg)
                {

                    if (trg->PassibleSolid())
                    {
                        if (mass>0)
                        {
                            if (rr.getInt(0,10)>9)
                            {
                                mass--;
                                trg->AddItem(new RawMaterial(mat,SIZE_BACKPACK));
                                //trg->GetStack()->StackAll();
                                trg->StackAndPack();
                            }
                        }
                        else
                            return false;
                        return true;
                    }
                    return false;
                }
                void Finish(ITile *trg)
                {
                    if (mass)
                        trg->AddTemp(rr.getInt(5,10));
                    /*if (mass>0)
                    {
                        for (int i=0;i<mass;i++)
                            trg->AddItem(new RawMaterial(mm,SIZE_BACKPACK));
                        trg->StackAndPack();
                    }*/

                }
            }rfk(10,ttop);
            cur->RemoveTop();
            for (int i=0;i<3;i++)
            {
                RunRandomRay(10,(SIDES)random->getInt(E_UP,E_DOWN),cur,rfk,50);
            }
            if (rfk.mass>0)
                cur->AddItem(new ItemPile(new RawMaterial(ttop,SIZE_BACKPACK),rfk.mass));
            cur->StackAndPack();
        }
    }
}
void Map::lqdSim()
{

    for (int i=0;i<places.size();i++)
    {
        if (places[i].placetype==E_PLACE_FOUNTAIN)
        {
            ITile *ttt=places[i].t;
            //ttt->AddTop(stdMaterials[0]);
            if (ttt->GetEntityCount()==0)
                new Creature_Fighter(ttt);
            //int h=ttt->GetHeight();
            //if (h+1<MAXMAT)
            //   ttt->mats[h+1]=stdMaterials[0];
        }
        if (places[i].placetype==E_PLACE_EATER)
        {
            ITile *ttt=places[i].t;
            //if (ttt->IsLiquid()||ttt->IsGas())
            ttt->RemoveTop();
            //
            //    ttt->mats[ttt->GetHeight()]=NULL;
        }
        if (places[i].placetype==E_PLACE_LAVA)
        {
            ITile *ttt=places[i].t;
            ttt->SetTemp(CONST_LAVA_TEMP);
        }
    }

    for (int i=0;i<map.size();i++)
    {
        map[i]->EntityReset();
        /*ISpecial *a=cur->GetSpecial();
        if (a!=NULL)
        {
            a->ResetDone();
        }*/
    }
    for (int i=0;i<map.size();i++)
    {
        Tile *cur=map[i];

        /*ISpecial *a=cur->GetSpecial();
        if (a!=NULL)
        {
            a->Tick();
            if (a->IsDead())
            {
                delete a;
                cur->SetSpecial(NULL);
            }
        }*/
        cur->EntityTick();
        if (cur->GetStack())
        {
            if ((cur->GetSolidH()<0)&&(cur->PassibleSolid(E_DOWN)))
            {
                cur->DropStackDown();
            }
        }
        Material *b=cur->GetTop();
        if (b)
        {

            if (b->flags&(MAT_IGNITES))
                if (cur->GetTemp()>b->IgniteTemp)
                    if ((!cur->exits[E_UP])||(cur->exits[E_UP]->GetHeight()<0))
                        if (!cur->GetEntityCount())
                        {
                            cur->RemoveMatter();
                            //spill flame?
                            if (b->flags&MAT_LONGBURN)
                                cur->AddEntity(new ISP_fire(random->getInt(100,200),b->BurnTemp,cur));
                            else
                                cur->AddEntity(new ISP_fire(random->getInt(10,20),b->BurnTemp,cur));//random->getInt(b->BurnTemp-10,b->BurnTemp+10),cur));
                        }
            if (b->flags&(MAT_EXPLODE))
                if (cur->GetTemp()>b->IgniteTemp)
                {
                    ExplodeCircle(2+random->getInt(0,2),cur);

                }
        }

        //if (0)

#ifndef MODEL_ACCURATE
        for (int j=0;j<1;j++)//6;j++)
        {
            int r=random->getInt(0,E_LAST-1);
            Tile *trg=cur->exits[r];
            if (trg)
            {
                int diff=cur->temp-trg->temp;
                if (diff>0)
                {
                    // 1 tickas -> 10 laipsniu
                    // 0 rez
                    // 1 tickas -> 1 laipsnis
                    // 1000 rez;
                    // 1 tickas -> 1/4 prarast 1 laipsny?
                    // 4000 rez;
                    int tran;

                    if (cur->GetTop()&&trg->GetTop())
                    {

                        int rez;
                        rez=cur->GetTop()->ResistTemp+trg->GetTop()->ResistTemp;
                        //if(cur->temp-trg->temp>333)
                        //   rez/=3;
                        rez/=2;

                        if (rez<=1000)
                            tran=10+(rez*(-9))/1000;
                        else
                        {
                            if (random->getInt(0,100)<100000/rez)
                                tran=1;
                            else
                                tran=0;
                            if (rez>=100000)
                                tran=0;
                        }
                        tran*=6;// optimatizacija...
                        //if(tran>0)tran=rand()%(tran/2)+tran/2;
                        if (tran>diff/2)tran=diff/2;
                        cur->exits[r]->temp+=tran;
                        cur->temp-=tran;
                    }
                    /*if(cur->IsLiquid())
                        tran=rand()%6;
                    if(cur->IsGas())
                        tran=rand()%10;
                    if(cur->IsSolid())
                        tran=rand()%4;*/


                    //tran*=((float)(trg->mymat->ResistTemp*trg->liqh)/(float)(cur->mymat->ResistTemp*cur->liqh));



                }
            }
        }
#else
        for (int j=0;j<1;j++)//6;j++)
        {
            int r=random->getInt(0,E_LAST-1);
            Tile *trg=cur->exits[r];
            if (trg)
            {
                float diff=cur->GetTemp()-trg->GetTemp();
                if (diff>0)
                {
                    //hmz turetu but T=T_a+(T_0-T_a)*e^(-kt)
                    int tran;

                    if (cur->GetTop()&&trg->GetTop())
                    {

                        //int rez;
                        //rez=cur->GetTop()->ResistTemp+trg->GetTop()->ResistTemp;
                        //if(cur->temp-trg->temp>333)
                        //   rez/=3;
                        //rez/=2;
                        //float k;
                        //if(rez!=0)
                        //k=rez/1000.0f;

                        tran=diff*cur->GetTop()->ResistTemp*trg->GetTop()->ResistTemp;//*exp(-k);
                        if (tran==0)
                            tran=1;
                        if (tran>diff/2)tran=diff/2;
                        trg->AddTemp(tran);
                        cur->AddTemp(-tran);
                    }
                }
            }
        }
#endif
        bool isgas=cur->IsGas();
        if (cur->IsLiquid()||isgas)
        {

            /*if (cur->liqh==1)
                if (rand()%1000<=1)
                    cur->liqh=0;*/
            int curh=cur->GetHeight();
            if (cur->GetSolidH()<0)
               // if (cur->exits[E_DOWN])
                    if(cur->PassibleLiquid(E_DOWN))
                    if (cur->exits[E_DOWN]->GetHeight()<MAXMAT-2)
                    {
                        //int l=(MAXMAT-cur->exits[E_DOWN]->GetHeight());
                        //if (l>curh)
                        //{
                        //cur->exits[E_DOWN]->temp=(cur->liqh*cur->temp+cur->exits[E_DOWN]->liqh*cur->exits[E_DOWN]->temp)/7;
                        //cur->exits[E_DOWN]->liqh+=cur->liqh;
                        //cur->TransferMass(cur->exits[E_DOWN]);
                        /*int ch=cur->exits[E_DOWN]->GetHeight();
                        cur->exits[E_DOWN]->mats[ch+1]=cur->GetTop();
                        cur->mats[curh]=NULL;*/
                        if (isgas)
                        {
                            if (random->getInt(0,100)==0)
                                cur->TransferMass(cur->exits[E_DOWN]);
                        }
                        else
                            cur->TransferMass(cur->exits[E_DOWN]);

                        //cur->liqh=0;
                        //cur->temp=21;
                        /*}
                        else
                        {
                            int trans=7-cur->exits[E_DOWN]->liqh;
                            cur->exits[E_DOWN]->temp=(trans*cur->temp+cur->exits[E_DOWN]->liqh*cur->exits[E_DOWN]->temp)/7;
                            cur->liqh-=trans;
                            cur->exits[E_DOWN]->liqh=7;
                        }*/
                    }
            for (int j=0;(j<6)&&(curh>0);j++)
            {

                int r;
                r=random->getInt(E_NW,E_SE);

                curh=cur->GetHeight();
                if (isgas)
                    if (random->getInt(0,10)>4)
                    {
                        if(cur->PassibleLiquid(E_UP))
                        //if (cur->exits[E_UP])
                            //if (cur->exits[E_UP]->GetSolidH()<0)
                                cur->TransferMass(cur->exits[E_UP]);

                    }
                ITile *trg=cur->exits[r];
                //if (trg) //passible matter(r) turi jau checka
                    //if (cur->exits[r]->walk())
                    if ((cur->GetDeepness()>0)||isgas)
                        if (cur->PassibleLiquid((SIDES)r))//(trg->PassibleMatter())
                            if (trg->GetHeight()<curh)
                            {

                                /*cur->exits[r]->temp=cur->exits[r]->temp*cur->exits[r]->liqh+cur->temp;
                                cur->exits[r]->liqh++;
                                cur->exits[r]->temp/=cur->exits[r]->liqh;
                                cur->liqh--;*/
                                cur->TransferMass(cur->exits[r]);
                                //cur->Sort();

                            }


            }
            //cur->Sort();//WOW THATS SLOW
        }
        //some temp decay
        /*
        if (rand()%100>80)
        {
            if (cur->temp>21)
                cur->temp--;
            if (cur->temp<21)
                cur->temp++;
        }*/
        //cur->Sort();
    }

}
void Map::ConnectImmediate(Tile *a,Tile *b,SIDES dir)
{
    if (a)
        a->exits[dir]=b;
    if (b)
        b->exits[E_DOWN-dir]=a;
}
void Map::ConnectClose(Tile *a,Tile *b,SIDES dir)
{
    ConnectImmediate(a,b,dir);

    switch (dir)
    {
    case E_DOWN:
    case E_UP:
        ;//do nothing more
        break;
    case E_NW:
        ConnectImmediate(a->exits[E_N],b,E_W);
        //a->exits[E_N]->exits[E_W]=b;
        //b->exits[E_LAST-E_W]=a->exits[E_N]->exits[E_W];
        ConnectImmediate(a->exits[E_W],b,E_N);
        break;
    case E_N:
        ConnectImmediate(a->exits[E_W],b,E_NE);
        ConnectImmediate(a->exits[E_E],b,E_NW);
        break;
    case E_NE:
        ConnectImmediate(a->exits[E_N],b,E_E);
        ConnectImmediate(a->exits[E_E],b,E_N);
        break;
    case E_E:
        ConnectImmediate(a->exits[E_N],b,E_SE);
        ConnectImmediate(a->exits[E_S],b,E_NE);
        break;
    case E_SE:
        ConnectImmediate(a->exits[E_S],b,E_E);
        ConnectImmediate(a->exits[E_E],b,E_S);
        break;
    case E_S:
        ConnectImmediate(a->exits[E_E],b,E_SW);
        ConnectImmediate(a->exits[E_W],b,E_SE);
        break;
    case E_SW:
        ConnectImmediate(a->exits[E_S],b,E_W);
        ConnectImmediate(a->exits[E_W],b,E_S);
        break;
    case E_W:
        ConnectImmediate(a->exits[E_N],b,E_SW);
        ConnectImmediate(a->exits[E_S],b,E_NW);
        break;
    case E_LAST:
        break;
    }

}
