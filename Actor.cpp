#include "GameConstants.h"
#include "Actor.h"
#include "GraphObject.h"
#include "StudentWorld.h"
#include <algorithm>

using namespace std;

//////////////////////////////////////////////////////////////
// Actor Implementation                                     //
//////////////////////////////////////////////////////////////

Actor::Actor(StudentWorld* world, int imageID,
      int startX, int startY, Direction dir,
      double size, unsigned int depth):
GraphObject(imageID, startX, startY, dir, size, depth)
{
    m_world = world;
    setVisible(true);
    m_alive = true;
    m_permanent = true;
    m_pickableIceman = true;
    m_ticks = 0;
}

Actor::~Actor()
{
    setVisible(false);
    setDead();
    setPickableIceman(false);
    setStun(false);
}

// Accessors
StudentWorld* Actor::getWorld() const
{
    return m_world;
}

bool Actor::isAlive() const
{
    return m_alive;
}

bool Actor::isPickableIceman() const
{
    return m_pickableIceman;
}

bool Actor::isPermanent() const
{
    return m_permanent;
}

bool Actor::isStunned() const
{ return NULL; }

int Actor::getTicks() const
{
    return m_ticks;
}

bool Actor::isBonked() const
{ return NULL; }

bool Actor::isFixated() const
{ return NULL; }

// Mutators
void Actor::setDead()
{
    m_alive = false;
}

void Actor::setTemp()
{
    m_permanent = false;
}

void Actor::setPickableIceman(bool pickable)
{
    m_pickableIceman = pickable;
}

void Actor::setStun(bool stun)
{ return; }

void Actor::setFixated(bool fix)
{ return; }

void Actor::setLeave()
{ return; }

void Actor::addTick()
{
    m_ticks++;
}

void Actor::setBonked(bool bonk)
{ return; }

void Actor::isAnnoyed(int annoy)
{ return; }

//////////////////////////////////////////////////////////////
// Ice Implementation                                       //
//////////////////////////////////////////////////////////////

Ice::Ice(StudentWorld* world, int x, int y):
Actor(world, IID_ICE, x, y, right, 0.25, 3)
{
    setVisible(true);
    setPickableIceman(false);
}

Ice::~Ice()
{
    setVisible(false);
}

void Ice::doSomething() { return; }

//////////////////////////////////////////////////////////////
// Boulder Implementation                                   //
//////////////////////////////////////////////////////////////

Boulder::Boulder(StudentWorld* world, int x, int y):
Actor(world, IID_BOULDER, x, y, down, 1.0, 1)
{
    setPickableIceman(false);
    m_stable = true;
    setFalling(false);
}

Boulder::~Boulder()
{
    setVisible(false);
    setDead();
    setPickableIceman(false);
    setFalling(false);
}

// Accessors
bool Boulder::isStable() const
{
    return m_stable;
}

bool Boulder::isFalling() const
{
    return m_falling;
}

// Mutators
void Boulder::setUnstable()
{
    m_stable = false;
}

void Boulder::setFalling(bool fall)
{
    m_falling = fall;
}

// Makes boulder fall one unit down
void Boulder::fall(int x, int y)
{
    moveTo(x, y-1);
    
    // disappears if it hits ice or another boulder
    if (getWorld()->isIceGrid(x, y-1) || getWorld()->isBoulder(this, x, y-1))
    {
        setDead();
        setVisible(false);
    }
    // kills if actor or protester is hit
    if (personUnder())
        return;
}

// Kills if actor or protester is under
bool Boulder::personUnder()
{
    bool under = false;
    
    // if iceman is under and within range
    if (getWorld()->wiRadIceman(this, 3.0) &&
        getWorld()->isFacingIceman(this))
    {
        getWorld()->getIceman()->isAnnoyed(0);
        under = true;
    }
    
    // if protester under and within range
    vector <Actor*> actors = getWorld()->getActors();
    vector<Actor*>::iterator it = actors.begin();
    while (it != actors.end())
    {
        
        if (((*it)->getID() == IID_PROTESTER ||
             (*it)->getID() == IID_HARD_CORE_PROTESTER))
        {
            if (getWorld()->wiRadProtester(this, (*it), 3.0) &&
                getWorld()->isFacingProtester((*it), this))
            {
                (*it)->isAnnoyed(0);
                (*it)->setBonked(true);
                under = true;
            }
        }
        it++;
    }
    return under;
}

