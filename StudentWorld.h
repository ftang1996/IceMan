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

class Actor;
class Person;
class Iceman;
class Ice;

class StudentWorld : public GameWorld
{
public:
    enum ActorType {iceman, protester, hardcore,
        boulder, gold, barrel, sonar, water, squirt};
    StudentWorld(std::string assetDir);
    virtual ~StudentWorld();
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();

    Iceman* getIceman();
    std::vector<Actor*> getActors();
    void setDisplayText();
    void insertRandom(int amt, ActorType type);
    
    bool isIce(int x, int y) const;
    bool isIceGrid(int x, int y);
    bool clearIce(int x, int y);
//    bool digIce(int x, int y);

    bool isBoundary(ActorType type, int x, int y) const;
    
    //bool isTunnel(int x, int y) const;
    bool isBoulder(Actor* a, int x, int y) const;
    double distance(int x1, int y1, int x2, int y2) const;
    bool euclidian6(int x, int y) const;
    bool wiRadIceman(Actor* a, double radius) const;
    bool isFacingIceman(Actor* a) const;
    void addObjIceman(ActorType type);
    int getBarrels() const;
    int getPools() const;
    int getProtesters() const;
    void addActor(Actor* add);
    

    int chanceOfGoodie(int chance);
    int chanceOfProtester(int chance);
    void destroyAll();
    
    

//    void showNearbyItems(int x, int y);
//    bool hitObject(int perX, int perY);
    
   
    
private:
    Iceman* m_iceman;
    Ice* m_ice[VIEW_WIDTH][VIEW_HEIGHT];
    std::vector<Actor*> m_actors;
    int m_ticks;
};

#endif // STUDENTWORLD_H_
