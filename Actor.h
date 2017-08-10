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
    virtual void doSomething() = 0;
    StudentWorld* getWorld();
    bool isAlive();
    void setDead();
private:
    StudentWorld* m_world;
    bool m_alive;
};

class Person: public Actor
{
public:
    Person(int hp, StudentWorld* world, int imageID,
           int startX, int startY, Direction dir = right,
           double size = 1.0, unsigned int depth = 0);
    virtual ~Person();
    virtual void doSomething() = 0;
    
    int getHP();

    

    virtual bool isBoundary(int x, int y) = 0;
    // TODO:functions to implement
    // virtual void bonked();
    // virtual void annoyed();
    // virtual void addItem() = 0;
    
private:
    int m_hp;
};

class Iceman: public Person
{
public:
    Iceman(StudentWorld* world);
    virtual ~Iceman();
    
    int getSquirts();
    int getCharge();
    int getGold();
    void addGold();
    // TODO: getBarrels

    virtual void doSomething();
    virtual bool isBoundary(int x, int y);
    // void dig(int x, int y, Direction dir);

private:
    int m_squirts;
    int m_charge;
    int m_gold;
//    int m_barrels;
};

class Ice: public Actor
{
public:
    Ice(StudentWorld* world, int x, int y);
    virtual ~Ice();
    
    virtual void doSomething();
};

class Gold: public Actor
{
public:
    Gold(StudentWorld* world, int x, int y);
    virtual ~Gold();
    
    bool isPermanent();
    void setTemp();
    bool isPickableIceman();
    bool isPickableProtester();
    void setPickableProtester();
    
    virtual void doSomething();
private:
    bool m_permanent;
    bool m_pickableIceman;
    bool m_pickableProtester;
};

class Barrel: public Actor
{
public:
    Barrel(StudentWorld* world, int x, int y);
    virtual ~Barrel();
    
    virtual void doSomething();

    
};

//TODO: Classes to implement
//class Squirt: public Actor {};
//class SonarKit: public Actor {};
//class Boulder: public Actor{};

//class Barrel: public Actor {};
//class Pool: public Actor {};
//class Protester: public Person {};
//class HardcoreProtester: public Protester {};


#endif // ACTOR_H_
