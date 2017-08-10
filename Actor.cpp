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
}

Actor::~Actor()
{
    setVisible(false);
}

StudentWorld* Actor::getWorld()
{
    return m_world;
}

bool Actor::isAlive()
{
    return m_alive;
}

void Actor::setDead()
{
    m_alive = false;
    setVisible(false);
}

bool Actor::isPickableIceman()
{
    return m_pickableIceman;
}

void Actor::setPickableIceman(bool pickable)
{
    m_pickableIceman = pickable;
}

bool Actor::isPermanent()
{
    return m_permanent;
}

void Actor::setTemp()
{
    m_permanent = false;
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
}

int Person::getHP()
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

int Iceman::getSquirts()
{
    return m_squirts;
}

int Iceman::getCharge()
{
    return m_charge;
}

int Iceman::getGold()
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
        switch(key)
        {
            case KEY_PRESS_UP:
                if (dir == up)
                    moveTo(getX(), getY()+1);
                else
                    setDirection(up);
                break;
            case KEY_PRESS_DOWN:
                if (dir == down)
                    moveTo(getX(), getY()-1);
                else
                    setDirection(down);
                break;
            case KEY_PRESS_LEFT:
                if (dir == left)
                   moveTo(getX()-1, getY());
                else
                    setDirection(left);
                break;
            case KEY_PRESS_RIGHT:
                if (dir == right)
                    moveTo(getX()+1, getY());
                else
                    setDirection(right);
                break;
            default:
                break;
        }
    }
}

bool Iceman::isBoundary(int x, int y)
{
    // right boundary needs to acct for image size
    if (x < 0 || x > ICE_GRID_WIDTH - SPRITE_WIDTH || y < 0 || y > ICE_GRID_HEIGHT)
        return true;
    return false;
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
    setVisible(false);
    m_pickableProtester = false;
}

Gold::~Gold()
{
    setVisible(false);
    setTemp();
}

bool Gold::isPickableProtester()
{
    return m_pickableProtester;
}

void Gold::setPickableProtester()
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
        getWorld()->addItemIceman(StudentWorld::gold);
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
        getWorld()->addItemIceman(StudentWorld::barrel);
    }
}

//////////////////////////////////////////////////////////////
// Boulder Implementation                                   //
//////////////////////////////////////////////////////////////

Boulder::Boulder(StudentWorld* world, int x, int y):
Actor(world, IID_BARREL, x, y, down, 1.0, 1)
{
    setPickableIceman(false);
    m_stable = true;
    m_falling = false;
}

Boulder::~Boulder()
{
    setVisible(false);
}

bool Boulder::getStability()
{
    return m_stable;
}

void Boulder::setUnstable()
{
    m_stable = false;
}

bool Boulder::getFalling()
{
    return m_falling;
}

void Boulder::setFalling()
{
    m_falling = true;
}

void Boulder::doSomething()
{
    if (!isAlive())
        return;
}

//////////////////////////////////////////////////////////////
// SonarKit Implementation                                  //
//////////////////////////////////////////////////////////////

//SonarKit::SonarKit(StudentWorld* world, int x, int y):
//Actor(world, IID_SONAR, x, y, right, 1.0, 2)
//{
//    setVisible(true);
//    setTemp();
//
//}










