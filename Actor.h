#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class Actor: public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, Direction dir = right,
               double size = 1.0, unsigned int depth = 0);
    virtual ~Actor() {return;}           // TODO: change dummy method
    virtual void doSomething() = 0;

private:
    // Image ID already in GraphObject
    
};

class Person: public Actor
{
public:
    Person(int hp, int imageID, int startX, int startY, Direction dir = right,
           double size = 1.0, unsigned int depth = 0):
    Actor(imageID, startX, startY, dir, size, depth)
    {
        m_hp = hp;
        setVisible(true);
    }
    
    virtual ~Person();
    
private:
    int m_hp;
};


class Iceman: public Person
{
public:
    Iceman(): Person(10, IID_PLAYER, 30, 60) {}
    virtual ~Iceman() {return;}           //TODO: change dummy method
    virtual void doSomething() {return;}              //TODO: implement
private:
    // TODO: add squirtsOfWater
    // TODO: add sonar charge
    // TODO: add goldnuggets
};

class Ice: public Actor
{
public:
    Ice(int startX, int startY): Actor(IID_ICE, startX, startY, right, 0.25, 3)
    {
        setVisible(true);
    }
    virtual ~Ice() {return;}            // TODO: change dummy method
    virtual void doSomething() { return; }
};


#endif // ACTOR_H_
