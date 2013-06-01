#include "IItem.h"
int SizeConvert(ITEM_SIZE_CLASS from, ITEM_SIZE_CLASS to,int &count)
{
    int ret=(Size_Rel[to]*count)/Size_Rel[from];
    count=((Size_Rel[to]*count)%Size_Rel[from])/Size_Rel[to];
    return ret;
}
IItem::IItem(Material *mat,int pic,ITEM_SIZE_CLASS size):mat(mat),pic(pic),mysize(size)
{
    temp=21;
    //ctor
}

IItem::~IItem()
{
    //dtor
}
bool IItem::IsSame(IItem &a,IItem &b)
{

    if (
        (a.GetMaterial()==b.GetMaterial())&&
        (a.GetType()==b.GetType())&&
        (a.GetName()==b.GetName())&&
        (a.GetPic()==b.GetPic())&&
        (a.GetSize()==b.GetSize())
    )
        return true;
    return false;

}
ItemPile *IItem::PileUp(IItemVec &vec)
{
    int count=0;
    unsigned thispos=-1;

    for (unsigned i=0;i<vec.size();i++)
    {
        if (vec[i])
            if (vec[i]==this)
            {
                thispos=i;
                count++;
            }
            else

                if (*vec[i]==*this)
                {
                    count++;
                    delete vec[i];
                    vec[i]=NULL;
                }
    }
    if (count==1)
        return NULL;
    if (count==0)
        throw "Error: Item not found in stackpile";
    ItemPile *ret;
    if (count>1)
    {
        vec[thispos]=NULL;
        ret=new ItemPile(this,count);
        unsigned bb=0;
        while (bb<vec.size())
        {
            if (vec[bb]==NULL)
            {
                vec.erase(vec.begin()+bb);
            }
            else
                bb++;
        }
        return ret;
    }
    return NULL;
}
RawMaterial::RawMaterial(Material *mat,ITEM_SIZE_CLASS size):IItem(mat,'.',size)
{
    pic=ITEM_PICS[size];
}
void RawMaterial::SetSize(ITEM_SIZE_CLASS s)
{
    IItem::SetSize(s);
    pic=ITEM_PICS[s];
}
void RawMaterial::Save(TCODZip &file)
{
    file.putInt(ITEM_TYPES::TYPE_RAWMAT);
    file.putInt(mat->ID);
    file.putInt(mysize);
}
void RawMaterial::Load(TCODZip &file)
{
    int matid=file.getInt();
    mysize=(ITEM_SIZE_CLASS)file.getInt();
    pic=ITEM_PICS[mysize];
    mat=MaterialManager::Get().GetMaterial(MATT_ALL,matid);
    if (mat==NULL)
        throw "Error loading item, no mat found";
}

/*ItemPile *RawMaterial::PileUp(IItemVec &vec)
{
    int count=0;
    unsigned thispos=-1;

    for (unsigned i=0;i<vec.size();i++)
    {
        if (vec[i])
            if (vec[i]==this)
            {
                thispos=i;
                count++;
            }
            else

                if (*vec[i]==*this)
                {
                    count++;
                    delete vec[i];
                    vec[i]=NULL;
                }
    }
    if (count==1)
        return NULL;
    if (count==0)
        throw "Error: Item not found in stackpile";
    ItemPile *ret;
    if (count>1)
    {
        vec[thispos]=NULL;
        ret=new ItemPile(this,count);
        unsigned bb=0;
        while (bb<vec.size())
        {
            if (vec[bb]==NULL)
            {
                vec.erase(vec.begin()+bb);
            }
            else
                bb++;
        }
        return ret;
    }
    return NULL;
}*/
RawMaterial::~RawMaterial()
{

}
std::string RawMaterial::GetName()
{
    if ((mat->MeltTemp<temp)&&(mat->BoilTemp>temp))
    {
        return "Puddle of "+mat->name;
    }
    if (mat->BoilTemp<temp)
    {
        return "Gas of "+mat->name;// neturetu taip but...
    }
    return mat->name;
}
ItemPile::ItemPile(IItem *first,int count):base(first),count(count),IItem(first->GetMaterial(),first->GetPic(),first->GetSize())
{

}
ItemPile::~ItemPile()
{
    if (base)
        delete base;
}
void ItemPile::Save(TCODZip &file)
{
    file.putInt(ITEM_TYPES::TYPE_PILE);
    file.putInt(count);
    base->Save(file);
}
void ItemPile::Load(TCODZip &file)
{
    count=file.getInt();
    base=ItemManager::Get().Load(file);
    mat=base->GetMaterial();
    pic=base->GetPic();
    mysize=base->GetSize();
}
ItemPile *ItemPile::PileUp(IItemVec &vec)
{
    int c=0;
    unsigned thispos=-1;

    for (unsigned i=0;i<vec.size();i++)
    {
        if (vec[i])
        {
            if (vec[i]==this)
            {
                thispos=i;
                c+=count;
            }
            else
            {
                if (vec[i]->GetType()==ITEM_TYPES::TYPE_PILE)
                {
                    ItemPile *trg=dynamic_cast<ItemPile*>(vec[i]);
                    if (*(trg->GetBaseItem())==*base)
                    {
                        c+=trg->GetCount();
                        delete vec[i];
                        vec[i]=NULL;
                    }
                }
                else
                    if (*vec[i]==*base)
                    {

                        c++;
                        delete vec[i];
                        vec[i]=NULL;
                    }
            }
        }
    }
    if (c==count)
        return NULL;
    //    vec[first]=this;
    if (c<count)
        throw "Error:Pile not found in stack";
    //ItemPile *ret;
    if (c>1)
    {
        //ret=new ItemPile(this,c);
        vec[thispos]=NULL;
        count=c;
        unsigned bb=0;
        while (bb<vec.size())
        {
            if (vec[bb]==NULL)
            {
                vec.erase(vec.begin()+bb);
            }
            else
                bb++;
        }
        return this;
    }
    return NULL;
}

