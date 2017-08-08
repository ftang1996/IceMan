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

class Person;
class Iceman;
class Ice;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    bool isBoundary(int x, int y, double size);
    bool isIce(int x, int y);
    void removeIce(int x, int y);
private:
    Iceman* m_iceman;
    Ice* m_ice[VIEW_WIDTH][VIEW_HEIGHT];
};

#endif // STUDENTWORLD_H_
