#ifndef IITEM_H
#define IITEM_H
#include "libtcod.hpp"
#include "material.h"
#include "tooltypes.h"
#include <vector>
#include <string>
#include <sstream>
//#include <iostream>
enum ITEM_SIZE_CLASS
{
    SIZE_COIN,SIZE_APPLE,SIZE_CAT,SIZE_BACKPACK,SIZE_HUMAN,SIZE_FULLTILE,SIZE_LAST
};
const char ITEM_PICS[]={',','o','c','%','@','#'};
const int Size_Rel[]={12000,3000,120,60,6,1};// vienas humanas ~=20000 coinu
int SizeConvert(ITEM_SIZE_CLASS from, ITEM_SIZE_CLASS to,int &count);
namespace ITEM_TYPES
{
enum ITEM_TYPE
{
    TYPE_RAWMAT,TYPE_PILE,TYPE_TOOL,TYPE_LAST
};
}
class ItemPile;
class IItem;
typedef std::vector<IItem*> IItemVec;
class IItem
{
public:
    IItem(Material *mat,int pic,ITEM_SIZE_CLASS size);
    IItem(){};
    virtual std::string GetName()=0;
    virtual ~IItem();
    virtual int GetPic()
    {return pic;};
    virtual TCODColor GetColor()
    {return mat->color;};
    ITEM_SIZE_CLASS GetSize()
    {return mysize;};
    virtual void SetSize(ITEM_SIZE_CLASS s)
    {mysize=s;};
    Material *GetMaterial()
    {return mat;};
    void SetMaterial(Material *s)
    {mat=s;};
    virtual int GetValue()
    {return mat->value;};
    virtual int GetTemp()
    {return temp;};
    virtual void SetTemp(int t)
    {temp=t;};
    virtual ITEM_TYPES::ITEM_TYPE GetType()=0;
    virtual bool IsSame(IItem &a,IItem &b);
    virtual bool operator ==(IItem &a){return IsSame(*this,a);};
    virtual ItemPile *PileUp(IItemVec &vec);
    virtual void Save(TCODZip &file)=0;
    virtual void Load(TCODZip &file)=0;
    virtual bool IsBroken(){return mat->MeltTemp<temp;};
protected:
    int pic,temp;
    Material *mat;
    ITEM_SIZE_CLASS mysize;
private:

};

class ItemPile:public IItem
{
    public:
        ItemPile(IItem *first,int count);
        ItemPile(){};
        ~ItemPile();
        void AddItems(int n=1){count+=n;};
        void RemoveItems(int n=1){count-=n;};
        int GetCount(){return count;};
        std::string GetName();
        IItem *GetBaseItem(){return base;};
        void SetMaterial(Material *s)
        {
            IItem::SetMaterial(s);
            base->SetMaterial(s);
        }
        void SetTemp(int t)
        {
            IItem::SetTemp(t);
            base->SetTemp(t);
        }
        ITEM_TYPES::ITEM_TYPE GetType(){return ITEM_TYPES::TYPE_PILE;};
        bool operator ==(ItemPile &a)
        {
            return (IItem::IsSame(*this,a)&&(GetCount()==a.GetCount()));
        }
        ItemPile *PileUp(IItemVec &vec);
        void Save(TCODZip &file);
        void Load(TCODZip &file);
        void NullBase(){base=NULL;};
        //void BrakeAll();
        //Material *ExtractMat();
    private:
        int count;
        IItem *base;

};

class RawMaterial:public IItem
{
    public:
        RawMaterial(Material *mat,ITEM_SIZE_CLASS size);
        RawMaterial(){};
        ~RawMaterial();
        std::string GetName();
        void SetSize(ITEM_SIZE_CLASS s);
        ITEM_TYPES::ITEM_TYPE GetType(){return ITEM_TYPES::TYPE_RAWMAT;};
        //ItemPile *PileUp(IItemVec &vec);
        void Save(TCODZip &file);
        void Load(TCODZip &file);
    private:

};

class ItemTool:public IItem
{
    public:
        ItemTool(Material *mat,TOOLTYPES::T type);
        ItemTool(){};
        ~ItemTool();

        std::string GetName();
        void Save(TCODZip &file);
        void Load(TCODZip &file);
        ITEM_TYPES::ITEM_TYPE GetType(){return ITEM_TYPES::TYPE_TOOL;};
        TOOLTYPES::T GetToolType(){return mytype;};
        ItemPile *PileUp(IItemVec &vec){return NULL;}; //doesnt pile up...
        bool IsSame(ItemTool &a,ItemTool &b){return IItem::IsSame(a,b)&&(a.GetToolType()==b.GetToolType());};
    private:
        TOOLTYPES::T mytype;
};

class ItemStack
{
public:
    ItemStack() {bigestvalid=false;};
    ~ItemStack() {};
    void AddItem(IItem *t);
    int GetCount();
    IItem *GetItem(int n);
    //IItem *operator[](int n);
    void RemoveItem(int n);
    //void RemoveItem(IItem *t);


    IItem *GetBiggestItem();

    /*int GetPic();
    TCODColor GetColor();*/

    int CountBySize(ITEM_SIZE_CLASS size);
    int CountByType(ITEM_TYPES::ITEM_TYPE type);

    Material *MakesRaw();

    void StackAll();
    void UnstackSingles();

    void Save(TCODZip &file);
    void Load(TCODZip &file);
    void SetTemp(int t);
    bool BrakeAll();
private:
    int GetBiggest();
    IItem *GetBiggestItemReal();
    IItemVec items;
    IItem *bigest;
    bool bigestvalid;
};
class ItemManager
{
    public:
        ~ItemManager();
        static ItemManager &Get();
        IItem *Load(TCODZip &file);
        IItem *Create(ITEM_TYPES::ITEM_TYPE type);
    private:
        ItemManager();
};
#endif // IITEM_H
