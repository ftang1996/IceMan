#include "Actor.h"
#include "StudentWorld.h"

//////////////////////////////////////////////////////////////
// Actor Implementation                                     //
//////////////////////////////////////////////////////////////
Actor::Actor(StudentWorld* world, int imageID,
      int startX, int startY, Direction dir,
      double size, unsigned int depth):
GraphObject(imageID, startX, startY, dir, size, depth)
{
    m_world = world;
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
    setVisible(true);
}

Person::~Person()
{
    setVisible(false);
}

bool Person::isAlive()
{
    return m_alive;
}

void Person::setDead()
{
    m_alive = false;
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
                if (dir == up)
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
    
    
                }
            }
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


