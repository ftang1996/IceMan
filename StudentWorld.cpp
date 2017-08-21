#include "GameConstants.h"
#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <math.h>

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir)
{
    m_ticks = 0;
}

StudentWorld::~StudentWorld()
{
    destroyAll();
}

// Accessors
Iceman* StudentWorld::getIceman()
{
    return m_iceman;
}

vector<Actor*> StudentWorld::getActors()
{
    return m_actors;
}

// Returns number of barrels left in oil field
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

// Returns number of protesters in oil field
int StudentWorld::getProtesters() const
{
    int protesters = 0;
    vector<Actor*>::const_iterator it = m_actors.begin();
    while (it != m_actors.end())
    {
        if ((*it)->getID() == IID_PROTESTER ||
            (*it)->getID() == IID_HARD_CORE_PROTESTER )
            protesters++;
        it++;
    }
    return protesters;
}

// Checks for edges of ice grid
bool StudentWorld::isBoundary(ActorType type, int x, int y) const
{
    // general window boundary
    if (x < 0 || x > ICE_GRID_WIDTH-SPRITE_WIDTH || y < 0 || y > ICE_GRID_HEIGHT)
        return true;
    
    // additional boulder boundaries
    if (type == boulder)
        if (x < 0 || x > 60 - SPRITE_WIDTH || y < 20 || y > 56 - SPRITE_HEIGHT)
            return true;
    
    // additional gold and barrel boundaries
    if (type == gold || type == barrel || type == boulder)
        if (x >= (30-SPRITE_WIDTH) && x < 34 && y >= (4-SPRITE_HEIGHT) && y < 60 )
            return true;
    return false;
}

// Returns true if boulder is within distance of 3
bool StudentWorld::isBoulder(Actor* a, int x, int y) const
{
    vector<Actor*>::const_iterator it = m_actors.begin();
    while (it != m_actors.end())
    {
        if ((*it)->getID() == IID_BOULDER && (*it) != a)
        {
            int boX = (*it)->getX();
            int boY = (*it)->getY();
            if (distance(x, y, boX, boY) < 3.0)
                return true;
        }
        it++;
    }
    return false;
}

// Returns true if ice exists at point (x,y)
bool StudentWorld::isIce(int x, int y) const
{
    if (m_ice[y][x] == nullptr)
        return false;
    return true;
}

// Returns true if ice exists at 4x4 grid
bool StudentWorld::isIceGrid(int x, int y) const
{
    if (x < 0 || y < 0 || x > 60 || y > 60)
        return false;
    
    for (int r = y; r < y + 4; r++)
    {
        for (int c = x; c < x + 4; c++)
        {
            if (m_ice[r][c] != nullptr)
                return true;
        }
    }
    return false;
}

// Configure display text in game screen
void StudentWorld::setDisplayText()
{
    // get values
    int level = getLevel();
    int lives = getLives();
    double health = (m_iceman->getHP()/10)*100;
    int squirts = m_iceman->getSquirts();
    int gold = m_iceman->getGold();
    int barrels = getBarrels();
    int sonar = m_iceman->getCharge();
    int score = getScore();
    
    // format string
    ostringstream ss;
    ss << "Lvl: " << setw(2) << setfill(' ') << level << "  ";
    ss << "Lives: " << lives << "  ";
    ss << "Hlth: " << setw(3) << setfill(' ') << health << "%  ";
    ss << "Wtr: " << setw(2) << setfill(' ') << squirts << "  ";
    ss << "Gld: " << setw(2) << setfill(' ') << gold << "  ";
    ss << "Oil Left" << setw(2) << setfill(' ') << barrels << " ";
    ss << "Sonar: " << setw(2) << setfill(' ') << sonar << "  ";
    ss << "Scr: " << setw(6) << setfill('0') << score;
    
    // output to string
    string str(ss.str());
    setGameStatText(str);
}

// Returns true only if ice is cleared
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

// Returns distance between two actors
double StudentWorld::distance(int x1, int y1, int x2, int y2) const
{
    return sqrt(pow((x1 - x2),2) + pow((y1 - y2),2));
}

// Returns true if new object is within distance of 6 of an existing object
bool StudentWorld::euclidian6(int x, int y) const
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

// Returns true if actor is within certain radius from iceman
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

// Returns true if actor is facing iceman
bool StudentWorld::isFacingIceman(Actor* a) const
{
    int aX = a->getX();
    int aY = a->getY();
    GraphObject::Direction dir = a->getDirection();
    int actX = m_iceman->getX();
    int actY = m_iceman->getY();
    switch(dir)
    {
        case GraphObject::up:
            if (actY > aY && actX == aX)
                return true;
        case GraphObject::down:
            if (actY < aY && actX == aX)
                return true;
        case GraphObject::left:
            if (actX > aX && actY == aY)
                return true;
        case GraphObject::right:
            if (actX < aX && actY == aY)
                return true;
    }
    return false;
}

