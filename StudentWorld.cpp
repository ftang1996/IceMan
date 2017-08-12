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
: GameWorld(assetDir)
{
}

StudentWorld::~StudentWorld()
{
    cleanUp();           // TODO: Fix!!! cant call cleanup
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
    {
        for (int c = 30; c < 34; c++)
        {
            clearIce(c, r);
        }
    }
    
    // add all other actors
    int lvl = getLevel();
    int B = min(lvl/2 + 2, 9);  // boulders
    int G = max(5 - lvl/2, 2);  // gold
    int L = min(2 + lvl, 21);   // barrels
    
    insertRandom(B, boulder);
    insertRandom(G, gold);
    insertRandom(L, barrel);

    
    
    
    
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // iceman actions
    setDisplayText();
    m_iceman->doSomething();
    int imX = m_iceman->getX();     // dig away ice in iceman's path
    int imY = m_iceman->getY();
    bool dug = false;
    for (int iceX = imX; iceX < imX + SPRITE_WIDTH; iceX++)
    {
        for (int iceY = imY; iceY < imY + SPRITE_HEIGHT; iceY++)
            if(clearIce(iceX, iceY))
                dug = true;
    }
    if (dug)        // only play sound if ice was dug
        playSound(SOUND_DIG);
    
    // all actor actions
    vector<Actor*>::iterator it = m_actors.begin();
    while (it != m_actors.end())
    {
        if ((*it)->isAlive())
            (*it)->doSomething();
        if(!(*it)->isAlive())       // delete if dead
        {
            delete *it;
            it = m_actors.erase(it);
        }
        else
            it++;
    }
    
    //TODO: check for barrels
    
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
    
    // TODO: Delete items
}

Iceman* StudentWorld::getIceman() const
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
    int barrels = getBarrels();
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
    ss << "Oil Left" << setw(2) << setfill(' ') << barrels << " ";
    ss << "Sonar: " << setw(2) << setfill(' ') << sonar << "  ";
    ss << "Scr: " << setw(6) << setfill('0') << score;
    
    string str(ss.str());
    setGameStatText(str);
}

// insert object at random location
void StudentWorld::insertRandom(int amt, ObjType type)
{
    int i = 0;
    while (i < amt)
    {
        // generate random coordinates
        int x = rand() % (ICE_GRID_WIDTH - SPRITE_WIDTH + 1);
        int y = rand() % (ICE_GRID_HEIGHT - SPRITE_WIDTH + 1);
        
        // generate object if coordinates meet distance requirement
        if (placeNewObj(x, y) && !isTunnel(x, y))
        {
            switch(type)
            {
                case boulder:
                    m_actors.push_back(new Boulder(this, x, y));
                    // remove ice behind boulder
                    for (int r = y; r < y+SPRITE_HEIGHT; r++)
                        for (int c = x; c< x+SPRITE_WIDTH; c++)
                            clearIce(c, r);
                    break;
                case gold:
                    m_actors.push_back(new Gold(this, x, y));
                    break;
                case barrel:
                    m_actors.push_back(new Barrel(this, x, y));
                    break;
                case sonar:
                    
                    break;
                case water:
                    
                    break;
            }
            i++;
        }
    }
}

// returns true if ice exists at x,y
bool StudentWorld::isIce(int x, int y) const
{
    if (m_ice[y][x] == nullptr)
        return false;
    return true;
}

// normally delete ice
//void StudentWorld::clearIce(int x, int y)
//{
//    if (isIce(x, y))
//    {
//        delete m_ice[y][x];
//        m_ice[y][x] = nullptr;
//    }
//}

// returns true if ice is removed
bool StudentWorld::clearIce(int x, int y)
{
    if (isIce(x, y))
    {
        delete m_ice[y][x];
        m_ice[y][x] = nullptr;
        return true;
    }
    return false;
}

// checks for edges of screen
bool StudentWorld::isBoundary(int x, int y) const
{
    // right boundary needs to acct for image size
    if (x < 0 || x > ICE_GRID_WIDTH - SPRITE_WIDTH || y < 0 || y > ICE_GRID_HEIGHT)
        return true;
    return false;
}

// returns true if location is in central tunnel
bool StudentWorld::isTunnel(int x, int y) const
{
    if (x >= (30-SPRITE_WIDTH) && x < 34 && y >= (4-SPRITE_HEIGHT) && y < 60 )
        return true;
    return false;
}

bool StudentWorld::isBoulder(int x, int y) const
{
    vector<Actor*>::const_iterator it = m_actors.begin();
    while (it != m_actors.end())
    {
        if ((*it)->getID() == IID_BOULDER)
        {
            int boX = (*it)->getX();
            int boY = (*it)->getY();
            if (distance(x, y, boX, boY) < 4.0)
                return true;
//            int c = x; int r = y;
//            int boX = (*it)->getX();
//            int boY = (*it)->getY();
//            switch (dir)
//            {
//                case Actor::up:
//                    r = y+4;
//                    while (c < x+SPRITE_WIDTH)
//                    {
//                        if (r == boY && c >= boX && c < boX+SPRITE_WIDTH)
//                            return true;
//                        c++;
//                    }
//                    break;
//                case Actor::down:
//                                        
//                default:
////                    break;
////            }
//            if(wiRadIceman(*it, 4.0))
//                return true;
//            int boX = (*it)->getX();
//            int boY = (*it)->getY();

//            for(int r = y; r < y+SPRITE_HEIGHT; r++)
//            {
//                for(int c = x; r < x+SPRITE_WIDTH; x++)
//                    if (c >= boX && c < boX+SPRITE_WIDTH &&
//                        r >= boY && r < boY+SPRITE_HEIGHT)
//                        return true;
//            }
        }
        it++;
    }
    return false;
}

double StudentWorld::distance(int x1, int y1, int x2, int y2) const
{
    return sqrt(pow((x1 - x2),2) + pow((y1 - y2),2));
}

// checks if new object distance is greater than radius of existing objects
bool StudentWorld::placeNewObj(int x, int y) const
{
    int objX, objY;
    for (int i = 0; i < m_actors.size(); i++)
    {
        objX = m_actors[i]->getX();
        objY = m_actors[i]->getY();
        double dist = distance(x, y, objX, objY);
        if (dist < 6.0)
            return false;
    }
    return true;
}

// returns true if actor distance from iceman is within radius
bool StudentWorld::wiRadIceman(Actor* a, double radius) const
{
    int actX = a->getX();
    int actY = a->getY();
    int imX = m_iceman->getX();
    int imY = m_iceman->getY();
    if (distance(actX, actY, imX, imY) <= radius)
        return true;
    return false;
}

// add objects to iceman inventory
void StudentWorld::addObjIceman(ObjType type)
{
    switch (type)
    {
        case gold:
            playSound(SOUND_GOT_GOODIE);
            increaseScore(10);
            getIceman()->addGold();
            break;
        case barrel:
            playSound(SOUND_FOUND_OIL);
            increaseScore(1000);
            break;
        case sonar:
            playSound(SOUND_GOT_GOODIE);
            increaseScore(75);
            getIceman()->addCharge();
            break;
        case water:
            playSound(SOUND_GOT_GOODIE);
            increaseScore(100);
            getIceman()->addSquirts();
            break;
    }
}

// returns number of barrels left in level
int StudentWorld::getBarrels() const
{
    int barrels = 0;
    vector<Actor*>::const_iterator it = m_actors.begin();
    while (it != m_actors.end())
    {
        if ((*it)->getID() == IID_BARREL)
            barrels++;
        it++;
    }
    return barrels;
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



