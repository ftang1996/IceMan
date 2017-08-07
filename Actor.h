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
          double size, unsigned int depth);
    virtual ~Actor();
    StudentWorld* getWorld();

    
//    void playSound()
//    {
//        getWorld()->playSound(m_sound);
//        
//    }
    
    virtual void doSomething() = 0;
    //virtual void makeVisible() = 0;
    

private:
    StudentWorld* m_world;
};

class Person: public Actor
{
public:
    Person(int hp, StudentWorld* world, int imageID,
           int startX, int startY, Direction dir = right,
           double size = 1.0, unsigned int depth = 0);
    virtual ~Person();
    bool isAlive();
    void setDead();
    int getHP();
    virtual void actionMove(int x, int y) = 0;

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
    Iceman(StudentWorld* world);
    virtual ~Iceman();
    virtual void doSomething();
    virtual void actionMove(int x, int y);

    
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
