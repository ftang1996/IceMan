#ifndef ACTOR_H_
#define ACTOR_H_

#include "GameConstants.h"
#include "GraphObject.h"

class StudentWorld;

class Actor: public GraphObject
{
public:
    Actor(StudentWorld* world, int imageID,
          int startX, int startY, Direction dir = right,
          double size = 1.0, unsigned int depth = 0);
    virtual ~Actor();
    
    StudentWorld* getWorld() const;
    bool isAlive() const;
    bool isPermanent() const;
    bool isPickableIceman() const;
    int getTicks() const;
    virtual bool isStunned() const;
    virtual bool isBonked() const;
    virtual bool isFixated() const;
    
    void setDead();
    void setTemp();
    void setPickableIceman(bool pickable);
    virtual void setStun(bool stun);
    virtual void setBonked(bool bonk);
    virtual void setFixated(bool fix);
    virtual void setLeave();
    void addTick();
    virtual void isAnnoyed(int annoy);
    virtual void doSomething() = 0;
private:
    StudentWorld* m_world;
    bool m_alive;
    bool m_permanent;
    bool m_pickableIceman;
    int m_ticks;
};

class Ice: public Actor
{
public:
    Ice(StudentWorld* world, int x, int y);
    virtual ~Ice();

    virtual void doSomething();
};

class Boulder: public Actor
{
public:
    Boulder(StudentWorld* world, int x, int y);
    virtual ~Boulder();
    
    bool isStable() const;
    bool isFalling() const;
    
    void setUnstable();
    void setFalling(bool fall);
    void fall(int x, int y);
    bool personUnder();
    virtual void doSomething();
private:
    bool m_stable;
    bool m_falling;
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

class Person: public Actor
{
public:
    Person(double hp, StudentWorld* world, int imageID,
           int startX, int startY, Direction dir = right,
           double size = 1.0, unsigned int depth = 0);
    virtual ~Person();
    
    double getHP() const;
    
    void setHP(double newHP);
    virtual void isAnnoyed(int annoy);
    virtual void doSomething() = 0;
private:
    double m_hp;
};

class Iceman: public Person
{
public:
    Iceman(StudentWorld* world);
    virtual ~Iceman();
    
    int getGold() const;
    int getSquirts() const;
    int getCharge() const;

    void addGold();
    void addSquirts();
    void decSquirt();
    void addCharge();
    void decCharge();
    
    void dropGold();
    void squirt();
    void useSonar();
    virtual void doSomething();
private:
    int m_gold;
    int m_squirts;
    int m_charge;
};

class Protester: public Person
{
public:
    Protester(double hp, StudentWorld* world, int imID);
    virtual ~Protester();
    
    bool isLeaving() const;
    virtual bool isBonked() const;
    virtual int getStunTicks() const;
    int getTicksToMove() const;
    int getTicksToTurn() const;
    int getRestTicks() const;
    int getTurnTicks() const;
    int getShoutTicks() const;
    int getPerpendicularTicks() const;

    virtual void setLeave();
    virtual void setBonked(bool bonk);
    virtual void setStunTicks(int ticks);
    void setTicksToMove();
    void setTicksToTurn();
    void setRestTicks(int ticks);
    void setTurnTicks(int ticks);
    void resetPerpendicularTicks();
    void resetShoutTicks();
    void addNonRestTicks();
    bool isBlocked(int x, int y, Direction dir);
    Direction randomDirection();
    bool intersectionTurn();
    bool inlineIceman();
    void move();
    void annoy();
    virtual void doSomething() = 0;
private:
    bool m_leaving;
    bool m_bonked;
    bool m_stunned;
    int m_stunTicks;
    int m_ticksToMove;
    int m_ticksToTurn;
    int m_restTicks;
    int m_turnTicks;
    int m_perpedicularTicks;
    int m_shoutTicks;
};

class RegularProtester: public Protester
{
public:
    RegularProtester(StudentWorld* world);
    virtual ~RegularProtester();
    virtual void doSomething();
    
};

class HardcoreProtester: public Protester
{
public:
    HardcoreProtester(StudentWorld* world);
    virtual ~HardcoreProtester();
    
    virtual bool isFixated() const;
    virtual int getStareTicks() const;
    
    virtual void setFixated(bool fix);
    virtual void setStareTicks(int ticks);
    virtual void doSomething();
private:
    bool m_fixated;
    int m_stareTicks;
};


#endif // ACTOR_H_
