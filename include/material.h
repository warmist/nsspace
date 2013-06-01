#ifndef MATERIAL_H_INCLUDED
#define MATERIAL_H_INCLUDED
#include "libtcod.h"
#include <string>
#include "random.h"
#include <map>
#include <vector>
#include <math.h>
enum MATFLAGS
{
    MAT_FLAMABLE=1,MAT_IGNITES=2,MAT_ORGANIC=4,MAT_RARE=8,MAT_LONGBURN=16,MAT_EXPLODE=32,MAT_LIVELY=64
};
enum MATTYPE
{
    MATT_ALL,MATT_STONE,MATT_METAL,MATT_LIQUID,MATT_WATER,MATT_OTHER,MATT_BLOOD,MATT_FLESH
};
namespace MATTEMPLATE
{
enum T
{
    MAT_T_STONE,MAT_T_METAL,MAT_T_SOIL,MAT_T_FLESH,MAT_T_WOOD,MAT_T_SKIN,MAT_T_LEATHER
};
};
struct Material
{
    //int pic;
    TCODColor color;
    int MeltTemp;
    int BoilTemp;
    float ResistTemp;
    int IgniteTemp;
    int BurnTemp;
    int value;
    std::string name;
    int flags;
    int ID;
    MATTYPE mytype;
    void Save(TCODZip &file);
    void Load(TCODZip &file);
    /*int c;
    int density;*/
};
//Perdaryt i tokia sistema:
/*
    struct MatReal
    {
        Material *info;  // turetu turet:c
        int energy;
        int temp;
        int form;// 0-solid,1-liquid,2-gas,3-powder
        MatForm Getform();
        bool AddEnergy(int Q); // cmDT, jei neuzhtenka pakeist 1 tile'ui grazint false

    }
*/
//
typedef std::vector <Material *> materialVec;
class MaterialTemplate
{
    public:
        MaterialTemplate(std::string name);
        ~MaterialTemplate();

        void MeltTemp(range temp);
        void BoilTemp(range temp);
        void ResistTemp(range temp);
        void IgniteTemp(range temp);
        void BurnTemp(range temp);
        //void value(range value); <- pagal naudinguma...
        void Flags(int setflags);//<- kazkaip padaryt kad butu random chance...
        void HSV(range h,range s,range v);//h:0-360,s ir v: 0-255
        Material *GenMaterial(std::string name,TCODRandom *gen);
    private:
        range mtemp,boiltemp,rtemp,itemp,burntemp,ch,cs,cv;
        int flags;

        std::string prefix;
};
//material templates??
//pvz: metal-> high melttemp,low resisttemp,no ignite and burn...
typedef std::map <MATTYPE,materialVec> MatMap;
class MaterialManager
{
    public:
        static MaterialManager &Get();
        void AddMaterial(Material *toadd);
        void Save(TCODZip &file);
        void Load(TCODZip &file);
        Material *GetMaterial(MATTYPE t=MATT_ALL,int idx=0);
        Material *GetRandom(TCODRandom *ran,MATTYPE t=MATT_ALL);
        unsigned GetCount(MATTYPE t=MATT_ALL);
        void InitStd();
        int GetID(){return GetCount();};
    private:
        MaterialManager();
        //materialVec AllMats;
        MatMap Materials;
};
#endif // MATERIAL_H_INCLUDED
