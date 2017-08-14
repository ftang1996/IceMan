#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"       //TODO: remove later



class StudentWorld;

class Actor: public GraphObject
{
public:
    Actor(StudentWorld* world, int imageID,
          int startX, int startY, Direction dir = right,
          double size = 1.0, unsigned int depth = 0);
    virtual ~Actor();
    virtual void doSomething() = 0;
    void addTick();
    StudentWorld* getWorld() const;
    bool isAlive() const;
    virtual void setDead();
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

    void die();
    int getHP() const;
    void setHP(int newHP);

    

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
    void decSquirt();
    int getCharge() const;
    void addCharge();
    void decCharge();
    int getGold() const;
    void addGold();
    // TODO: getBarrels

    virtual void doSomething();
    void useSonar();
    void dropGold();
    void squirt();
    void isAnnoyed(int annoy);

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
    SonarKit(StudentWorld* world);
    virtual ~SonarKit();
    
    virtual void doSomething();
    
};

class WaterPool: public Actor
{
public:
    WaterPool(StudentWorld* world, int x, int y);
    virtual ~WaterPool();
    
    virtual void doSomething();
};

class Squirt: public Actor
{
public:
    Squirt(StudentWorld* world, int x, int y, Direction dir);
    virtual ~Squirt();
    
    virtual void doSomething();
private:
    int m_traveled;
};

class Protester: public Person
{
public:
    Protester(int hp, StudentWorld* world, int imID);
    virtual ~Protester();
    virtual void doSomething() = 0;
    void setLeave();
    bool isLeaving();
    
    void addTick();
    int getRestTicks();
    int getTurnTicks();
    int getShoutTicks();
    void setRestTicks(int ticks);
    void setTurnTicks(int ticks);
    void setShoutTicks(int ticks);
//    int getShoutTick();
    
    int getTicksToMove();
    int getTicksToTurn();
    void setTicksToMove();
    void setTicksToTurn();
    
    bool isBlocked(int x, int y, Direction dir);
    Direction randomDirection();
    void move();
    void annoy();
//    void setRestTick(int num);
//    void setShoutTick(int num);
//    void setTurnTick(int num);
//    void setStepsToMove(int num);
//    int randomStepsToMove();
    
   
private:
    bool m_leaving;
    int m_restTicks;
    int m_turnTicks;
    int m_shoutTicks;
    int m_ticksToMove;
    int m_ticksToTurn;


    


//    int m_shoutTicks;

};

class RegularProtester: public Protester
{
public:
    RegularProtester(StudentWorld* world);
    virtual ~RegularProtester();
    virtual void doSomething();
    
};

//class HardcoreProtester: public Protester {};


#endif // ACTOR_H_
