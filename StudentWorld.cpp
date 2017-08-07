#include "StudentWorld.h"
#include <string>
#include "Actor.h"
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}



StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir)
{
    m_iceman = nullptr;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    // add iceman
    m_iceman = new Iceman(this);
    
    // add ice
    for (int r = 0; r < 60; r++)
    {
        for (int c = 0; c < 64; c++)
        {
            m_ice[r][c] = new Ice(this, c, r);
        }
    }
    
    for (int r = 4; r < 60; r++)       // remove center tunnel
        for (int c = 30; c < 34; c++)
        {
            delete m_ice[r][c];
            m_ice[r][c] = nullptr;
        }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    //decLives();
    return GWSTATUS_CONTINUE_GAME;  //GWSTATUS_PLAYER_DIED;
}


