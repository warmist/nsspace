#ifndef BUILDINGTEMPLATE_H
#define BUILDINGTEMPLATE_H
enum BuildingID
{
    BID_FLOOR,BID_WALL,BID_DOOR,BID_WINDOW,BID_GRATE,BID_LAST
};
class BuildingTemplate;
#include "clBuilding.h"
#include "ITile.h"

enum SidesOccupied
{
    SO_N=1,SO_S=2,SO_E=4,SO_W=8
};
class BuildingTemplate
{
public:
    BuildingTemplate(){};
    virtual ~BuildingTemplate(){};
    virtual void Tick(ITile *t,int &data) {};
    virtual int GetPriority(){return 500;};
    virtual BuildingID GetBuildingID()=0;
    virtual int GetPic(ITile *t,int &data)=0;
    virtual int GetFlags(int &data){return 0;};
};
class Build_Wall:public BuildingTemplate
{
 public:
    Build_Wall(){};
    ~Build_Wall(){};
    //void Tick(){};
    int GetPic(ITile *t,int &data);
    BuildingID GetBuildingID(){return BID_WALL;};
    int occupied(ITile *t);
    int GetFlags(int &data){return IENTITY_FLAGS::F_BLOCKS_ALL;};
};
class Build_Floor:public BuildingTemplate
{
public:
    Build_Floor(){};
    ~Build_Floor(){};
    int GetPic(ITile *t,int &data){return '+';};
    BuildingID GetBuildingID(){return BID_FLOOR;};
    int GetPriority(){return -1;};
    int GetFlags(int &data){return IENTITY_FLAGS::F_BLOCKS_ALL|IENTITY_FLAGS::F_FLOOR;};

};
class Build_Grate:public BuildingTemplate
{
public:
    Build_Grate(){};
    ~Build_Grate(){};
    int GetPic(ITile *t,int &data){return '#';};
    BuildingID GetBuildingID(){return BID_GRATE;};
    int GetPriority(){return -1;};
    int GetFlags(int &data){return IENTITY_FLAGS::F_BLOCKS_SOLID|IENTITY_FLAGS::F_BLOCKS_LIGHT|IENTITY_FLAGS::F_FLOOR;};

};
class Build_Window:public BuildingTemplate
{
  int GetPic(ITile *t,int &data){return '/';};
  BuildingID GetBuildingID(){return BID_WINDOW;};
  virtual int GetFlags(int &data){return IENTITY_FLAGS::F_BLOCKS_SOLID|IENTITY_FLAGS::F_BLOCKS_LIGHT;};
};
class Build_Door:public BuildingTemplate
{
public:
    Build_Door(){};
    ~Build_Door(){};
    int GetPic(ITile *t,int &data){return (data)?'|':'/';};
    BuildingID GetBuildingID(){return BID_DOOR;};
     virtual int GetFlags(int &data){return (data)?IENTITY_FLAGS::F_BLOCKS_ALL:0;};
};
class BTManager
{
public:
    ~BTManager();
    BuildingTemplate *GetTemplate(BuildingID id);
    static BTManager &Get();
private:
    BTManager();
    BuildingTemplate *arr[BID_LAST];

};
#endif // BUILDINGTEMPLATE_H
