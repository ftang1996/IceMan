#include "Actor.h"
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
}

StudentWorld* Actor::getWorld() const
{
    return m_world;
}

bool Actor::isAlive() const
{
    return m_alive;
}

void Actor::setDead()
{
    m_alive = false;
    setVisible(false);
}

bool Actor::isPickableIceman() const
{
    return m_pickableIceman;
}

void Actor::setPickableIceman(bool pickable)
{
    m_pickableIceman = pickable;
}

bool Actor::isPermanent() const
{
    return m_permanent;
}

void Actor::setTemp()
{
    m_permanent = false;
}

int Actor::getTicks() const
{
    return m_ticks;
}

void Actor::addTick()
{
    m_ticks++;
}

//////////////////////////////////////////////////////////////
// Person Implementation                                    //
//////////////////////////////////////////////////////////////


Person::Person(int hp, StudentWorld* world, int imageID,
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
    die();
}

void Person::die()
{
    setDead();
    setHP(0);
}

int Person::getHP() const
{
    return m_hp;
}

void Person::setHP(int newHP)
{
    m_hp = newHP;
}

//////////////////////////////////////////////////////////////
// Protestor Implementation                                 //
//////////////////////////////////////////////////////////////

Protester::Protester(int hp, StudentWorld* world, int imID):
Person(hp, world, imID, 60, 60, left)
{
    m_leaving = false;
    setTicksToTurn();
    setTicksToMove();
}

Protester::~Protester()
{
    setVisible(false);
    die();
}


bool Protester::isLeaving()
{
    return m_leaving;
}

void Protester::setLeave()
{
    m_leaving = false;
}

int Protester::getRestTicks()
{
    return m_restTicks;
}

int Protester::getTurnTicks()
{
    return m_turnTicks;
}

void Protester::setRestTicks(int ticks)
{
    m_restTicks = ticks;
}

void Protester::setTurnTicks(int ticks)
{
    m_turnTicks = ticks;
}

int Protester::getTicksToMove()
{
    return m_ticksToMove;
}

