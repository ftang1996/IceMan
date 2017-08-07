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
    for (int r = 0; r < ICE_GRID_HEIGHT; r++)
    {
        for (int c = 0; c < ICE_GRID_WIDTH; c++)
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
    m_iceman->doSomething();
    
    
    return GWSTATUS_CONTINUE_GAME;
    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    // decLives();
    // return GWSTATUS_PLAYER_DIED;
}

// Function returns true if trying to move past boundaries
bool StudentWorld::isBoundary(int x, int y)
{
    if (x < 0 || x > ICE_GRID_WIDTH || y < 0 || y > ICE_GRID_HEIGHT)
        return true;
    return false;
}

// Function returns true if ice exists at x,y
bool StudentWorld::isIce(int x, int y)
{
    if (!isBoundary(x,y) && m_ice[y][x] == nullptr)
        return false;
    return true;
}

void StudentWorld::removeIce(int x, int y, string dir)
{
    if (dir == "vertical")
    {
        for (int c = x; c < x+4; c++)
        {
            int r = y;
            delete m_ice[r][c];
            m_ice[r][c] = nullptr;
        }
    }
    else if (dir == "horizontal")
    {
        for (int r = y; r < y+4; r++)
        {
            int c = x;
            delete m_ice[r][c];
            m_ice[r][c] = nullptr;
        }
    }

}