void Boulder::doSomething()
{
    if (!isAlive())
        return;
    if (!isStable())   // count ticks while unstable
        addTick();
    // check if ice is under boulder
    bool iceUnder = false;
    if (isStable())
    {
        int r = getY()-1;
        for(int c = getX(); c < getX()+SPRITE_WIDTH; c++)
        {
            if(getWorld()->isIce(c,r))
            {
                iceUnder = true;
                break;
            }
        }
    }
    // set to waiting if no ice under
    if (!iceUnder)
        setUnstable();
    // falls if unstable for ticks > 30
    if (!isFalling() && !isStable() && getTicks() > 30)
    {
        setFalling(true);
        getWorld()->playSound(SOUND_FALLING_ROCK);
    }
    if (isFalling())
        fall(getX(), getY());
}

//////////////////////////////////////////////////////////////
// Gold Implementation                                      //
//////////////////////////////////////////////////////////////

Gold::Gold(StudentWorld* world, int x, int y):
Actor(world, IID_GOLD, x, y, right, 1.0, 2)
{
    setVisible(false);
    m_pickableProtester = false;
}

Gold::~Gold()
{
    setVisible(false);
    setDead();
}

// Accessors
bool Gold::isPickableProtester() const
{
    return m_pickableProtester;
}

// Modifies default properties if dropped by iceman
void Gold::droppedGold()
{
    setPickableIceman(false);
    setTemp();
    m_pickableProtester = true;
}

void Gold::doSomething()
{
    if(!isAlive())
        return;
    if(!isPermanent())
        addTick();
    
    // reveal item if within range
    if(!isVisible() && isPermanent() && getWorld()->wiRadIceman(this, 4.0))
    {
        setVisible(true);
        return;
    }
    // otherwise have iceman pickup item if within range
    else if(getWorld()->wiRadIceman(this, 3.0) && isVisible() && isPickableIceman())
    {
        setDead();
        getWorld()->addObjIceman(StudentWorld::gold);
    }
    // otherwise if in temporary state, dies if not picked up in time
    else if (!isPermanent() && getTicks() > 100)
        setDead();
    
    // pick up by protester
    vector <Actor*> actors = getWorld()->getActors();
    vector<Actor*>::iterator it = actors.begin();
    while (it != actors.end())
    {
        
        if ((*it)->getID() == IID_PROTESTER)
        {
            if (getWorld()->wiRadProtester(this, (*it), 3.0))
            {
                getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
                getWorld()->increaseScore(25);
                (*it)->setLeave();
                setDead();
            }
        }
        else if ((*it)->getID() == IID_HARD_CORE_PROTESTER)
            if (getWorld()->wiRadProtester(this, (*it), 3.0))
            {
                getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
                getWorld()->increaseScore(50);
                (*it)->setFixated(true);
                setDead();
            }
        it++;
    }
}


//////////////////////////////////////////////////////////////
// Barrel Implementation                                    //
//////////////////////////////////////////////////////////////

Barrel::Barrel(StudentWorld* world, int x, int y):
Actor(world, IID_BARREL, x, y, right, 1.0, 2)
{
    setVisible(false);
}

Barrel::~Barrel()
{
    setVisible(false);
    setDead();
    setPickableIceman(false);
}

void Barrel::doSomething()
{
    if (!isAlive())
        return;
    // reveal item if within range
    if(!isVisible() && isAlive() && getWorld()->wiRadIceman(this, 4.0))
    {
        setVisible(true);
        return;
    }
    // have iceman pickup item if within range
    if(getWorld()->wiRadIceman(this, 3.0) && isVisible())
    {
        setDead();
        getWorld()->addObjIceman(StudentWorld::barrel);
    }
}

//////////////////////////////////////////////////////////////
// SonarKit Implementation                                  //
//////////////////////////////////////////////////////////////

SonarKit::SonarKit(StudentWorld* world):
Actor(world, IID_SONAR, 0, 60, right, 1.0, 2)
{
    setVisible(true);
    setTemp();
}

SonarKit::~SonarKit()
{
    setVisible(false);
    setDead();
    setPickableIceman(false);
}

