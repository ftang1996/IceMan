#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameConstants.h"
#include "GameWorld.h"
#include "GraphObject.h"
#include <string>
#include <vector>

const int ICE_GRID_WIDTH = 64;
const int ICE_GRID_HEIGHT = 60;

class Actor;
class Person;
class Iceman;
class Ice;
class Protester;

class StudentWorld : public GameWorld
{
public:
    struct Point    // To store coordinates for exit path
    {
        Point(int x, int y)
        {
            m_x = x;
            m_y = y;
        }
        int m_x, m_y;
    };
    enum ActorType {iceman, protester, hardcore,
        boulder, gold, barrel, sonar, water, squirt};
    StudentWorld(std::string assetDir);
    virtual ~StudentWorld();
    
    Iceman* getIceman();
    std::vector<Actor*> getActors();
    int getBarrels() const;
    int getProtesters() const;
    bool isBoundary(ActorType type, int x, int y) const;
    bool isBoulder(Actor* a, int x, int y) const;
    bool isIce(int x, int y) const;
    bool isIceGrid(int x, int y) const;
    
    void setDisplayText();
    bool clearIce(int x, int y);
    double distance(int x1, int y1, int x2, int y2) const;
    bool euclidian6(int x, int y) const;
    bool wiRadIceman(Actor* a, double radius) const;
    bool isFacingIceman(Actor* a) const;
    GraphObject::Direction lineIceman(Actor* a) const;
    bool wiRadProtester(Actor *a, Actor* protester, double radius) const;
    bool isFacingProtester(Actor* protester, Actor* a) const;
    int chanceOfGoodie(int chance);
    int chanceOfProtester(int chance);
    void insertRandom(int amt, ActorType type);
    void addActor(Actor* add);
    void addObjIceman(ActorType type);
    void destroyAll();
    std::vector<Point> pathToExit(Protester* pro, int inX, int inY);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
private:
    Iceman* m_iceman;
    Ice* m_ice[VIEW_WIDTH][VIEW_HEIGHT];
    std::vector<Actor*> m_actors;
    int m_ticks;
};

#endif // STUDENTWORLD_H_
