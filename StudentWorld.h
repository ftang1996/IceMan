#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>

#include "Actor.h"      //TODO: Remove Later

class Iceman;
class Ice;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    virtual ~StudentWorld();
    virtual int init();

    virtual int move();

	virtual void cleanUp()
	{
	}

private:
    Iceman* m_iceman;
    Ice* m_ice[VIEW_WIDTH][VIEW_HEIGHT];
};

#endif // STUDENTWORLD_H_