int Protester::getTicksToTurn()
{
    return m_ticksToTurn;
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

bool Protester::isBlocked(int x, int y, Direction dir)
{
    switch(dir)
    {
        case up:
            if (getWorld()->isIceGrid(x, y+1) ||
                getWorld()->isBoulder(this, x, y+1) ||
                getWorld()->isBoundary(StudentWorld::protester, x, y+1))
                return false;
        case down:
            if (getWorld()->isIceGrid(x, y-1) ||
                getWorld()->isBoulder(this, x, y-1) ||
                getWorld()->isBoundary(StudentWorld::protester, x, y-1))
                return false;
        case left:
            if (getWorld()->isIceGrid(x+1, y) ||
                getWorld()->isBoulder(this, x+1, y) ||
                getWorld()->isBoundary(StudentWorld::protester, x-1, y))
                return false;
        case right:
            if (getWorld()->isIceGrid(x-1, y) ||
                getWorld()->isBoulder(this, x-1, y) ||
                getWorld()->isBoundary(StudentWorld::protester, x+1, y))
                return false;
    }
    return true;
}

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

void Protester::move()
{

}

//    int getShoutTick();

//////////////////////////////////////////////////////////////
// Regular Protester Implementation                         //
//////////////////////////////////////////////////////////////

RegularProtester::RegularProtester(StudentWorld* world):
Protester(5, world, IID_PROTESTER)
{

}

RegularProtester::~RegularProtester()
{
    setVisible(false);
    die();
}

void RegularProtester::doSomething()
{
    if (!isAlive())
        return;
    
    if (getTurnTicks() <= getTicksToTurn()) // check ticks til next turn
        setTurnTicks(getTurnTicks()+1);
    else
    {
        while(isBlocked(getX(), getY(), getDirection()))
            setDirection(randomDirection());
        setTicksToTurn();
        setTurnTicks(0);
    }
    if (getRestTicks() <= getTicksToMove()) // check ticks til next move
    {
        setRestTicks(getTicksToMove()+1);
        return;
    }
    else
    {
        move();
        setRestTicks(0);
    }
    
    if (getHP() == 0)
    {
        setLeave();
    }
    
    
    
}


//////////////////////////////////////////////////////////////
// Iceman Implementation                                    //
//////////////////////////////////////////////////////////////

Iceman::Iceman(StudentWorld* world): Person(10, world, IID_PLAYER, 30, 60)
{
    m_squirts = 5;
    m_charge = 10;
    m_gold = 0;
}

Iceman::~Iceman()
{
    setVisible(false);
    setDead();
}

int Iceman::getSquirts() const
{
    return m_squirts;
}

int Iceman::getCharge() const
{
    return m_charge;
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

int Iceman::getGold() const
{
    return m_gold;
}

void Iceman::addGold()
{
    m_gold++;
}

void Iceman::doSomething()
{
    if (!isAlive())
        return;
    
    Direction dir = getDirection();
    int key;
    if (getWorld()->getKey(key))    // get player key inputs
    {
        int x = getX();
        int y = getY();
        switch(key)
        {
            case KEY_PRESS_ESCAPE:
                die();
                break;
            case KEY_PRESS_UP:
                if (dir == up)
                {
                    // still animates at boundaries
                    if (getWorld()->isBoundary(StudentWorld::iceman, x, y+1))
                        y--;
                    // don't move if boulder
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
}

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

void Iceman::squirt()
{
    if (getSquirts() > 0)
    {
        getWorld()->playSound(SOUND_PLAYER_SQUIRT);
        int x = getX();
        int y = getY();
        Direction dir = getDirection();
        Squirt* addSquirt;
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

void Iceman::decCharge()
{
    m_charge--;
}

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

void Ice::doSomething() {}

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

bool Gold::isPickableProtester() const
{
    return m_pickableProtester;
}

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
    // Reveal item if nearby
    if(!isVisible() && isPermanent() && getWorld()->wiRadIceman(this, 4.0))
    {
        setVisible(true);
        return;
    }
    // Have iceman pickup item if nearby
    else if(getWorld()->wiRadIceman(this, 3.0) && isVisible() && isPickableIceman())
    {
        setDead();
        getWorld()->addObjIceman(StudentWorld::gold);
    }
    else if (getTicks() > 100)
        setDead();
    

    
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
    // Reveal item if nearby
    if(!isVisible() && isAlive() && getWorld()->wiRadIceman(this, 4.0))
    {
        setVisible(true);
        return;
    }
    // Have iceman pickup item if nearby
    if(getWorld()->wiRadIceman(this, 3.0) && isVisible())
    {
        setDead();
        getWorld()->addObjIceman(StudentWorld::barrel);
    }
}

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

bool Boulder::isStable() const
{
    return m_stable;
}

void Boulder::setUnstable()
{
    m_stable = false;
}

bool Boulder::isFalling() const
{
    return m_falling;
}

void Boulder::setFalling(bool fall)
{
    m_falling = fall;
}

// makes boulder fall one unit down
void Boulder::fall(int x, int y)
{
    moveTo(x, y-1);
    //int r = getY();
    //for (int c = getX(); c < getX()+SPRITE_WIDTH; c++)
    if (getWorld()->isIceGrid(x, y-1) || getWorld()->isBoulder(this, x, y-1))
    {
        setDead();
        setVisible(false);
    }
}

void Boulder::doSomething()
{
    if (!isAlive())
        return;
    if (!isStable())   // count ticks while unstable
        addTick();
    bool iceUnder = false;  // check is is under boulder
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
    if (!iceUnder)   // set to waiting if not ice under
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
    // have iceman pickup item if nearby
    if (getWorld()->wiRadIceman(this, 3.0) && isVisible())
    {
        setDead();
        getWorld()->addObjIceman(StudentWorld::sonar);
    }
    // check whether temp item is ticks are up
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
    
    // have iceman pickup item if nearby
    if (getWorld()->wiRadIceman(this, 3.0) && isVisible())
    {
        setDead();
        getWorld()->addObjIceman(StudentWorld::water);
    }
    // check whether temp item is ticks are up
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
    
    int x = getX();
    int y = getY();
    switch (getDirection())
    {
        case up:
            moveTo(getX(), getY()+1);
            break;
        case down:
            moveTo(getX(), getY()-1);
            break;
        case left:
            moveTo(getX()-1, getY());
            break;
        case right:
            moveTo(getX()+1, getY());
            break;
        default:
            break;
    }
    m_traveled++;
    
    if (m_traveled >= 4 || getWorld()->isIceGrid(x, y)
        || getWorld()->isBoulder(this, x, y))
    {
        setDead();
        return;
    }
}










