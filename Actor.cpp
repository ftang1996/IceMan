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
                    actionMove(getX(), getY()+1);
                else
                    setDirection(up);
                break;
            case KEY_PRESS_DOWN:
                if (dir == down)
                    actionMove(getX(), getY()-1);
                else
                    setDirection(down);
                break;
            case KEY_PRESS_LEFT:
                if (dir == left)
                   actionMove(getX()-1, getY());
                else
                    setDirection(left);
                break;
            case KEY_PRESS_RIGHT:
                if (dir == right)
                    actionMove(getX()+1, getY());
                else
                    setDirection(right);
                break;
        }
    }
}

// Moves only if destination is valid and does appropriate
// character interaction with any objects
void Iceman::actionMove(int x, int y)
{
    Direction dir = getDirection();
    switch(dir)
    {
        case up:
            // if boundary, animate in same location, do not move
            if (getWorld()->isBoundary(x,y,getSize()))
                y--;
            // move to new location, remove ice if needed
            else
                for (int c = x; c < x+4; c++)
                {
                    int r = y+3;
                    getWorld()->removeIce(c, r);
                }
            break;
        case down:
            if (getWorld()->isBoundary(x,y,getSize()))
                y++;
            else
                for (int c = x; c < x+4; c++)
                {
                    int r = y;
                    getWorld()->removeIce(c, r);
                }
            break;
        case left:
            if (getWorld()->isBoundary(x,y,getSize()))
                x++;
            else
                for (int r = y; r < y+4; r++)
                {
                    int c = x;
                    getWorld()->removeIce(c, r);
                }
            break;
        case right:
            if (getWorld()->isBoundary(x,y,getSize()))
                x--;
            else
                for (int r = y; r < y+4; r++)
                {
                    int c = x+3;
                    getWorld()->removeIce(c, r);
                }
            break;
    }
        moveTo(x, y);
}


//////////////////////////////////////////////////////////////
// Ice Implementation                                       //
//////////////////////////////////////////////////////////////

Ice::Ice(StudentWorld* world, int startX, int startY):
Actor(world, IID_ICE, startX, startY, right, 0.25, 3)
{
    setVisible(true);
}

Ice::~Ice()
{
    setVisible(false);
}

void Ice::doSomething() {}


