#include "ISpecial.h"

ISpecial::ISpecial(ITile *t)
{
    mytile =t;
    //ctor
}

ISpecial::~ISpecial()
{
    //dtor
}
ISP_fire::ISP_fire(int l,int temp,ITile *t):ISpecial(t)
{
    life=l;
    modtemp=temp;
}
ISP_fire::~ISP_fire()
{

}
void ISP_fire::Save(TCODZip &file)
{
    file.putChar(CL_ISP_FIRE);
    file.putInt(mytile->GetSave()->num);
    file.putInt(life);
    file.putInt(modtemp);
}
void ISP_fire::Load(TCODZip &file,VecLoad &bonusdata)
{
    /*if(file.getChar()!=CL_ISP_FIRE)
        throw "Error loading file";*/
    mytile=GetFromLoad(file.getInt(),bonusdata);
    if(mytile==NULL)
        throw "Error loading file";
    life=file.getInt();
    modtemp=file.getInt();
}
TCODColor ISP_fire::GetColor()
{
    if (modtemp>0)
        return TCODColor::red;
    return TCODColor::darkBlue;
}
int ISP_fire::GetPic()
{
    return (((life%4)>2)?'*':'+');
}
void ISP_fire::Tick()
{
    ISpecial::Tick();
    if (life<=0)
        return;
    int i=rand()%E_LAST;
    ITile *trg=mytile->GetSide((SIDES)i);
    if (modtemp>0)
        if (trg)
        {
            Material *b=trg->GetTop();
            if (b)
                if (b->flags&MAT_FLAMABLE)
                    if ((!trg->GetSide(E_UP))||(trg->GetSide(E_UP)->GetHeight()<0))
                        if (trg->GetEntityCount()==0)
                        {
                            trg->RemoveTop();
                            if (b->flags&MAT_LONGBURN)
                                trg->AddEntity(new ISP_fire(100+rand()%100,b->BurnTemp,trg));//random->getInt(100,200),b->BurnTemp,cur));
                            else
                                trg->AddEntity(new ISP_fire(10+rand()%10,b->BurnTemp,trg));//random->getInt(10,20),b->BurnTemp,cur));
                        }
        }
    mytile->AddTemp(modtemp);
    life--;
}
bool ISP_fire::IsDead()
{
    return (life<=0);
}
ISP_bolt::ISP_bolt(int dir,int temp,int speed,ITile *trg):ISpecial(trg)
{
    curdir=dir;
    modtemp=temp;
    this->speed=speed;
    alive=true;
}
ISP_bolt::~ISP_bolt()
{

}
void ISP_bolt::Save(TCODZip &file)
{
    file.putChar(CL_ISP_BOLT);
    file.putInt(mytile->GetSave()->num);
    file.putInt(curdir);
    file.putInt(modtemp);
    file.putInt(speed);
}
void ISP_bolt::Load(TCODZip &file,VecLoad &bonusdata)
{
     //if(file.getChar()!=CL_ISP_BOLT)
    //    throw "Error loading file";
    mytile=GetFromLoad(file.getInt(),bonusdata);
    if(mytile==NULL)
        throw "Error loading file";
    curdir=file.getInt();
    modtemp=file.getInt();
    speed=file.getInt();
    alive=true;
}
TCODColor ISP_bolt::GetColor()
{
    return TCODColor::red;
}
int ISP_bolt::GetPic()
{
    const int arr[]={'.','\\','|','/','-','-','/','|','\\','.'};
    return arr[curdir];
}
void ISP_bolt::Tick()
{
    //ISpecial::Tick();
    if (done) return;
    done=true;
    if (!alive)return;
    /*static bool a;
    a=(a)?false:true;
    if(a) return;*/
    SIDES s=(SIDES)curdir;
    ITile *trg=mytile->GetSide(s);
    if (trg)
    {
        if (trg->GetEntityCount()==0)
        {
            if (trg->GetHeight()<5)
            {
                mytile->RemoveEntity(this);
                trg->AddEntity(this);
                mytile=trg;
                //return;
            }
            else
            {
                trg->AddTemp(modtemp);
                alive=false;
                //return;
            }
        }

        else

            if (rand()%100==0)
            {
                curdir=(int)closest((SIDES)curdir,false);
                // return;
            }
    }

    else
        alive=false;
}
bool ISP_bolt::IsDead()
{
    return !alive;
}
ISP_glow::ISP_glow(ITile *trg):ISpecial(trg)
{
    life=100;
    pic=trg->GetPic();
}
ISP_glow::ISP_glow(ITile *trg,int pic,int life):ISpecial(trg),pic(pic),life(life)
{

}
ISP_glow::~ISP_glow()
{

}
void ISP_glow::Save(TCODZip &file)
{
    file.putChar(CL_ISP_GLOW);
    file.putInt(mytile->GetSave()->num);
    file.putInt(pic);
    file.putInt(life);
}
void ISP_glow::Load(TCODZip &file,VecLoad &bonusdata)
{
     //if(file.getChar()!=CL_ISP_GLOW)
    //    throw "Error loading file";
    mytile=GetFromLoad(file.getInt(),bonusdata);
    if(mytile==NULL)
        throw "Error loading file";
    pic=file.getInt();
    life=file.getInt();
}
TCODColor ISP_glow::GetColor()
{
    return TCODColor::darkRed;

}
int ISP_glow::GetPic()
{
    return pic;//mytile->GetPic();
}
void ISP_glow::Tick()
{
    if (life<=0)
        return;
    life--;
}
bool ISP_glow::IsDead()
{
    return (life<=0);
}
