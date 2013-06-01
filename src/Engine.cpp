#include "Engine.h"
#include <iostream>
Engine::Engine()
{
    //TODO read config.
    mycfg.w=100;
    mycfg.h=75;
    mycfg.mw=200;
    mycfg.mh=150;
    mycfg.fps=250;
    TCODConsole::initRoot(mycfg.w,mycfg.h,"Non standart space",false);

    TCODSystem::setFps(mycfg.fps); // limit framerate to 25 frames per second
    main=TCODConsole::root;
    editor=new TCODConsole(mycfg.w/3,mycfg.h/3);
    inventory=new TCODConsole(mycfg.w/3,mycfg.h/3);
    onground=new TCODConsole(mycfg.w/3,mycfg.h/3);
    main->setKeyboardRepeat(10,25);
    marked=0;
    Mode_PauseSim=false;
    curtool=NULL;
    CurrentItem=0;
    //ctor
}
Engine::~Engine()
{
    delete editor;
    delete inventory;
    delete onground;
    //delete main;
    //dtor
}
void Sleep(float t)
{
    float time;
    time=TCODSystem::getElapsedMilli();
    while (TCODSystem::getElapsedMilli()-time<t);
}
char Blinker()
{
    float time;
    time=TCODSystem::getElapsedSeconds();
    if ((int)time%2)
        return '_';
    return ' ';
}
Tile *Engine::Intro()
{

    for (int i=0;i<E_LAST;i++)
    {
        SIDES ii=(SIDES)i;
        SIDES cw=closest(ii,false);
        SIDES ccw=closest(ii,true);
        std::cout<<cw;
        std::cout<<std::endl;
    }
    bool exist=false;
    while (true)
    {
        main->clear();
        main->printLeft(0,0,TCOD_BKGND_NONE,">Initiating space bend program.Please wait...");
        main->printLeft(0,1,TCOD_BKGND_NONE,">Found Os.Ver:%s",AutoVersion::FULLVERSION_STRING);
        main->flush();
        Sleep(123);
        main->printLeft(0,2,TCOD_BKGND_NONE,">Done. Commands supported:PROC,QUIT,COLORTEST,CHARMAP,NEW,EMPTY.");
        main->flush();
        Sleep(222);
        main->printLeft(0,3,TCOD_BKGND_NONE,">Enter command or extension:");
        main->flush();
        Sleep(111);
        main->printLeft(0,4,TCOD_BKGND_NONE,">");
        main->flush();
        std::string pp;
        char in=0;
        while (in!=13)
        {

            if ((in!=8)&&(in!=0))
                pp+=in;
            if (in==8)
                pp=pp.substr(0,pp.size()-1);
            in=main->checkForKeypress().c;
            main->printLeft(1,4,TCOD_BKGND_NONE,"                           ",pp.c_str());
            main->printLeft(1,4,TCOD_BKGND_NONE,"%s%c",pp.c_str(),Blinker());
            main->flush();
        }
        if (pp=="EMPTY")
        {
            mymap=new Map(3,3);
            mapname="empty";
            return mymap->GetSomeTile();
        }
        if (pp=="PROC")
        {
            int a;
            TCODZip file;
            file.putInt(1);
            file.putInt(10);
            file.putInt(16);
            file.putInt(1);
            file.putInt(3);
            file.saveToFile("File.map");
            mymap=new Map("File.map",a);
            return mymap->GetSomeTile();
        }
        //std::cout<<pp<<"\n"<<pp.substr(4)<<"\n";
        /*
        std::cout<<"rez\tperturna\tturnusk\tLaiko(s)\n";
        for(int i=0;i<10000;i+=1000)
        {
            float ex;
            ex=exp(-(float)i/1000);
        std::cout<<i<<"\t"<<ex<<"\t"<<1/ex<<"\t"<<0.05/ex<<"\n";
        }
        */

        if (pp.substr(0,4)=="QUIT")
            exit(0);
        if (pp=="COLORTEST")
        {
            main->clear();
            int h=main->getHeight();
            int w=main->getWidth();
            int o=0;
            while (main->checkForKeypress().c!='q')
            {
                o++;
                o%=3600;
                for (int i=0;i<w;i++)
                    for (int j=0;j<h;j++)
                    {

                        TCODColor col;
                        col.setHSV((float)o/10.0f,(float)i/(float)w,(float)j/(float)h);
                        main->setForegroundColor(col);
                        main->putChar(i,j,219,TCOD_BKGND_NONE);


                    }
                main->flush();
            }
            exit(0);
        }
        if (pp=="CHARMAP")
        {
            int h=main->getHeight();
            main->clear();
            for (int i=0;i<256;i++)
                main->printLeft((i/h)*7,i%h,TCOD_BKGND_NONE,"%c->%i",(char)i,i);
            main->flush();
            while (main->checkForKeypress().c!='q');
            exit(0);
        }
        if ( pp.substr(0,4)=="NEW(")
        {
            //int a=pp.find(')');
            int kb=pp.find(',');
            mapname=pp.substr(4,kb-4);
            std::cout<<mapname<<"\n";
            int coords[]={-1,-1,-1};

            for (int i=0;i<3;i++)
            {
                int lkb=kb;

                kb=pp.find(',',kb+1);
                std::string nname=pp.substr(lkb+1,kb-lkb-1);
                if (nname!="")
                    if (i<3)
                        coords[i]=atoi(nname.c_str());

            }

            if (pp[pp.size()-1]=='}')
            {
                Mode_Debug=true;
                Mode_NoClip=true;
            }

            std::cout<<coords[0]<<" "<<coords[1]<<" "<<coords[2]<<" "<<"\n";
            if (coords[2]<=0)
                mymap=new Map(coords[0],coords[1]);
            else
                mymap=new Map(coords[0],coords[1],coords[2]);
            //mymap->SetTemp(21);
            //std::cin>>pp;
            //exit(0);
            Tile *a=mymap->GetSomeTile();
            while (a->exits[E_UP])
            {
                a=a->exits[E_UP];
                if (a->GetHeight()<5)
                    break;
            }
            return a;
        }

        exist=false;
        std::fstream aa(pp.c_str());
        if (aa.is_open())
            exist=true;
        aa.close();
        mapname=pp;
        if (exist)
        {
            int a;
            mymap=new Map(pp,a);
            return mymap->GetNum(a);
        }
        main->printLeft(0,5,TCOD_BKGND_NONE,">Error command not found! Reseting...");
        main->flush();
        Sleep(500);
    }
}

