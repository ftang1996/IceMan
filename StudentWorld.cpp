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
    
    for (vector<Actor*>::iterator it = m_items.begin(); it != m_items.end(); it++)
    {
        if ((*it)->isAlive())
            (*it)->doSomething();
        if(!(*it)->isAlive())   //delete if picked up NEED TO FIX
            delete (*it);

    }
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

Iceman* StudentWorld::getIceman()
{
    return m_iceman;
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
        if (placeNewItem(x, y) && !isTunnel(x, y))
        {
            switch(type)
            {
                case boulder:
                    
                case gold:
                    m_items.push_back(new Gold(this, x, y));
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

// Returns true if location is in central tunnel
bool StudentWorld::isTunnel(int x, int y)
{
    if (x >= 4 && x < 60 && y >= 30 && y < 34)
        return true;
    return false;
}

double StudentWorld::distance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow((x1 - x2),2) + pow((y1 - y2),2));
}

// Checks if actor distance is greater than radius of world items
bool StudentWorld::placeNewItem(int x, int y)
{
    int itemX, itemY;
    for (int i = 0; i < m_items.size(); i++)
    {
        itemX = m_items[i]->getX();
        itemY = m_items[i]->getY();
        double dist = distance(x, y, itemX, itemY);
        if (dist < 6.0)
            return false;
    }
    return true;
}

// Returns true if item distance from iceman is within radius
bool StudentWorld::wiRadIceman(Actor* item, double radius)
{
    int itemX = item->getX();
    int itemY = item->getY();
    int imX = m_iceman->getX();
    int imY = m_iceman->getY();
    if (distance(itemX, itemY, imX, imY) <= radius)
        return true;
    return false;
}


//void StudentWorld::showNearbyItems(int x, int y)
//{
//    int itemX, itemY, type;
//    for (int i = 0; i < m_items.size(); i++)
//    {
//        type = m_items[i]->getID();
//        if(type == IID_GOLD || type == IID_BARREL)
//        {
//            itemX = m_items[i]->getX();
//            itemY = m_items[i]->getY();
//            if (distance(x, y, itemX, itemY) <= 4.0)
//                m_items[i]->setVisible(true);
//        }
//    }
//}

//bool StudentWorld::hitObject(int perX, int perY)
//{
//    for (int i = 0; i < m_objects.size(); i++)
//}


//bool StudentWorld::okRadius(int x, int y, double rad)
//{
//    int existX, existY;
//    for (int i = 0; i < m_objects.size(); i++)
//    {
//        existX = m_objects[i]->getX();
//        existY = m_objects[i]->getY();
//        float rad = sqrt(pow((x - existX),2) + pow((y-existY),2));
//        if (rad < MIN_RADIUS)
//            return false;
//    }
//    return true;
//}


//// Function returns true if trying to move past boundaries
//bool StudentWorld::isBoundary(int x, int y)
//{
//    // right boundary needs to acct for image size
//    if (x < 0 || x > ICE_GRID_WIDTH - SPRITE_WIDTH || y < 0 || y > ICE_GRID_HEIGHT)
//        return true;
//    return false;
//}