void SonarKit::doSomething()
{
    if (!isAlive())
        return;
    // have iceman pickup item if within range
    if (getWorld()->wiRadIceman(this, 3.0) && isVisible())
    {
        setDead();
        getWorld()->addObjIceman(StudentWorld::sonar);
    }
    // item disappears if not picked up by ticks
    addTick();
    int lvl = getWorld()->getLevel();
    int T = max(100, 300 - 10*lvl);
    if (getTicks() > T)
        setDead();
}

//////////////////////////////////////////////////////////////
// WaterPool Implementation                                 //
//////////////////////////////////////////////////////////////

WaterPool::WaterPool(StudentWorld* world, int x, int y):
Actor(world, IID_WATER_POOL, x, y, right, 1.0, 2)
{
    setVisible(true);
    setTemp();
}

WaterPool::~WaterPool()
{
    setVisible(false);
    setDead();
    setPickableIceman(false);
}

void WaterPool::doSomething()
{
    if (!isAlive())
        return;
    
    // have iceman pickup item if within range
    if (getWorld()->wiRadIceman(this, 3.0) && isVisible())
    {
        setDead();
        getWorld()->addObjIceman(StudentWorld::water);
    }
    // item disappears if not picked up by ticks
    addTick();
    int lvl = getWorld()->getLevel();
    int T = max(100, 300 - 10*lvl);
    if (getTicks() > T)
        setDead();
}

//////////////////////////////////////////////////////////////
// Squirt Implementation                                    //
//////////////////////////////////////////////////////////////

Squirt::Squirt(StudentWorld* world, int x, int y, Direction dir):
Actor(world, IID_WATER_SPURT, x, y, dir, 1.0, 1)
{
    setVisible(true);
}

Squirt::~Squirt()
{
    setVisible(false);
    setPickableIceman(false);
    setDead();
}

void Squirt::doSomething()
{
    if (!isAlive())
        return;
    // continue moving in direction
    int x = getX();
    int y = getY();
    switch (getDirection())
    {
        case up:
            moveTo(x, y+1);
            break;
        case down:
            moveTo(x, y-1);
            break;
        case left:
            moveTo(x-1, y);
            break;
        case right:
            moveTo(x+1, y);
            break;
        default:
            break;
    }
    m_traveled++;
    
    // annoy if squirt hits a protester
    vector<Protester*> protesters;
    vector <Actor*> actors = getWorld()->getActors();
    vector<Actor*>::const_iterator it = actors.begin();
    while (it != actors.end())
    {

        if (((*it)->getID() == IID_PROTESTER ||
            (*it)->getID() == IID_HARD_CORE_PROTESTER))
        {
            if (getWorld()->wiRadProtester(this, (*it), 3.0) &&
                getWorld()->isFacingProtester((*it), this))
            {
                (*it)->isAnnoyed(2);
                (*it)->setStun(true);
                getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
                setDead();
            }
        }
        it++;
    }
    
    // disappear if squirt travelled max distance or hits ice or a boulder
    if (m_traveled >= 4 || getWorld()->isIceGrid(x, y)
        || getWorld()->isBoulder(this, x, y))
    {
        setDead();
        return;
    }
}

//////////////////////////////////////////////////////////////
// Person Implementation                                    //
//////////////////////////////////////////////////////////////

Person::Person(double hp, StudentWorld* world, int imageID,
               int startX, int startY, Direction dir,
               double size, unsigned int depth):
Actor(world, imageID, startX, startY, dir, size, depth)
{
    m_hp = hp;
    setPickableIceman(false);
}

Person::~Person()
{
    setVisible(false);
    setDead();
    setHP(0);
}

// Accesors
double Person::getHP() const
{
    return m_hp;
}

//Mutators
void Person::setHP(double newHP)
{
    m_hp = newHP;
}

// Reduce HP by amount annoyed, set hp = 0 if 0 is input
void Person::isAnnoyed(int annoy)
{
    if (annoy == 0)
        setHP(0);
    setHP(getHP() - annoy);
}

//////////////////////////////////////////////////////////////
// Iceman Implementation                                    //
//////////////////////////////////////////////////////////////

Iceman::Iceman(StudentWorld* world): Person(10, world, IID_PLAYER, 30, 60)
{
    m_squirts = 5;
    m_charge = 1;
    m_gold = 0;
}

Iceman::~Iceman()
{
    setVisible(false);
    setDead();
}

// Accessor functions
int Iceman::getGold() const
{
    return m_gold;
}

int Iceman::getSquirts() const
{
    return m_squirts;
}

int Iceman::getCharge() const
{
    return m_charge;
}