void Engine::TileEd(char key,Tile *cur)
{
    editor->setBackgroundColor(TCODColor::darkYellow);
    editor->printFrame(0,0,mycfg.w/3,mycfg.h/3,true,"Tile Editor");
    editor->printLeft(1,1,TCOD_BKGND_NONE,"Press . to mark tile");
    char mt=' ';
    char crnew='-';
    static bool createnew;
    static bool locked=false;
    //static bool isnew=false;
    static int ww=1,hh=1;
    //static int pp=0;
    static int curtemp=21;
    if (marked)
        mt=marked->GetPic();
    if (createnew)
        crnew='+';
    editor->printLeft(1,2,TCOD_BKGND_NONE,"Cur tile:%c,Create new:%c",mt,crnew);
    editor->printLeft(1,3,TCOD_BKGND_NONE,"(C)New %ix%i space",ww,hh);


    for (int i=0;i<MAXMAT;i++)
        if (cur->GetMaterial(i))
        {
            editor->printLeft(1+i,5,TCOD_BKGND_NONE,"M");
            editor->setFore(1+i,5,cur->GetMaterial(i)->color);
        }
    editor->printLeft(1,6,TCOD_BKGND_NONE,"T locked(%c) at:%i",(locked)?'+':'-',curtemp);
    switch (key)
    {
    case '.':
        /*if (isnew)
        {
            delete marked;
            isnew=false;
        }*/
        marked=cur;
        break;

    case 'c':
        createnew=!createnew;
        break;
    case 'C':
    {
        tiles ret;
        if ((ww==0)||(hh==0))
            break;
        for (int i=0;i<ww*hh;i++)
        {
            Tile *tiletemp=new Tile();
            mymap->SetTile(tiletemp,0);
            ret.push_back(tiletemp);
        }
        Map::ConnectMap(ret,ww,hh);
        marked=ret[0];
        mymap->RegTiles(ret);
    }
    break;
    case 'b':
        {
            IEntity *bb=new clBuilding(BTManager::Get().GetTemplate(BID_FLOOR),MaterialManager::Get().GetRandom(&random,MATT_STONE));
            cur->AddEntity(bb);
            bb->SetTile(cur);
        }
    break;
     case 'B':
        {
            IEntity *bb=new clBuilding(BTManager::Get().GetTemplate(BID_GRATE),MaterialManager::Get().GetRandom(&random,MATT_STONE));
            cur->AddEntity(bb);
            bb->SetTile(cur);
        }
    break;
    case 'i':
        curtemp+=100;
        break;
    case 'u':
        curtemp-=100;
        break;
    case 'l':
        locked=!locked;
        break;
    case '7':
        if (createnew)
            mymap->ConnectClose(cur,marked,E_NW);
        else
            cur->exits[E_NW]=marked;
        break;
    case '8':
        if (createnew)
            mymap->ConnectClose(cur,marked,E_N);
        else
            cur->exits[E_N]=marked;
        break;
    case '9':
        if (createnew)
            mymap->ConnectClose(cur,marked,E_NE);
        else
            cur->exits[E_NE]=marked;
        break;
    case '4':
        if (createnew)
            mymap->ConnectClose(cur,marked,E_W);
        else
            cur->exits[E_W]=marked;
        break;
    case '1':
        if (createnew)
            mymap->ConnectClose(cur,marked,E_SW);
        else
            cur->exits[E_SW]=marked;
        break;
    case '2':
        if (createnew)
            mymap->ConnectClose(cur,marked,E_S);
        else
            cur->exits[E_S]=marked;
        break;
        break;
    case '3':
        if (createnew)
            mymap->ConnectClose(cur,marked,E_SE);
        else
            cur->exits[E_SE]=marked;
        break;
        break;
    case '6':
        if (createnew)
            mymap->ConnectClose(cur,marked,E_E);
        else
            cur->exits[E_E]=marked;
        break;
        break;
    case '>':
        //cur->exits[E_DOWN]=marked;
        mymap->ConnectImmediate(cur,marked,E_DOWN);
        break;
    case '<':
        //cur->exits[E_UP]=marked;
        mymap->ConnectImmediate(cur,marked,E_UP);
        break;
    case 'p':
        //SIDES s=(SIDES)(rand()%(int)E_LAST);
        mymap->ConnectClose(cur,marked,E_N);
        break;
    case 'w':
        marked->temp+=100;
        break;
    case 'W':
        marked->temp+=1000;
        break;
    case 's':
        marked->temp-=100;
        break;
    case 'S':
        marked->temp-=1000;
        break;

    case '5':
        //mymap->SetMat(marked,ww);
    {
        int h=cur->GetHeight();
        if (h<MAXMAT-1)
            mymap->SetMat(cur,ww,h+1);
    }
    //if (!isnew)
    //{
    /*marked=new Tile();
    marked->pic='!';
    marked->walk=true;
    marked->r=125;
    marked->g=125;
    marked->b=125;
    mymap->RegTile(marked);*/
    //}
    break;
    case ']':
        ww++;
        break;
    case '[':
        ww--;
        break;
    case '-':
        hh--;
        break;
    case '=':
        hh++;
        break;
    case 't':
        mymap->AddPlace(cur,E_PLACE_FOUNTAIN);
        break;
    case 'g':
        mymap->AddPlace(cur,E_PLACE_EATER);
        break;
    case 'd':
        if (cur->doors==NODOORS)
            cur->doors=DOORSCLOSED;
        else
            cur->doors=NODOORS;
        break;

    }
    if (ww<0)ww=0;
    if (hh<0)hh=0;
    //editor->flush();
    if (marked)
        if (locked)
            marked->temp=curtemp;
    TCODConsole::blit(editor,0,0,mycfg.w/3,mycfg.h/3,main,mycfg.w*2/3,0,200);
    //main->printFrame(0,0,mycfg.w,mycfg.h,false,"Tile Editor");
    //main->setBackgroundColor(TCODColor::darkRed);
    //main->hline(0,25,mycfg.w,TCOD_BKGND_ALPHA(0.5f));
}
void Engine::UpdInventory(ItemStack *t,ItemStack *gr)
{
    inventory->setBackgroundColor(TCODColor::darkYellow);
    inventory->printFrame(0,0,mycfg.w/3,mycfg.h/3,true,"Inventory");
    if (CurrentItem<0)CurrentItem=0;
    if (CurrentItem>=t->GetCount())CurrentItem=t->GetCount()-1;
    if (t!=NULL);
    for (unsigned i=0;i<t->GetCount();i++)
    {
        inventory->printLeft(1,i+1,TCOD_BKGND_NONE,"%c%c %s",(CurrentItem==i)?'>':' ',t->GetItem(i)->GetPic(),t->GetItem(i)->GetName().c_str());
    }
    onground->setBackgroundColor(TCODColor::darkYellow);
    onground->printFrame(0,0,mycfg.w/3,mycfg.h/3,true,"On ground");
    if (gr!=NULL)
        for (unsigned i=0;i<gr->GetCount();i++)
        {
            onground->printLeft(1,i+1,TCOD_BKGND_NONE,"%c %s",gr->GetItem(i)->GetPic(),gr->GetItem(i)->GetName().c_str());
        }
    TCODConsole::blit(onground,0,0,mycfg.w/3,mycfg.h/3,main,mycfg.w*2/3,mycfg.h*2/3,200);
    TCODConsole::blit(inventory,0,0,mycfg.w/3,mycfg.h/3,main,mycfg.w*2/3,mycfg.h*1/3,200);
}
bool Engine::ControlToSide(char c,SIDES &s)
{
    SIDES &ret=s;
    switch (c)
    {
    case '7':
        ret=E_NW;
        return true;
    case '8':
        ret=E_N;
        return true;
    case '9':
        ret=E_NE;
        return true;
    case '4':
        ret=E_W;
        return true;
    case '1':
        ret=E_SW;
        return true;
    case '2':
        ret=E_S;
        return true;
    case '3':
        ret=E_SE;
        return true;
    case '6':
        ret=E_E;
        return true;
    case '<':
        if (!Mode_Debug)
        {
            break;
        }
        ret=E_UP;
        return true;
    case '>':
        if (!Mode_Debug)
        {
            break;
        }
        ret=E_DOWN;
        return true;
    }
    return false;
}
void Engine::GameTick()
{
    main->clear();
    float fp=TCODSystem::getLastFrameLength();
    //mymap->PrintMapRot(main,0,0,mycfg.w,mycfg.h,40,cur,MyRot);
    mymap->PrintMapXX(main,0,0,mycfg.w,mycfg.h,40, dynamic_cast<Tile*>(player.GetTile()));
    if (!cred)
        cred=TCODConsole::renderCredits(10,10,true);
    ITiles mytiles;
    if(player.GetTile()->GetSide(E_NW))
    if(IsNormal(player.GetTile()->GetSide(E_NW),3,3,mytiles))
        main->setBack(mycfg.w/2,mycfg.h/2,TCODColor::black);
    else
        main->setBack(mycfg.w/2,mycfg.h/2,TCODColor::red);
    //for(int i=0;i<3;i++)
    //    for(int j=0;j<3;j++)
    //        mytiles[i+j*3]->SetHeight(j,MaterialManager::Get().GetMaterial(MATT_ALL,CurrentTile));
    UpdInventory(&player.GetInventory(),player.GetTile()->GetStack());
    if (player.GetTile()->GetSide(E_UP))
        main->putChar(0,0,24,TCOD_BKGND_NONE);
    if (player.GetTile()->GetSide(E_DOWN))
        main->putChar(0,1,25,TCOD_BKGND_NONE);


    if (Mode_Debug)
    {
        main->setForegroundColor(TCODColor::white);
        main->printLeft(1,3,TCOD_BKGND_NONE,"Temp:%i,%f",player.GetTile()->GetTemp(),fp);
        main->setBack(mycfg.w/2,mycfg.h/2,TCODColor::red);
        main->printLeft(3,0,TCOD_BKGND_NONE,"R:%i ,Mat:%s",R,MaterialManager::Get().GetMaterial(MATT_ALL,CurrentTile)->name.c_str());

    }


}
void Engine::Run()
{
    int keycode;
    AI_Human *t=new AI_Human(&player,&keycode);

    // E_UP,E_NW,E_N ,E_NE,E_E ,E_W ,E_SW,E_S ,E_SE,E_DOWN,E_LAST
    int keybinds[]={'<','7','8','9','6','4','1','2','3','>'};
    for (int i=0;i<E_LAST;i++)
        t->AddActionKeyd(new Action_Move((SIDES)i),keybinds[i]);
    tiles closure;
    cred=false;

    Mode_Debug=false;
    Mode_NoClip=false;
    Tile *cur,*lcur;
    cur=Intro();
    Creature_Snaky mon(10);
    mon.SetColor(TCODColor::red);
    mon.SetPic('S','s');
    mon.SetSpeed(5);
    {
        ITile *tt=cur->GetSide((SIDES)random.getInt(E_UP,E_DOWN));
        while (!tt)
            tt=cur->GetSide((SIDES)random.getInt(E_UP,E_DOWN));
        mon.SetTile(tt);
    }
    //mon.SetAi(new AI_Wander(&mon));
    AI_Wander *ppp=new AI_Wander(&mon);
    //Mode_PauseSim=true;
    //mymap=new Map(mycfg.mw,mycfg.mh);
    //mymap=new Map("test.map");
    TCOD_key_t a;

    Editor_on=false;
    bool walk=false;
    int ctile=0;
    CurrentTile=0;
    SIDES from;
    SIDES to;
    //Mob player;
    player.SetColor(TCODColor::darkGray);
    player.SetPic((int)'@');
    player.SetTile(cur);
    player.SetSpeed(0);
    //cur->SetMob(&player);
    R=1;
    //int MyRot=0;
    player.GetInventory().AddItem(new ItemTool(MaterialManager::Get().GetMaterial(MATT_METAL,0),TOOLTYPES::TYPE_PICK));
    player.GetInventory().AddItem(new ItemTool(MaterialManager::Get().GetMaterial(MATT_METAL,0),TOOLTYPES::TYPE_CHISEL));
    player.GetInventory().AddItem(new ItemTool(MaterialManager::Get().GetMaterial(MATT_METAL,0),TOOLTYPES::TYPE_BUILD));
    while ((!main->isWindowClosed())&&(a.c!='q'))
    {

        GameTick();

        if (Editor_on)
            TileEd(a.c,dynamic_cast<Tile*>(player.GetTile()));
        main->flush();
        a= main->checkForKeypress();
        lcur=cur;
        if (a.vk==TCODK_PRINTSCREEN)TCODSystem::saveScreenshot(NULL);
        walk=ControlToSide(a.c,to);
        from=(SIDES)(E_DOWN-to);
        if(!Editor_on)
            keycode=a.c;

        //if(a.c=='!')Mode_Debug=!Mode_Debug;
        cur=dynamic_cast<Tile*>(player.GetTile());
        if (!Editor_on)
            switch (a.c)
            {

            case '!':
                Mode_Debug=!Mode_Debug;
                break;

            case '5':
                if (Mode_Debug)
                    mymap->SetTile(cur,ctile);
                break;
            case '[':
                if (cur->GetHeight()>=0)
                    cur->RemoveTop();
                break;
            case ']':
                cur->AddTop(MaterialManager::Get().GetMaterial(MATT_ALL,CurrentTile));
                break;
            case 'D':
                IEntity *a=cur->GetEntity();
                if (a)
                {
                    cur->RemoveEntity(a);
                    delete a;
                }
                break;
            case 'e':
            {
                //TCODRandom random;
                IEntity *bb=new clBuilding(BTManager::Get().GetTemplate(BID_WINDOW),MaterialManager::Get().GetRandom(&random,MATT_STONE));
                cur->AddEntity(bb);
                bb->SetTile(cur);
            }
            break;
            case '+':
                R++;
                break;
            case '-':
                if (R>0)R--;
                break;


            case 'o':
                mymap->lqdSim();
                break;

            case 'a':
            {
                //TCODRandom random;
                IEntity *bb=new clBuilding(BTManager::Get().GetTemplate(BID_WALL),MaterialManager::Get().GetRandom(&random,MATT_STONE));
                cur->AddEntity(bb);
                bb->SetTile(cur);
            }
            break;
            case 'A':
            {
                //TCODRandom random;
                IEntity *bb=new clBuilding(BTManager::Get().GetTemplate(BID_FLOOR),MaterialManager::Get().GetRandom(&random,MATT_STONE));
                cur->AddEntity(bb);
                bb->SetTile(cur);
            }
            break;
            case 's':
                if (Mode_Debug)
                    mymap->SaveMap(mapname,mymap->FindTile(cur));
                break;
            case 'z':
                if (Mode_Debug)
                    Mode_PauseSim= !Mode_PauseSim;
                break;
            case 'c':
                for (int i=E_UP;i<E_LAST;i++)
                    if (cur->exits[i])
                    {
                        clBuilding *a=GetBuildingOfType(cur->exits[i],BID_DOOR);
                        if (a)
                            if (!a->GetData())
                            {
                                a->GetData()=1-a->GetData();
                                break;
                            }

                    }
                break;
            case 'n':
                Mode_NoClip=!Mode_NoClip;
                if(Mode_NoClip)
                    player.SetCrFlags(CreatureFlags::F_NOCLIP);
                else
                    player.SetCrFlags(0);
                break;
            case 'N':
                if (Mode_Debug)
                {
                    class cflame:public TileFunct
                    {

                    public:
                        bool Run(ITile *t)
                        {
                            Material *mat=t->GetTop();
                            if (mat)
                            {
                                TCODRandom rand;
                                SIDES s=(SIDES)rand.getInt(0,E_LAST);
                                for (int i=0;i<10;i++)
                                {
                                    s=(SIDES)rand.getInt(0,E_LAST);
                                    while (!t->GetSide(s))
                                    {
                                        s=(SIDES)rand.getInt(0,E_LAST);
                                    }
                                    t->GetSide(s)->AddItem(new RawMaterial(mat,SIZE_BACKPACK));
                                    Material *mat2;
                                    mat2=t->GetSide(s)->GetStack()->MakesRaw();
                                    if (mat2)
                                        t->GetSide(s)->AddTop(mat2);
                                }
                                t->RemoveTop();
                            }
                            return true;
                        }
                    }flame;
                    mymap->RunCircle(R,cur,flame);
                }
                break;
            case 'M':
            {
                clPather cc(cur,R,marked);
                TilePath a=cc.RunSolid();
                for (unsigned i=0;i<a.size();i++)
                    //a[i]->SetSpecial(new ISP_fire(25,10,a[i]));
                    //a[i]->SetHeight(1,mymap->GetMat(CurrentTile));
                    /*if (a[i]->GetSpecial()==NULL)
                        a[i]->SetSpecial(new ISP_glow(a[i]));
                        */
                    a[i]->AddEntity(new ISP_glow(a[i]));
            }
            break;
            case 'm':
            {
                clPather cc(cur,R,marked);
                TilePath a=cc.Run();
                for (unsigned i=0;i<a.size();i++)
                    //a[i]->SetSpecial(new ISP_fire(25,10,a[i]));
                    //a[i]->SetHeight(1,mymap->GetMat(CurrentTile));
                    /*if (a[i]->GetSpecial()==NULL)
                        a[i]->SetSpecial(new ISP_glow(a[i]));*/
                    a[i]->AddEntity(new ISP_glow(a[i]));
            }
            break;


            case 'u':
                new Creature_Fighter(cur);
                break;
            case 'b':

                /*if (Mode_Debug)
                {
                    class cflame:public TileFunct
                    {
                    public:
                        bool Run(ITile *t)
                        {
                            t->AddEntity(new ISP_fire(rand()%30+30,40+rand()%10,t));
                            return true;
                        }
                    }flame;
                    mymap->RunCircle(r,cur,flame);
                }*/
            {
                class rrfunct:public RayFunct
                {
                    int mass;
                    int stmass;
                    TCODRandom rr;
                public:
                    rrfunct(int m)
                    {
                        stmass=m;
                    }
                    void Init(ITile *trg)
                    {
                        mass=stmass;
                    }
                    bool Run(ITile *trg)
                    {
                        if (mass>0)
                        {
                            if (rr.getInt(0,10)>1)
                            {
                                mass--;
                                trg->AddItem(new RawMaterial(MaterialManager::Get().GetRandom(&rr,MATT_STONE),SIZE_BACKPACK));
                                //trg->GetStack()->StackAll();
                                trg->StackAndPack();
                            }
                        }
                        else
                            return false;
                        if (trg->PassibleSolid())
                            return true;
                        return false;
                    }
                    void Finish(ITile *trg)
                    {
                        if (mass>0)
                        {
                            for (int i=0;i<mass;i++)
                                trg->AddItem(new RawMaterial(MaterialManager::Get().GetRandom(&rr,MATT_STONE),SIZE_BACKPACK));
                            trg->StackAndPack();
                        }

                    }
                }rfk(10);
                for (int i=E_UP+1;i<E_DOWN;i++)
                    mymap->RunRandomRay(10,(SIDES)i,cur,rfk,50);
            }
            break;
            case 'x':
                //cur->AddItem(new RawMaterial(MaterialManager::Get().GetMaterial(MATT_ALL,CurrentTile),SIZE_BACKPACK));
                //cur->StackAndPack();
                //closure.push_back(cur);
                //cur->SetSpecial(new ISP_fire(80,0,cur));
                cur->AddEntity(new ISP_glow(cur));
                break;
            case 'X':
            {
                //tiles fill=mymap->CollectEnclosed(closure);
                /*for (int i=0;i<closure.size();i++)

                    if (closure[i]->GetSpecial()==NULL)
                        closure[i]->SetSpecial(new ISP_glow(closure[i]));*/
                //for (unsigned i=0;i<fill.size();i++)
                /*if (fill[i]->GetSpecial()==NULL)
                    fill[i]->SetSpecial(new ISP_glow(fill[i]));*/
                //   fill[i]->AddEntity(new ISP_glow(fill[i]));
                //closure.clear();
                //TCODRandom ran;
                cur->AddItem(ItemManager::Get().Create((ITEM_TYPES::ITEM_TYPE)random.getInt(0,ITEM_TYPES::TYPE_LAST-1)));
                cur->GetStack()->StackAll();
            }
            break;
            /*case ',':
                if (cur->GetStack())
                {
                    IItem *p=cur->GetStack()->GetItem(0);
                    if (p)
                    {
                        cur->GetStack()->RemoveItem(0);
                        player.inventory.AddItem(p);
                        player.inventory.StackAll();
                    }
                }
                break;
            case ';':
            {
                IItem *p=player.inventory.GetItem(0);
                if (p)
                {
                    cur->AddItem(p);
                    //cur->GetStack()->StackAll();
                    Material *raw=cur->GetStack()->MakesRaw();
                    if (raw)
                        cur->AddTop(raw);

                    player.inventory.RemoveItem(0);
                }
                break;
            }*/
            case 'y':
                CurrentItem--;
                break;
            case 'h':
                CurrentItem++;
                break;
            case 'j':
                if (CurrentTile<MaterialManager::Get().GetCount()-1)
                    CurrentTile++;
                break;
            case 'k':
                if (CurrentTile>0)
                    CurrentTile--;
                break;
            case 'd':
            {
                //TCODRandom random;
                IEntity *bb=new clBuilding(BTManager::Get().GetTemplate(BID_DOOR),MaterialManager::Get().GetRandom(&random,MATT_STONE));
                cur->AddEntity(bb);
                bb->SetTile(cur);
            }
            break;
            case 'B':
                if (Mode_Debug)
                {

                    class cflame:public TileFunct
                    {
                        Material *mat;
                        int h;
                    public:
                        cflame(Material *t,int he=7)
                        {
                            mat=t;
                            h=he;
                        };
                        bool Run(ITile *t)
                        {
                            t->SetTemp(21);
                            t->SetHeight(h,mat);
                            return true;
                        }
                    }flame(MaterialManager::Get().GetMaterial(MATT_ALL,CurrentTile));
                    mymap->RunCircle(R,cur,flame);
                }
                break;

            case 'W':
                if (Mode_Debug)
                {
                    class cflame:public TileFunct
                    {
                    public:
                        bool Run(ITile *t)
                        {
                            /*if (t->GetSpecial()==NULL)
                            {
                                t->SetSpecial(new ISP_fire(rand()%30+30,-40-(rand()%10),t));
                            }*/
                            t->AddEntity(new ISP_fire(rand()%30+30,-40-(rand()%10),t));
                            return true;
                        }
                    }flame;
                    mymap->RunCircle(R,cur,flame);
                }
                break;
            case 'w':
                if (Mode_Debug)
                {
                    class cflame:public TileFunct
                    {
                    public:
                        bool Run(ITile *t)
                        {
                            if (t->GetHeight()>5)
                                return true;
                            return false;
                        }
                    }flame;
                    //mymap->RunCircle(r,cur,flame);
                    tiles ret=mymap->FloodFill(cur,flame);
                    for (int i=0;i<ret.size();i++)
                        //ret[i]->SetSpecial(new ISP_glow(ret[i]));
                        ret[i]->AddEntity(new ISP_glow(ret[i]));

                }
                break;
                /*case 'b':

                    printf("tile:%i",mymap->FindTile(cur));
                    break;*/
            }


        /*
        if ((!Mode_NoClip)&&(lcur!=cur))
            cur=TryMove(lcur,cur);
        else
        {
            if (!cur)cur=lcur;
        }*/
        if (ctile<0)ctile=mymap->GetStdTileC()-1;
        if (ctile>=mymap->GetStdTileC())ctile=0;

        if (Mode_Debug)
            if (a.c=='/')Editor_on=!Editor_on;
        //time=TCODSystem::getElapsedMilli();

        //if (time-ltime>100)
        //{
        //ltime=time;
        if (!Mode_PauseSim)
            mymap->lqdSim();
        //}

    }
    delete mymap;
}