std::string ItemPile::GetName()
{
    std::stringstream a;
    a<<base->GetName();
    a<<"(";
    a<<count;
    a<<")";
    return a.str();
}
void ItemStack::AddItem(IItem *t)
{
    items.push_back(t);
}
int ItemStack::GetCount()
{
    return items.size();
}
IItem *ItemStack::GetItem(int n)
{
    if (n>GetCount()) return NULL;
    return items[n];
}
void ItemStack::RemoveItem(int n)
{
    if (items[n]==bigest)
        bigestvalid=false;
    items.erase(items.begin()+n);

}
int ItemStack::GetBiggest()
{
    int size=0;
    int sizeind=0;
    for (unsigned i=0;i<items.size();i++)
        if (items[i]->GetSize()>size)
        {
            size=items[i]->GetSize();
            sizeind=i;
        }
    return sizeind;
}
IItem *ItemStack::GetBiggestItem()
{
    if (bigestvalid)
        return bigest;
    else
    {
        bigest=GetBiggestItemReal();
        bigestvalid=true;
    }
}
IItem *ItemStack::GetBiggestItemReal()
{
    return items[GetBiggest()];
}
/*int ItemStack::GetPic()
{

}
TCODColor GetColor()
{

}*/
int ItemStack::CountBySize(ITEM_SIZE_CLASS size)
{
    int count=0;
    for (unsigned i=0;i<items.size();i++)
        if (items[i]->GetSize()==size)
            count++;
    return count;
}
int ItemStack::CountByType(ITEM_TYPES::ITEM_TYPE type)
{
    int count=0;
    for (unsigned i=0;i<items.size();i++)
        if (items[i]->GetType()==type)
            count++;
    return count;
}
void ItemStack::StackAll()
{
    unsigned bb=0;
    while (bb<items.size())
    {
        ItemPile *t;
        t=items[bb]->PileUp(items);
        if (t)
        {
            items.push_back(t);
            //bb++;
        }
        else
            bb++;
    }

}
void ItemStack::UnstackSingles()
{
    for (unsigned i=0;i<items.size();i++)
    {
        if (items[i]->GetType()==ITEM_TYPES::TYPE_PILE)
        {
            ItemPile *p=dynamic_cast<ItemPile*>(items[i]);
            if (p->GetCount()==1)
            {

                items[i]=p->GetBaseItem();
                p->NullBase();
                delete p;
            }
        }
    }
}
void ItemStack::SetTemp(int t)
{
    for (unsigned i=0;i<items.size();i++)
        items[i]->SetTemp(t);

}
Material *ItemStack::MakesRaw()
{
    StackAll();
    Material *ret=NULL;
    for (unsigned i=0;i<items.size();i++)
        if (items[i]->GetType()==ITEM_TYPES::TYPE_PILE)
        {
            ItemPile *t=dynamic_cast<ItemPile*>(items[i]);
            if (t->GetBaseItem()->GetType()==ITEM_TYPES::TYPE_RAWMAT)
            {
                int lcc=t->GetCount();
                int cc=lcc;
                int countmade=SizeConvert(t->GetBaseItem()->GetSize(),SIZE_HUMAN,cc);
                if (countmade==1)
                {
                    //std::cout<<cc<<std::endl;

                    ret=t->GetMaterial();
                    if (cc==0)
                    {
                        delete items[i];
                        items[i]=NULL;
                    }
                    else
                    {
                        t->RemoveItems(lcc-cc);
                    }
                    break;
                }
                if (countmade>1)
                {
                    //t->RemoveItems(lcc-cc);
                    ret=t->GetMaterial();
                    cc=1;
                    t->RemoveItems(SizeConvert(SIZE_HUMAN,t->GetBaseItem()->GetSize(),cc));

                }
            }
        }
    unsigned bb=0;
    while (bb<items.size())
    {
        if (items[bb]==NULL)
        {
            items.erase(items.begin()+bb);
        }
        else
            bb++;
    }
    return ret;
}
void ItemStack::Save(TCODZip &file)
{
    file.putInt(GetCount());
    for (unsigned i=0;i<GetCount();i++)
        items[i]->Save(file);

}
void ItemStack::Load(TCODZip &file)
{
    int cc=file.getInt();
    for (unsigned i=0;i<cc;i++)
        items.push_back(ItemManager::Get().Load(file));
}
bool ItemStack::BrakeAll()
{
    bool ret=false;
    for (unsigned i=0;i<items.size();i++)
        if (items[i]->GetType()!=ITEM_TYPES::TYPE_RAWMAT)
            if (items[i]->IsBroken())
            {
                Material *mat=items[i]->GetMaterial();

                if (items[i]->GetType()==ITEM_TYPES::TYPE_PILE)
                {
                    ItemPile *p=dynamic_cast<ItemPile*>(items[i]);
                    if(p->GetBaseItem()->GetType()!=ITEM_TYPES::TYPE_RAWMAT)
                    {
                        ITEM_SIZE_CLASS s=p->GetBaseItem()->GetSize();
                        int c=p->GetCount();
                        delete items[i];
                        items[i]=new ItemPile( new RawMaterial(mat,s),c);
                        ret=true;
                    }
                }
                else
                {
                    ITEM_SIZE_CLASS s=items[i]->GetSize();
                    items[i]=new RawMaterial(mat,s);
                    ret=true;
                }
            }
    return ret;
}
ItemTool::ItemTool(Material *mat,TOOLTYPES::T type):IItem(mat,TOOLTYPES::pics[type],SIZE_CAT)
{
    mytype=type;

}
ItemTool::~ItemTool()
{
}
std::string ItemTool::GetName()
{
    std::string name;
    name=mat->name;
    name+=" ";
    switch (mytype)
    {
    case TOOLTYPES::TYPE_PICK:
        name+="Pick";
        break;
    case TOOLTYPES::TYPE_CHISEL:
        name+="Chisel";
        break;
    case TOOLTYPES::TYPE_BUILD:
        name+="Building Tools";
        break;
    }
    return name;
}
void ItemTool::Save(TCODZip &file)
{
    file.putInt(ITEM_TYPES::TYPE_TOOL);
    file.putInt(mat->ID);
    file.putInt(mytype);
}
void ItemTool::Load(TCODZip &file)
{
    int matid=file.getInt();
    mat=MaterialManager::Get().GetMaterial(MATT_ALL,matid);
    if (mat==NULL)
        throw "Error loading item, no mat found";
    mytype=(TOOLTYPES::T)file.getInt();
    pic=TOOLTYPES::pics[mytype];
}
ItemManager::~ItemManager()
{

}
ItemManager::ItemManager()
{
}
ItemManager &ItemManager::Get()
{
    static ItemManager me;
    return me;
}
IItem *ItemManager::Load(TCODZip &file)
{
    ITEM_TYPES::ITEM_TYPE type=(ITEM_TYPES::ITEM_TYPE)file.getInt();
    switch (type)
    {
    case ITEM_TYPES::TYPE_PILE:
    {
        ItemPile *pp=new ItemPile();
        pp->Load(file);
        return pp;
    }
    break;
    case ITEM_TYPES::TYPE_RAWMAT:
    {
        RawMaterial *pp=new RawMaterial();
        pp->Load(file);
        return pp;
    }
    break;
    case ITEM_TYPES::TYPE_TOOL:
    {
        ItemTool *pp=new ItemTool();
        pp->Load(file);
        return pp;
    }
    break;

    }
}
IItem *ItemManager::Create(ITEM_TYPES::ITEM_TYPE type)
{
    switch (type)
    {
    case ITEM_TYPES::TYPE_PILE:
    {
        TCODRandom rr;
        ITEM_TYPES::ITEM_TYPE tt2=(ITEM_TYPES::ITEM_TYPE)rr.getInt(0,ITEM_TYPES::TYPE_LAST-1);

        while (tt2==ITEM_TYPES::TYPE_PILE)
            tt2=(ITEM_TYPES::ITEM_TYPE)rr.getInt(0,ITEM_TYPES::TYPE_LAST-1);
        ItemPile *pp=new ItemPile(Create(tt2),rr.getInt(2,100));
        return pp;
    }
    break;
    case ITEM_TYPES::TYPE_RAWMAT:
    {
        TCODRandom rr;
        RawMaterial *pp=new RawMaterial(MaterialManager::Get().GetRandom(&rr,MATT_ALL),(ITEM_SIZE_CLASS)rr.getInt(0,SIZE_LAST-1));
        return pp;
    }
    break;
    case ITEM_TYPES::TYPE_TOOL:
    {
        TCODRandom rr;
        ItemTool *pp=new ItemTool(MaterialManager::Get().GetRandom(&rr,MATT_ALL),(TOOLTYPES::T)rr.getInt(0,TOOLTYPES::TYPE_LAST-1));
        return pp;
    }
    break;
    }
}