// Mutator functions
void Iceman::addGold()
{
    m_gold++;
}

void Iceman::addSquirts()
{
    m_squirts += 5;
}

void Iceman::decSquirt()
{
    m_squirts--;
}

void Iceman::addCharge()
{
    m_charge++;
}

void Iceman::decCharge()
{
    m_charge--;
}

// Add new gold that iceman drops to world
void Iceman::dropGold()
{
    if (getGold() > 0)
    {
        int x = getX();
        int y = getY();
        Gold* drGold = new Gold(getWorld(), x, y);
        drGold->droppedGold();
        
        getWorld()->addActor(drGold);
    }
}

// Add new squirt to world if iceman squirts gun
void Iceman::squirt()
{
    if (getSquirts() > 0)
    {
        getWorld()->playSound(SOUND_PLAYER_SQUIRT);
        int x = getX();
        int y = getY();
        Direction dir = getDirection();
        Squirt* addSquirt;
        // initiate squirt direction
        switch (getDirection())
        {
            case up:
                addSquirt = new Squirt(getWorld(), x, y+4, dir);
                break;
            case down:
                addSquirt = new Squirt(getWorld(), x, y-1, dir);
                break;
            case left:
                addSquirt = new Squirt(getWorld(), x-1, y, dir);
                break;
            case right:
                addSquirt = new Squirt(getWorld(), x+4, y, dir);
                break;
        }
        getWorld()->addActor(addSquirt);
        decSquirt();
    }
}

// Reveal nearby items if iceman uses sonar
void Iceman::useSonar()
{
    if (getCharge() == 0)
        return;
    getWorld()->playSound(SOUND_SONAR);
    vector <Actor*> actors = getWorld()->getActors();
    vector<Actor*>::iterator it = actors.begin();
    while (it != actors.end())
    {
        if (((*it)->getID() == IID_GOLD || (*it)->getID() == IID_BARREL)
            && getWorld()->wiRadIceman((*it), 12.0))
            (*it)->setVisible(true);
        it++;
    }
    decCharge();
}

void Iceman::doSomething()
{
    if (!isAlive())
        return;
    
    // get player key inputs
    Direction dir = getDirection();
    int key;
    if (getWorld()->getKey(key))
    {
        int x = getX();
        int y = getY();
        switch(key)
        {
            case KEY_PRESS_ESCAPE:
                setDead();
                break;
            case KEY_PRESS_UP:
                if (dir == up)
                {
                    // still animates against boundaries
                    if (getWorld()->isBoundary(StudentWorld::iceman, x, y+1))
                        y--;
                    // don't move if against boulder
                    if (!getWorld()->isBoulder(this, x, y+1))
                        moveTo(x, y+1);
                }
                else
                    setDirection(up);
                break;
            case KEY_PRESS_DOWN:
                if (dir == down)
                {
                    if (getWorld()->isBoundary(StudentWorld::iceman, x, y-1))
                        y++;
                    if (!getWorld()->isBoulder(this, x, y-1))
                        moveTo(x, y-1);
                }
                else
                    setDirection(down);
                break;
            case KEY_PRESS_LEFT:
                if (dir == left)
                {
                    if (getWorld()->isBoundary(StudentWorld::iceman, x-1, y))
                        x++;
                    if (!getWorld()->isBoulder(this, x-1, y))
                        moveTo(x-1, y);
                }
                else
                    setDirection(left);
                break;
            case KEY_PRESS_RIGHT:
                if (dir == right)
                {
                    if (getWorld()->isBoundary(StudentWorld::iceman, x+1, y))
                        x--;
                    if (!getWorld()->isBoulder(this, x+1, y))
                        moveTo(x+1, y);
                }
                else
                    setDirection(right);
                break;
            case KEY_PRESS_TAB:
                dropGold();
                break;
            case KEY_PRESS_SPACE:
                squirt();
                break;
            case 'z':
            case 'Z':
                useSonar();
                break;
            default:
                break;
        }
    }

    if (getHP() <= 0)
        setDead();
}

//////////////////////////////////////////////////////////////
// Protestor Implementation                                 //
//////////////////////////////////////////////////////////////

