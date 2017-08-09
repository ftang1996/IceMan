#include "StudentWorld.h"
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <math.h>

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
    // Add iceman
    m_iceman = new Iceman(this);
    
    // Add ice
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
    
    // Add all other actors
    int lvl = getLevel();
    int B = min(lvl/2 + 2, 9);  // boulders
    int G = max(5 - lvl/2, 2);  // gold
    int L = min(2 + lvl, 21);   // barrels
    
    insertRandom(G, gold);
    
    
    
    
    
    
    
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    setDisplayText();
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
            clearIce(c, r);
}

void StudentWorld::setDisplayText()
{
    // Get values
    int level = getLevel();
    int lives = getLives();
    int health = (m_iceman->getHP()/10)*100;
    int squirts = m_iceman->getSquirts();
    int gold = m_iceman->getGold();
    // TODO int barrelsLeft = ...
    int sonar = m_iceman->getCharge();
    int score = getScore();
    
    // Format string
    ostringstream ss;
    ss << "Lvl: " << setw(2) << setfill(' ') << level << "  ";
    ss << "Lives: " << lives << "  ";
    ss << "Hlth: " << setw(3) << setfill(' ') << health << "%  ";
    ss << "Wtr: " << setw(2) << setfill(' ') << squirts << "  ";
    ss << "Gld: " << setw(2) << setfill(' ') << gold << "  ";
    // TODO: add oil
    ss << "Sonar: " << setw(2) << setfill(' ') << sonar << "  ";
    ss << "Scr: " << setw(6) << setfill('0') << score;
    
    string str(ss.str());
    setGameStatText(str);
}

// Insert object at random location
void StudentWorld::insertRandom(int amt, ActorType type)
{
    int i = 0;
    while (i < amt)
    {
        // Generate random coordinates
        int x = rand() % (ICE_GRID_WIDTH - SPRITE_WIDTH + 1);
        int y = rand() % (ICE_GRID_HEIGHT - SPRITE_WIDTH + 1);
        
        // Generate item if coordinates meet distance requirement
        if (okRadius(x, y))
        {
            switch(type)
            {
                case boulder:
                    
                case gold:
                    m_objects.push_back(new Gold(this, x, y));
                    break;
                case barrel:
            
                case sonar:
                    break;
            }
            
            i++;
        }
    }
}

// Function returns true if ice exists at x,y
bool StudentWorld::isIce(int x, int y)
{
    if (m_ice[y][x] == nullptr)
        return false;
    return true;
}

// Normally delete ice
void StudentWorld::clearIce(int x, int y)
{
    if (isIce(x, y))
    {
        delete m_ice[y][x];
        m_ice[y][x] = nullptr;
    }
}

// Removes ice when Iceman digs
void StudentWorld::digIce(int x, int y)
{
    if (isIce(x, y))
    {
        delete m_ice[y][x];
        m_ice[y][x] = nullptr;
        playSound(SOUND_DIG);
    }
}

//// Function returns true if trying to move past boundaries
//bool StudentWorld::isBoundary(int x, int y)
//{
//    // right boundary needs to acct for image size
//    if (x < 0 || x > ICE_GRID_WIDTH - SPRITE_WIDTH || y < 0 || y > ICE_GRID_HEIGHT)
//        return true;
//    return false;
//}

bool StudentWorld::isTunnel(int x, int y)
// Checks that new object placement meets distance requirement
bool StudentWorld::okRadius(int x, int y)
{
    int existX, existY;
    for (int i =0; i < m_objects.size(); i++)
    {
        existX = m_objects[i]->getX();
        existY = m_objects[i]->getY();
        float rad = sqrt(pow((x - existX),2) + pow((y-existY),2));
        if (rad < MIN_RADIUS)
            return false;
    }
    return true;
}


//bool StudentWorld::hitObject(int perX, int perY)
//{
//    
//}




