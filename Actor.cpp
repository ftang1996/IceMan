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
}

Actor::~Actor()
{
    setVisible(false);
}

StudentWorld* Actor::getWorld()
{
    return m_world;
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
    m_alive = true;
}

Person::~Person()
{
    setVisible(false);
}

int Person::getHP()
{
    return m_hp;
}
bool Person::isAlive()
{
    return m_alive;
}

void Person::setDead()
{
    m_alive = false;
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
                    dig(getX(), getY()+1, dir);
                else
                    setDirection(up);
                break;
            case KEY_PRESS_DOWN:
                if (dir == down)
                    dig(getX(), getY()-1, dir);
                else
                    setDirection(down);
                break;
            case KEY_PRESS_LEFT:
                if (dir == left)
                   dig(getX()-1, getY(),dir);
                else
                    setDirection(left);
                break;
            case KEY_PRESS_RIGHT:
                if (dir == right)
                    dig(getX()+1, getY(), dir);
                else
                    setDirection(right);
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
void Iceman::dig(int x, int y, Direction dir)
{
    switch(dir)
    {
        case up:
            // if boundary, animate in same location, do not move
            if (isBoundary(x, y))
                y--;
            else
            {
                for (int c = x; c < x+4; c++)
                {
                    int r = y+3;
                    getWorld()->digIce(c, r);
                }
            }
            break;
        case down:
            if (isBoundary(x, y))
                y++;
            else
                for (int c = x; c < x+4; c++)
                {
                    int r = y;
                    getWorld()->digIce(c, r);
                }
            break;
        case left:
            if (isBoundary(x, y))
                x++;
            else
                for (int r = y; r < y+4; r++)
                {
                    int c = x;
                    getWorld()->digIce(c, r);
                }
            break;
        case right:
            if (isBoundary(x, y))
                x--;
            else
                for (int r = y; r < y+4; r++)
                {
                    int c = x+3;
                    getWorld()->digIce(c, r);
                }
            break;
    }
        moveTo(x, y);
}

//////////////////////////////////////////////////////////////
// Ice Implementation                                       //
//////////////////////////////////////////////////////////////

Ice::Ice(StudentWorld* world, int x, int y):
Actor(world, IID_ICE, x, y, right, 0.25, 3)
{
    setVisible(true);
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
    setVisible(true);               //TODO: set to false;
    m_permanent = true;
    m_pickableIceman = true;
    m_pickableProtester = false;
}

Gold::~Gold()
{
    setVisible(false);
}

void Gold::setPickableProtester()
{
    m_pickableProtester = true;
    m_pickableIceman = false;
    m_permanent = false;
}

void Gold::doSomething()
{
    return;
}