Protester::Protester(double hp, StudentWorld* world, int imID):
Person(hp, world, imID, 60, 60, left)
{
    m_leaving = false;
    m_stunned = false;
    m_bonked = false;
    setTicksToTurn();
    setTicksToMove();
    m_restTicks = 0;
    m_turnTicks = 0;
    m_perpedicularTicks = 200;
    m_shoutTicks = 15;
    m_stunned = 0;
}

Protester::~Protester()
{
    setVisible(false);
}

// Accessor functions
bool Protester::isLeaving() const
{
    return m_leaving;
}

bool Protester::isBonked() const
{
    return m_bonked;
}

int Protester::getStunTicks() const
{
    return m_stunTicks;
}

int Protester::getTicksToMove() const
{
    // number of rest ticks before next move
    return m_ticksToMove;
}

int Protester::getTicksToTurn() const
{
    // ticks until next turn between 8 to 61 ticks
    return m_ticksToTurn;
}

int Protester::getRestTicks() const
{
    // rest tick counter
    return m_restTicks;
}

int Protester::getTurnTicks() const
{
    // turn tick counter
    return m_turnTicks;
}

int Protester::getPerpendicularTicks() const
{
    // counter for ticks since last intersection turn
    return m_perpedicularTicks;
}

int Protester::getShoutTicks() const
{
    // count for ticks since last shout
    return m_shoutTicks;
}

// Mutator functions
void Protester::setLeave()
{
    m_leaving = true;
}

void Protester::setBonked(bool bonk)
{
    m_bonked = bonk;
}

void Protester::setStunTicks(int ticks)
{
    m_stunTicks = ticks;
}

void Protester::setTicksToMove()
{
    int lvl = getWorld()->getLevel();
    m_ticksToMove = max(0, 3 - lvl/4);
}

void Protester::setTicksToTurn()
{
    m_ticksToTurn = rand() % 61 + 8;
}

void Protester::setRestTicks(int ticks)
{
    m_restTicks = ticks;
}

void Protester::setTurnTicks(int ticks)
{
    m_turnTicks = ticks;
}

void Protester::resetPerpendicularTicks()
{
    m_perpedicularTicks = 0;
}

void Protester::resetShoutTicks()
{
    m_shoutTicks = 0;
}

void Protester::addNonRestTicks()
{
    m_shoutTicks++;
    m_perpedicularTicks++;
}

// Checks if protester move is blocked
bool Protester::isBlocked(int x, int y, Direction dir)
{
    switch(dir)
    {
        case up:
            if (getWorld()->isIceGrid(x, y+1) ||
                getWorld()->isBoulder(this, x, y+1) ||
                getWorld()->isBoundary(StudentWorld::protester, x, y+1))
                return true;
            break;
        case down:
            if (getWorld()->isIceGrid(x, y-1) ||
                getWorld()->isBoulder(this, x, y-1) ||
                getWorld()->isBoundary(StudentWorld::protester, x, y-1))
                return true;
            break;
        case left:
            if (getWorld()->isIceGrid(x-1, y) ||
                getWorld()->isBoulder(this, x-1, y) ||
                getWorld()->isBoundary(StudentWorld::protester, x-1, y))
                return true;
            break;
        case right:
            if (getWorld()->isIceGrid(x+1, y) ||
                getWorld()->isBoulder(this, x+1, y) ||
                getWorld()->isBoundary(StudentWorld::protester, x+1, y))
                return true;
            break;
    }
    return false;
}

// Choose random direction for protester to move
GraphObject::Direction Protester::randomDirection()
{
    int dir = rand() % 4;
    switch (dir)
    {
        case 0:
            return up;
            break;
        case 1:
            return down;
            break;
        case 2:
            return left;
            break;
        case 3:
            return right;
            break;
        default:
            return up;
            break;
    }
}

bool Protester::intersectionTurn()
{
    Direction dir = getDirection();
    int x = getX();
    int y = getY();
    switch (dir)
    {
        case up:
        case down:
            // both directions viable
            if (!isBlocked(x, y, left) && !isBlocked(x, y, right))
            {
                int random = rand() % 2;
                if (random == 0)
                    setDirection(left);
                else
                    setDirection(right);
                return true;
            }
            // left viable
            else if (!isBlocked(x, y, left))
            {
                setDirection(left);
                return true;
            }
            // right viable
            else if (!isBlocked(x, y, right))
            {
                setDirection(right);
                return true;
            }
            break;
        case left:
        case right:
            // both directions viable
            if (!isBlocked(x, y, up) && !isBlocked(x, y, down))
            {
                int random = rand() % 2;
                if (random == 0)
                    setDirection(up);
                else
                    setDirection(down);
                return true;
            }
            // up direction viable
            else if (!isBlocked(x, y, up))
            {
                setDirection(up);
                return true;
            }
            // down direction viable
            else if (!isBlocked(x, y, down))
            {
                setDirection(down);
                return true;
            }
            break;
    }
    return false;   // return false if can't change direction
}

