#include "Actor.h"
#include "StudentWorld.h"

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
    setDead();
    setPickableIceman(false);
}

int Person::getHP() const
{
    return m_hp;
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
            case KEY_PRESS_UP:
                if (dir == up)
                {
                    // still animates at boundaries
                    if (getWorld()->isBoundary(x, y+1))
                        y--;
                    // don't move if boulder
                    if (!getWorld()->isBoulder(x, y+1))
                        moveTo(x, y+1);
                }
                else
                    setDirection(up);
                break;
            case KEY_PRESS_DOWN:
                if (dir == down)
                {
                    if (getWorld()->isBoundary(x, y-1))
                        y++;
                    if (!getWorld()->isBoulder(x, y-1))
                        moveTo(x, y-1);
                }
                else
                    setDirection(down);
                break;
            case KEY_PRESS_LEFT:
                if (dir == left)
                {
                    if (getWorld()->isBoundary(x-1, y))
                        x++;
                    if (!getWorld()->isBoulder(x-1, y))
                        moveTo(x-1, y);
                }
                else
                    setDirection(left);
                break;
            case KEY_PRESS_RIGHT:
                if (dir == right)
                {
                    if (getWorld()->isBoundary(x+1, y))
                        x--;
                    if (!getWorld()->isBoulder(x+1, y))
                        moveTo(x+1, y);
                }
                else
                    setDirection(right);
                break;
            default:
                break;
        }
    }
}



// Moves only if destination is valid and does appropriate
// character interaction with any objects
//void Iceman::dig(int x, int y, Direction dir)
//{
//    switch(dir)
//    {
//        case up:
//            // if boundary, animate in same location, do not move
//            if (isBoundary(x, y))
//                y--;
//            else
//            {
//                for (int c = x; c < x+4; c++)
//                {
//                    int r = y+3;
//                    getWorld()->digIce(c, r);
//                }
//            }
//            break;
//        case down:
//            if (isBoundary(x, y))
//                y++;
//            else
//                for (int c = x; c < x+4; c++)
//                {
//                    int r = y;
//                    getWorld()->digIce(c, r);
//                }
//            break;
//        case left:
//            if (isBoundary(x, y))
//                x++;
//            else
//                for (int r = y; r < y+4; r++)
//                {
//                    int c = x;
//                    getWorld()->digIce(c, r);
//                }
//            break;
//        case right:
//            if (isBoundary(x, y))
//                x--;
//            else
//                for (int r = y; r < y+4; r++)
//                {
//                    int c = x+3;
//                    getWorld()->digIce(c, r);
//                }
//            break;
//    }
//        moveTo(x, y);
//}

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
    setVisible(true);
    m_pickableProtester = false;
}

Gold::~Gold()
{
    setVisible(false);
    setDead();
    setPickableIceman(false);
    setTemp();
}

bool Gold::isPickableProtester() const
{
    return m_pickableProtester;
}

void Gold::setPickableProtester()       //todo: need this???
{
    m_pickableProtester = true;
    setPickableIceman(false);
    setTemp();
}

void Gold::doSomething()
{
    if(!isAlive())
        return;
    // Reveal item if nearby
    if(!isVisible() && isAlive() && getWorld()->wiRadIceman(this, 4.0))
    {
        setVisible(true);
        return;
    }
    
    // Have iceman pickup item if nearby
    if(getWorld()->wiRadIceman(this, 3.0) && isVisible() && isPickableIceman())
    {
        setDead();
        getWorld()->addObjIceman(StudentWorld::gold);
    }
}

//////////////////////////////////////////////////////////////
// Barrel Implementation                                    //
//////////////////////////////////////////////////////////////

Barrel::Barrel(StudentWorld* world, int x, int y):
Actor(world, IID_BARREL, x, y, right, 1.0, 2)
{
    setVisible(true);
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
    moveTo(getX(), getY()-1);
    int r = getY();
    for (int c = getX(); c < getX()+SPRITE_WIDTH; c++)
        if (getWorld()->isIce(c, r))
        {
            setDead();
            setVisible(false);
        }
}

void Boulder::doSomething()
{
    if (!isAlive())
        return;
    if(!isStable())   // count ticks while unstable
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
    if(!iceUnder)   // set to waiting if not ice under
        setUnstable();
    // falls if unstable for ticks > 30
    if(!isFalling() && !isStable() && getTicks() > 30)
    {
        setFalling(true);
        getWorld()->playSound(SOUND_FALLING_ROCK);
    }
    if(isFalling())
        fall(getX(), getY());
}

//////////////////////////////////////////////////////////////
// SonarKit Implementation                                  //
//////////////////////////////////////////////////////////////

SonarKit::SonarKit(StudentWorld* world, int x, int y):
Actor(world, IID_SONAR, x, y, right, 1.0, 2)
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
    // Have iceman pickup item if nearby
    if(getWorld()->wiRadIceman(this, 3.0) && isVisible())
    {
        setDead();
        getWorld()->addObjIceman(StudentWorld::sonar);
    }
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
    // Have iceman pickup item if nearby
    if(getWorld()->wiRadIceman(this, 3.0) && isVisible())
    {
        setDead();
        getWorld()->addObjIceman(StudentWorld::water);
    }
}