void Engine::UseTool(Tile *from,SIDES to)
{
#if 0
    if (!curtool)
        return;
    Tile *t=from->GetSide(to);
    if (!t)
        return;
    SIDES back=(SIDES)(E_DOWN-to);
    switch (curtool->GetToolType())
    {
    case TOOLTYPES::TYPE_PICK:
    {
        int blast[]={back,closest(back,false),closest(back,true)};
        if (t->GetSolidH()<0)
            return;
        if (!t->IsSolid(t->GetHeight()))
            return;
        Material *top=t->GetTop();
        int i=10;
        while (i>0)
        {
            int r=random.getInt(0,1);
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

                r=random.getInt(0,2);
                Tile *ss=t->GetSide((SIDES)blast[r]);
                if (ss&&ss->PassibleMatter())
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
    }
    break;
    case TOOLTYPES::TYPE_CHISEL:
    {
        IItem *p=player.inventory.GetItem(CurrentItem);
        if (p)
            if (p->GetType()==ITEM_TYPES::TYPE_PILE)
            {
                ItemPile *pp=dynamic_cast<ItemPile*>(p);
                if (pp->GetBaseItem()->GetType()==ITEM_TYPES::TYPE_RAWMAT)
                {
                    RawMaterial *mat=dynamic_cast<RawMaterial*>(pp->GetBaseItem());

                    if (mat->GetSize()<SIZE_FULLTILE)
                    {
                        int rem=pp->GetCount();
                        int conv=SizeConvert(mat->GetSize(),(ITEM_SIZE_CLASS)(mat->GetSize()+1),rem);
                        if (conv==1)
                        {
                            player.inventory.AddItem(new RawMaterial(mat->GetMaterial(),(ITEM_SIZE_CLASS)(mat->GetSize()+1)));
                        }
                        if (conv>1)
                        {
                            player.inventory.AddItem(new ItemPile(new RawMaterial(mat->GetMaterial(),(ITEM_SIZE_CLASS)(mat->GetSize()+1)),conv));
                        }
                        if (rem>0)
                            pp->AddItems(rem-pp->GetCount());
                        else
                        {
                            player.inventory.RemoveItem(CurrentItem);
                            delete pp;
                        }
                        player.inventory.StackAll();
                        player.inventory.UnstackSingles();
                    }

                }
            }
    }
    break;
    case TOOLTYPES::TYPE_BUILD:
    {
        Material *curmat=NULL;
        IItem *curitem;


        TCOD_key_t a;
        bool run=true;
        bool first=true;
        ISP_glow *cglow=NULL;
        while (run)
        {
            a= main->checkForKeypress();//pakeist kaipnors...


            if (a.c=='u')
            {
                curitem=player.inventory.GetItem(CurrentItem);
                if (curitem)
                {
                    if (curitem->GetType()==ITEM_TYPES::TYPE_RAWMAT)
                    {
                        curmat=curitem->GetMaterial();
                        player.inventory.RemoveItem(CurrentItem);
                    }
                    else
                    {
                        if (curitem->GetType()==ITEM_TYPES::TYPE_PILE)
                        {
                            ItemPile *d=dynamic_cast<ItemPile*>(curitem);
                            if (d->GetBaseItem()->GetType()==ITEM_TYPES::TYPE_RAWMAT)
                            {
                                curmat=d->GetMaterial();
                                d->RemoveItems(1);
                            }
                        }
                    }
                }
                if (curmat)
                {
                    IEntity *bb=new clBuilding(BTManager::Get().GetTemplate(curbuild),curmat);
                    t->AddEntity(bb);
                    bb->SetTile(t);
                    player.inventory.UnstackSingles();
                }
                else return;
                run=false;
            }
            SIDES choice;
            if (curbuild>=BID_LAST)curbuild=(BuildingID)0;
            if (curbuild<0)curbuild=(BuildingID)(BID_LAST-1);
            if (first)
            {
                first=false;
                IEntity *bb=new clBuilding(BTManager::Get().GetTemplate(curbuild),NULL);
                bb->SetTile(t);
                int pic=bb->GetPic();
                if (!cglow)
                {
                    cglow=new ISP_glow(t,pic,1);
                    t->AddEntity(cglow);
                }
            }
            if (ControlToSide(a.c,choice))
            {
                if (choice==E_W)
                    curbuild=(BuildingID)((int)curbuild-1);
                else if (choice==E_E)
                    curbuild=(BuildingID)((int)curbuild+1);
                else
                    run=false;

                IEntity *bb=new clBuilding(BTManager::Get().GetTemplate(curbuild),NULL);
                bb->SetTile(t);
                int pic=bb->GetPic();
                if (!cglow)
                {
                    cglow=new ISP_glow(t,pic,1);
                    t->AddEntity(cglow);
                }
                else
                    cglow->SetPic(pic);

                bb->SetTile(NULL);
                delete bb;

            }
            GameTick();
            main->flush();
        }

    }
    break;
    }
#endif
}
Tile *Engine::TryMove(Tile *from, Tile *to)
{
    //const int PlayerH=3;

    if (!to) return from;
    int gh=to->GetSolidH();
    //std::cout<<"To:"<<gh<<" and from: "<<from->GetHeight()<<'\n';
    /*if (to->doors==DOORSCLOSED)
    {
        to->doors=DOORSOPEN;
        return from;
    }*/
    if (gh<0)
        if (to->exits[E_DOWN])
            if (to->exits[E_DOWN]->GetSolidH()<=5)
                return to->exits[E_DOWN];
            else
            {
                // std::cout<<"To by E_DOWN\n";


            }
        else
            return from;
    if (to->PassibleSolid())
    {
        if (gh-from->GetSolidH()<=H_HUMAN)
        {
            //std::cout<<"To by Diff:"<<gh<<" and "<<from->GetHeight()<<'\n';
            return to;

        }
    }
    else
    {
        if (gh-from->GetSolidH()<=H_HUMAN)
            if (gh>5)
            {
                if ((to->exits[E_UP])&&(to->exits[E_UP]->GetSolidH()<0))
                    return to->exits[E_UP];
                else
                    return from;
            }
        clBuilding *a=GetBuildingOfType(to,BID_DOOR);
        if (a)
            a->GetData()=1-a->GetData();

    }
    return from;
}