// Sets direction to face iceman if protester is inline with iceman
bool Protester::inlineIceman()
{
    int x = getX();
    int y = getY();
    Direction dir = getDirection();

    if (getWorld()->lineIceman(this) != none && getWorld()->wiRadIceman(this, 4.0))
    {
        Direction newDir = getWorld()->lineIceman(this);
        switch(dir)
        {
            case up:
                if (!isBlocked(x, y+1, dir))
                {
                    setDirection(newDir);
                    return true;
                }
                break;
            case down:
                if (!isBlocked(x, y-1, dir))
                {
                    setDirection(newDir);
                    return true;
                }
                break;
            case left:
                if (!isBlocked(x-1, y, dir))
                {
                    setDirection(newDir);
                    return true;
                }
                break;
            case right:
                if (!isBlocked(x+1, y, dir))
                {
                    setDirection(newDir);
                    return true;
                }
                break;
        }
    }
    return false;
}

//void Protester::moveTowardIceman()
//{
//    Direction dir = getDirection();
//    int x = getX();
//    int y = getY();
//        if (!isBlocked(x, y, dir))
//        {
//            switch(dir)
//            {
//                case up:
//                    moveTo(x, y+1);
//                    break;
//                case down:
//                    moveTo(x, y-1);
//                    break;
//                case left:
//                    moveTo(x-1, y);
//                    break;
//                case right:
//                    moveTo(x+1, y);
//                    break;
//            }
//        }
//        // change direction if hitting ice
//        else
//            setTurnTicks(getTicksToTurn()+1);
//    }
//        
//    }
//}

void Protester::move()
{
    Direction dir = getDirection();
    int x = getX();
    int y = getY();
    if (!isBlocked(x, y, dir))
    {
        switch(dir)
        {
            case up:
                moveTo(x, y+1);
                break;
            case down:
                moveTo(x, y-1);
                break;
            case left:
                moveTo(x-1, y);
                break;
            case right:
                moveTo(x+1, y);
                break;
        }
    }
    // change direction if hitting ice
    else
        setTurnTicks(getTicksToTurn()+1);
}

void Protester::annoy()
{
    getWorld()->getIceman()->isAnnoyed(2);
}

//////////////////////////////////////////////////////////////
// Regular Protester Implementation                         //
//////////////////////////////////////////////////////////////

RegularProtester::RegularProtester(StudentWorld* world):
Protester(5, world, IID_PROTESTER)
{ }

RegularProtester::~RegularProtester()
{
    setDead();
    setHP(0);
    setTurnTicks(0);
    setRestTicks(0);
    setVisible(false);
}

void RegularProtester::doSomething()
{
    if (!isAlive())
        return;
    
    // check if dead
    if (getHP() <= 0)
    {
        if (isBonked()) // if killed by boulder
            getWorld()->increaseScore(500);
        else    // if killed by squirt
            getWorld()->increaseScore(100);
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        setLeave();
    }
    
    if (isLeaving())
    {
        setDead();
        
        // TODO: implement maze
//        int x = getX();
//        int y = getY();
//        if (x == 60 && y == 60)
//            setDead();
//        else
//        {
//            getWorld()->exitPath(this, x, y);
//            
//        }
    }
    
    // if hit by squirt gun
    if (isStunned())
    {
        int lvl = getWorld()->getLevel();
        int N = max(50, 100-lvl*100);
        if (getStunTicks() <= N)
        {
            setStunTicks(getStunTicks()+1);
            return;
        }
        else
            setStun(false);
    }
    
    // check ticks til next move
    if (getRestTicks() <= getTicksToMove())
    {
        setRestTicks(getTicksToMove()+1);
        return;
    }
    
    // yell if iceman is near
    if (getWorld()->wiRadIceman(this, 4.0) && getWorld()->isFacingIceman(this))
    {
        if (getShoutTicks() > 15)
        {
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            annoy();
            resetShoutTicks();
            return;
        }
    }
    
    // check ticks til next turn
    if (getTurnTicks() <= getTicksToTurn())
        setTurnTicks(getTurnTicks()+1);
    else
    {
        while(isBlocked(getX(), getY(), getDirection()))
            setDirection(randomDirection());
        setTicksToTurn();
        setTurnTicks(0);
    }
    
    // checks if at intersection and if protester should turn
    if (getPerpendicularTicks() > 200)
    {
        if (intersectionTurn())     // only reset ticks if protester turned
        {
            resetPerpendicularTicks();
            setTurnTicks(0);
            setTicksToTurn();
            return;
        }
    }

    // moves
    if (getRestTicks())
    {
        if (inlineIceman())     // turns if inline with iceman
        {
            setTurnTicks(0);
            setTicksToTurn();
            addNonRestTicks();
            return;
        }
        move();
        setRestTicks(0);
        addNonRestTicks();
    }
}

