#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>

#include "GraphObject.h"

#include "Actor.h"      //TODO: Remove Later

const int ICE_GRID_WIDTH = 64;
const int ICE_GRID_HEIGHT = 60;
//const double MIN_RADIUS = 6.0;

class Actor;
class Person;
class Iceman;
class Ice;

class StudentWorld : public GameWorld
{
public:
    enum ActorType {boulder, gold, barrel, sonar};
    StudentWorld(std::string assetDir);
    virtual ~StudentWorld();
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();

    Iceman* getIceman();
    void setDisplayText();
    void insertRandom(int amt, ActorType type);
    
    bool isIce(int x, int y);
    void clearIce(int x, int y);
    bool digIce(int x, int y);
//    bool isBoundary(int x, int y);
    bool isTunnel(int x, int y);
    double distance(int x1, int y1, int x2, int y2);
    bool placeNewItem(int x, int y);
    bool wiRadIceman(Actor* item, double radius);

    
//    void showNearbyItems(int x, int y);
//    bool hitObject(int perX, int perY);
    
   
    
private:
    Iceman* m_iceman;
    Ice* m_ice[VIEW_WIDTH][VIEW_HEIGHT];
    std::vector<Actor*> m_items;
};

#endif // STUDENTWORLD_H_
