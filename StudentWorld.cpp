#include "StudentWorld.h"
#include <string>
#include "Actor.h"

using namespace std;




GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir) {}

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
void StudentWorld::cleanUp()
{
    delete m_iceman;
    m_iceman = nullptr;
    for (int r = 0; r < ICE_GRID_HEIGHT; r++)
        for (int c = 0; c < ICE_GRID_WIDTH; c++)
        {
            delete m_ice[r][c];
            m_ice[r][c] = nullptr;
        }
}

// Function returns true if trying to move past boundaries
bool StudentWorld::isBoundary(int x, int y, double size)
{
    // right boundary needs to acct for image size
    if (x < 0 || x > ICE_GRID_WIDTH - (size*4) || y < 0 || y > ICE_GRID_HEIGHT)
        return true;
    return false;
}

// Function returns true if ice exists at x,y
bool StudentWorld::isIce(int x, int y)
{
    if (m_ice[y][x] == nullptr)
        return false;
    return true;
}

// Removes ice if when Iceman digs
void StudentWorld::digIce(int x, int y)
{
    if (isIce(x, y))
    {
        delete m_ice[y][x];
        m_ice[y][x] = nullptr;
        playSound(SOUND_DIG);
    }

}






