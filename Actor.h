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
    void addTick();
    StudentWorld* getWorld() const;
    bool isAlive() const;
    void setDead();
    bool isPickableIceman() const;
    void setPickableIceman(bool pickable);
    bool isPermanent() const;
    int getTicks() const;
    void setTemp();
    


    
private:
    StudentWorld* m_world;
    bool m_alive;
    bool m_permanent;
    bool m_pickableIceman;
    int m_ticks;
};

class Person: public Actor
{
public:
    Person(int hp, StudentWorld* world, int imageID,
           int startX, int startY, Direction dir = right,
           double size = 1.0, unsigned int depth = 0);
    virtual ~Person();
    virtual void doSomething() = 0;
    
    int getHP() const;

    

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
    
    int getSquirts() const;
    void addSquirts();
    int getCharge() const;
    void addCharge();
    int getGold() const;
    void addGold();
    // TODO: getBarrels

    virtual void doSomething();
    void dropGold();
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

    bool isPickableProtester() const;
    void droppedGold();
    
    virtual void doSomething();
private:
    bool m_pickableProtester;
};

class Barrel: public Actor
{
public:
    Barrel(StudentWorld* world, int x, int y);
    virtual ~Barrel();
    
    virtual void doSomething();

    //TODO: correct annoyed implement
};

class Boulder: public Actor
{
public:
    Boulder(StudentWorld* world, int x, int y);
    virtual ~Boulder();
    
    bool isStable() const;
    void setUnstable();
    bool isFalling() const;
    void setFalling(bool fall);
    void fall(int x, int y);
    
    virtual void doSomething();
private:
    bool m_stable;
    bool m_falling;
};

class SonarKit: public Actor
{
public:
    SonarKit(StudentWorld* world, int x, int y);
    virtual ~SonarKit();
    
    virtual void doSomething();
    
};

class WaterPool: public Actor
{
    WaterPool(StudentWorld* world, int x, int y);
    virtual ~WaterPool();
    
    virtual void doSomething();
};

//TODO: Classes to implement
//class Squirt: public Actor {};


//class Protester: public Person {};
//class HardcoreProtester: public Protester {};


#endif // ACTOR_H_