// Returns direction of iceman relative to actor
GraphObject::Direction StudentWorld::lineIceman(Actor* a) const
{
    int actX = a->getX();
    int actY = a->getY();
    int imX = m_iceman->getX();
    int imY = m_iceman->getY();
    
    if (imX == actX && imY > actY)
        return GraphObject::up;
    else if (imX == actX && imY < actY)
        return GraphObject::down;
    else if (imY == actY && imX < actX)
        return GraphObject::left;
    else if (imY == actY && imX > actX)
        return GraphObject::right;
    return GraphObject::none;
}

// Returns true if actor is within certain radius from a protester
bool StudentWorld::wiRadProtester(Actor *a, Actor* protester, double radius) const
{
    int actX = a->getX();
    int actY = a->getY();
    int proX = protester->getX();
    int proY = protester->getY();
    if (distance(actX, actY, proX, proY) <= radius)
        return true;
    return false;
}

// Returns true if actor is facing protester
bool StudentWorld::isFacingProtester(Actor* protester, Actor* a) const
{
    int aX = a->getX();
    int aY = a->getY();
    GraphObject::Direction dir = a->getDirection();
    int proX = protester->getX();
    int proY = protester->getY();
    switch(dir)
    {
        case GraphObject::up:
            if (proY > aY && proX == aX)
                return true;
        case GraphObject::down:
            if (proY > aY  && proX == aX)
                return true;
        case GraphObject::left:
            if (proX < aX && proY == aY)
                return true;
        case GraphObject::right:
            if (proX > aX  && proY == aY)
                return true;
    }
    return false;
}

// Generates chance of adding goodie and which goodie
int StudentWorld::chanceOfGoodie(int chance)
{
    int num = chance/2;
    int random = rand() % chance+1;
    if (random == num)
        return rand() % 5;
    return -1;
}

// Generates chance of which kind of protester to add
int StudentWorld::chanceOfProtester(int chance)
{
    int random = rand() % 100;
    if (random <= chance)
        
        return 1;   // hardcore
    else
        return 0;   // regular
}

// Insert object at random location
void StudentWorld::insertRandom(int amt, ActorType type)
{
    int i = 0;
    while (i < amt)
    {
        // generate random coordinates
        int x = rand() % (ICE_GRID_WIDTH - SPRITE_WIDTH + 1);
        int y = rand() % (ICE_GRID_HEIGHT - SPRITE_WIDTH + 1);
        
        // generate object if coordinates meet distance requirement
        if (euclidian6(x, y) && !isBoundary(type, x, y))
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
                    addActor(new Gold(this, x, y));
                    break;
                case barrel:
                    addActor(new Barrel(this, x, y));
                    break;
                case water:
                    while (isIceGrid(x, y)) // can only be generated in iceless area
                    {
                        x = rand() % (ICE_GRID_WIDTH - SPRITE_WIDTH + 1);
                        y = rand() % (ICE_GRID_HEIGHT - SPRITE_WIDTH + 1);
                    }
                    addActor(new WaterPool(this, x, y));
                    break;
            }
            i++;
        }
    }
}

// Add actors to m_actors vector
void StudentWorld::addActor(Actor* add)
{
    m_actors.push_back(add);
}

// Add objects to iceman inventory
void StudentWorld::addObjIceman(ActorType type)
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

// Deallocates all memory for dynamic objects
void StudentWorld::destroyAll()
{
    // deallocate iceman
    delete m_iceman;
    m_iceman = nullptr;
    
    // deallocate ice
    for (int r = 0; r < ICE_GRID_HEIGHT; r++)
        for (int c = 0; c < ICE_GRID_WIDTH; c++)
            clearIce(c, r);
    
    // deallocate actors
    vector<Actor*>::iterator it = m_actors.begin();
    while (it != m_actors.end())
    {
        delete (*it);
        it = m_actors.erase(it);
    }
}

