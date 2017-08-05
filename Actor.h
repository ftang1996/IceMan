#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"       //TODO: remove later

class StudentWorld;

class Actor: public GraphObject
{
public:
    Actor(StudentWorld* world, int imageID,
          int startX, int startY, Direction dir,
          double size, unsigned int depth):
    GraphObject(imageID, startX, startY, dir, size, depth)
    {
        m_world = world;
    }
    
    virtual ~Actor()
    {
        setVisible(false);
    }
    
    StudentWorld* getWorld()
    {
        return m_world;
    }
    
//    void playSound()
//    {
//        getWorld()->playSound(m_sound);
//        
//    }
    
    virtual void doSomething() = 0;
    virtual void makeVisible() = 0;
    

private:
    StudentWorld* m_world;
};

class Person: public Actor
{
public:
    Person(int hp, StudentWorld* world, int imageID,
           int startX, int startY, Direction dir = right,
           double size = 1.0, unsigned int depth = 0):
    Actor(world, imageID, startX, startY, dir, size, depth)
    {
        m_hp = hp;
        m_alive = true;
        setVisible(true);
    }
    
    virtual ~Person()
    {
        setVisible(false);
    }
    
    bool isAlive()
    {
        return m_alive;
    }
    
    void setDead()
    {
        m_alive = false;
    }
    
    int getHP()
    {
        return m_hp;
    }
    
    void changeHP(int delta)
    {
        m_hp += delta;
    }
    // functions to implement
    // virtual void bonked();
    // virtual void annoyed();
    // virtual void addItem() = 0;
    // die():
    
private:
    int m_hp;
    bool m_alive;
};


class Iceman: public Person
{
public:
    Iceman(StudentWorld* world): Person(10, world, IID_PLAYER, 30, 60)
    {
        m_squirts = 5;
        m_charge = 1;
        m_gold = 0;
    }
    virtual ~Iceman()
    {
        setVisible(false);
        setDead();
    }
    virtual void doSomething()
    {
        if (!isAlive())
            return;
        
        Direction dir = getDirection();
        int key;
        
        if (getWorld()->getKey(key))
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
    
    // TODO: getSquirts
    // TODO: getCharge
    // TODO: getGold
    // TODO: getBarrels
    
private:
    int m_squirts;
    int m_charge;
    int m_gold;
    
    
};








class Ice: public Actor
{
public:
    Ice(StudentWorld* world, int startX, int startY);
    virtual ~Ice();
    virtual void doSomething();
};

//TODO: Classes to implement
//class Squirt: public Actor {};
//class SonarKit: public Actor {};
//class Boulder: public Actor{};
//class Gold: public Actor {};
//class Barrel: public Actor {};
//class Pool: public Actor {};
//class Protester: public Person {};
//class HardcoreProtester: public Protester {};
//

#endif // ACTOR_H_