//////////////////////////////////////////////////////////////
// Hardcore Protester Implementation                        //
//////////////////////////////////////////////////////////////

HardcoreProtester::HardcoreProtester(StudentWorld* world):
Protester(20, world, IID_HARD_CORE_PROTESTER)
{
    setFixated(false);
}

HardcoreProtester::~HardcoreProtester()
{
    setDead();
    setHP(0);
    setTurnTicks(0);
    setRestTicks(0);
    setVisible(false);
}

// Accessor functions
bool HardcoreProtester::isFixated() const
{
    return m_fixated;
}

int HardcoreProtester::getStareTicks() const
{
    return m_stareTicks;
}

// Mutator functions
void HardcoreProtester::setFixated(bool fix)
{
    m_fixated = fix;
}

void HardcoreProtester::setStareTicks(int ticks)
{
    m_stareTicks = ticks;
}

void HardcoreProtester::doSomething()
{
    if (!isAlive())
        return;
    
    // check if dead
    if (getHP() <= 0)
    {
        if (isBonked()) //if killed by boulder
            getWorld()->increaseScore(500);
        else    // if killed by squirt
            getWorld()->increaseScore(250);
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        setLeave();
    }
    
    if (isLeaving())
    {
        setDead();
        // TODO: implement maze
        //        int x = getX();
        //        int y = getY();
        //        if (x == 60 && y == 60)
        //            setDead();
        //        else
        //        {
        //            getWorld()->exitPath(this, x, y);
        //
        //        }
    }
    
    // if hit by squirt gun
    if (isStunned())
    {
        int lvl = getWorld()->getLevel();
        int N = max(50, 100-lvl*100);
        if (getStunTicks() <= N)
        {
            setStunTicks(getStunTicks()+1);
            return;
        }
        else
            setStun(false);
    }
    
    // if fixated by gold
    if (isFixated())
    {
        int lvl = getWorld()->getLevel();
        int ticks_to_stare = max(50, 100 - lvl*10);
        if (getStareTicks() <= ticks_to_stare)
        {
            setStareTicks(getStareTicks()+1);
            return;
        }
        else
            setStareTicks(0);
    }
    
    // check ticks til next move
    if (getRestTicks() <= getTicksToMove())
    {
        setRestTicks(getTicksToMove()+1);
        return;
    }
    
    // yell if iceman is near
    if (getWorld()->wiRadIceman(this, 4.0) && getWorld()->isFacingIceman(this))
    {
        if (getShoutTicks() > 15)
        {
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            annoy();
            resetShoutTicks();
            return;
        }
    }
    
    // check ticks til next turn
    if (getTurnTicks() <= getTicksToTurn())
        setTurnTicks(getTurnTicks()+1);
    else
    {
        while(isBlocked(getX(), getY(), getDirection()))
            setDirection(randomDirection());
        setTicksToTurn();
        setTurnTicks(0);
    }
    
    // checks if at intersection and if protester should turn
    if (getPerpendicularTicks() > 200)
    {
        if (intersectionTurn())     // only reset ticks if protester turned
        {
            resetPerpendicularTicks();
            setTurnTicks(0);
            setTicksToTurn();
            return;
        }
    }
    
    // moves
    if (getRestTicks())
    {
        if (inlineIceman())     // turns if inline with iceman
        {
            setTurnTicks(0);
            setTicksToTurn();
            addNonRestTicks();
            return;
        }
        move();
        setRestTicks(0);
        addNonRestTicks();
    }
}






