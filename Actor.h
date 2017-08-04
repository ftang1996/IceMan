#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class Actor: public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, Direction dir = right,
          double size = 1.0, unsigned int depth = 0):
    GraphObject(imageID, startX, startY, dir, size, depth) {}
    
    virtual ~Actor()
    {
        setVisible(false);
    }
    
    virtual void doSomething() = 0;
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
    
    virtual ~Person()
    {
        setVisible(false);
    }
    
    // functions to implement
    // void getBonked();
    // die():
    
    

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
    Ice(int startX, int startY);
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