// Finds quickest exit path for protester
vector<StudentWorld::Point> StudentWorld::pathToExit(Protester* pro, int inX, int inY)
{
    bool discovered[VIEW_WIDTH][VIEW_HEIGHT];
    for (int c = 0; c < VIEW_WIDTH-1; c++)
        for (int r = 0; r < VIEW_HEIGHT-1; r++)
        {
            discovered[c][r] = false;
        }
    
    queue<Point> mazequeue;
    vector<Point> finalpath;
    
    Point start = Point(inX, inY);  // initial position
    discovered[inX][inY] = true;
    mazequeue.push(start);
    
    while (!mazequeue.empty())
    {
        Point temp = mazequeue.front();
        int x = temp.m_x;
        int y = temp.m_y;
        discovered[x][y] = true;
        finalpath.push_back(temp);
        mazequeue.pop();
        
        if (x == 60 && y == 60)   // stop if exit point reached
        {
            discovered[60][60] = true;
            break;
        }
        
        // queue up and check all open points
        if (!pro->isBlocked(x, y, Actor::up))    // north
        {
            if (!discovered[x][y+1])    // add to queue if open and point not yet added
            {
//                discovered[x][y+1] = true;
                Point add = Point(x, y+1);
                mazequeue.push(add);
            }

        }
        
        if (!pro->isBlocked(x, y, Actor::down)) // south
        {
            
            if (!discovered[x][y-1])
            {
//                discovered[x][y-1] = true;
                Point add = Point(x, y-1);
                mazequeue.push(add);
            }
        }
        
        if (!pro->isBlocked(x, y, Actor::left))   // west
        {
            if (!discovered[x-1][y])
            {
//                discovered[x-1][y] = true;
                Point add = Point(x-1, y);
                mazequeue.push(add);
            }
        }
        if (!pro->isBlocked(x, y, Actor::right))   // east
        {
            if (!discovered[x+1][y])
            {
//                discovered[x+1][y] = true;
                Point add = Point(x+1, y);
                mazequeue.push(add);
            }
        }
    }
    
    // find correct path from queued open points, export as a vector
    // start from newest (end) points
    vector<Point>::iterator it = finalpath.end();
    it--;
    while (it != finalpath.begin())     // front is origin, stop here
    {
        int x = (*it).m_x;
        int y = (*it).m_y;
        
        int openCount = 0;
        // do nothing if point is exit
        if (x == 60 && y == 60)
        {
            it--;
            continue;
        }
        
        // count how many open adjacent points the coordinate has
        if (discovered[x][y+1] && x >= 0 && x < VIEW_WIDTH &&         // north
            y+1 >= 0 && y+1 < VIEW_HEIGHT)
            openCount++;
        if (discovered[x][y-1] && x >= 0 && x < VIEW_WIDTH &&         // south
            y-1 >= 0 && y-1 < VIEW_HEIGHT)
            openCount++;
        if (discovered[x+1][y] && x-1 >= 0 && x-1 < VIEW_WIDTH &&     // west
            y >= 0 && y < VIEW_HEIGHT)
            openCount++;
        if (discovered[x-1][y] && x+1 >= 0 && x+1 < VIEW_WIDTH &&     // east
            y >= 0 && y < VIEW_HEIGHT)
            openCount++;
        
        // coordinates with less than two open adjacent points is erased, not in path
        if (openCount < 2)
        {
            discovered[x][y] = false;
            it = finalpath.erase(it);
        }
        
        it--;
    }
    finalpath.erase(it);  // delete initial point in front
    return finalpath;
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
    for (int r = 4; r < 60; r++)    // remove center tunnel
    {
        for (int c = 30; c < 34; c++)
        {
            clearIce(c, r);
        }
    }
    
    // add all other actors
    int lvl = getLevel();
    int B = min(lvl/2 + 2, 9);  // boulders
    int N = max(5 - lvl/2, 2);  // gold
    int L = min(2 + lvl, 21);   // barrels
    insertRandom(B, boulder);
    insertRandom(N, gold);
    insertRandom(L, barrel);
    addActor(new RegularProtester(this));
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // update display text
    setDisplayText();
    
    // iceman actions
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
    if (dug)    // only play sound if ice was dug
        playSound(SOUND_DIG);
    
    // all actor actions
    vector<Actor*>::iterator it = m_actors.begin();
    while (it != m_actors.end())
    {
        if ((*it)->isAlive())
            (*it)->doSomething();
        if(!(*it)->isAlive())       // delete if dead
        {
            (*it)->setVisible(false);
            delete *it;
            it = m_actors.erase(it);
        }
        else
            it++;
    }
    
    // randomly add goodie, do nothing if returns -1
    int lvl = getLevel();
    int G = lvl*25 + 300;
    int chanceGoodie = chanceOfGoodie(G);
    if (chanceGoodie == 0)    // 1 in 5 chance sonar
        addActor(new SonarKit(this));
    else if (chanceGoodie > 0)    // 4 in 5 chance water
        insertRandom(1, water);
    
    // add protestor after T number of ticks
    int T = max(25, 200 - lvl);
    int lvlP = lvl*1.5;
    int P = min(15, 2 + lvlP);
    if (m_ticks % T == 0 &&  m_ticks != 0 && getProtesters() < P)
    {
        int probOfHardcore = min(90, lvl*10 + 30);
        int chanceProtest = chanceOfProtester(probOfHardcore);
        if (chanceProtest == 1)
            addActor(new RegularProtester(this));
        else
            addActor(new HardcoreProtester(this));
    }
    
    // if player dies
    if (!m_iceman->isAlive())
    {
        playSound(SOUND_PLAYER_GIVE_UP);
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    // if player finished level
    if (getBarrels() == 0)
    {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    // if game continues
    m_ticks++;
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    destroyAll();
}
