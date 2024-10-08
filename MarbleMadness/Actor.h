#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"


class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(int startX, int startY, int imageID, StudentWorld* world, int hitPoints = 0, int startDir = -1);
    // Action to perform each tick

    virtual void doSomething() = 0;

    // Is this actor alive?
    bool isAlive() const;

    // Mark this actor as dead
    void setAlive(bool b);

    // Reduce this actor's hit points
    void decHitPoints(int amt);

    // Get this actor's world
    StudentWorld* getWorld() const;

    // Can a Agent occupy the same square as this actor?
    virtual bool allowsAgentColocation() const;

    // Can a marble occupy the same square as this actor?
    virtual bool allowsMarble() const;

    // Does this actor count when a factory counts items near it?
    virtual bool countsInFactoryCensus() const;

    // Does this actor stop peas from continuing?
    virtual bool stopsPea() const;

    // Can this actor be damaged by peas?
    virtual bool isDamageable() const;

    // Cause this Actor to sustain damageAmt hit points of damage.
    virtual void damage(int damageAmt);

    // Can this actor be swallowed by a pit?
    virtual bool isSwallowable() const;

    // Can this actor be picked up by a ThiefBot?
    virtual bool isStealable() const;
    virtual void setStealable(bool b);

    // How many hit points does this actor have left?
    virtual int getHitPoints() const;

    // Set this actor's hit points to amt.
    virtual void setHitPoints(int amt);

    // Make the actor sustain damage.  Return true if this kills the
    // actor, and false otherwise.

    virtual bool playerMove(int x, int y, int dir);
    
 
    int health() const;

    int getx() const;
    int gety() const;

private:
    StudentWorld* myWorld;
    bool myAlive;
    int myHealth;
    bool myColocation;
    bool myAllowsMarble;
    bool myFactoryCount;
    bool myStopsPeas;
    bool myDamageable;
};

class Agent : public Actor
{
public:
    Agent(int startX, int startY, int imageID, StudentWorld* world, int hitPoints, int startDir);


    // Return true if this agent can push marbles (which means it's the
    // player).
    virtual bool canPushMarbles() const;

    // Return true if this agent doesn't shoot unless there's an unobstructed
    // path to the player.
    virtual bool needsClearShot() const;

    virtual bool isDamageable() const;
    virtual void damage(int damageAmt);
};

class Player : public Agent
{
public:
    Player(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual bool canPushMarbles() const;
    virtual void damage(int damageAmt);

    // Get player's health percentage
    int getHealthPct() const;

    // Get player's amount of ammunition
    int getAmmo() const;

    // Restore player's health to the full amount.
    void restoreHealth();

    // Increase player's amount of ammunition.
    void increaseAmmo();
private:
    int myAmmo;
};

//Robot Robot
class Robot : public Agent
{
public:
    Robot(StudentWorld* world, int startX, int startY, int imageID,
        int hitPoints, int score, int startDir,int tickRate);
    ~Robot();
    virtual void doSomething() const;
    virtual bool needsClearShot() const;
    virtual void damage(int damageAmt);

    // Does this robot shoot?
    virtual bool isShootingRobot() const;
    int tick() const;
    void addTick();
    void setTick(int n);
    int tickRate() const;
private:
    int myScore;
    int myTick;
    int myTickRate;
};

//Rage bot 

class RageBot : public Robot
{
public:
    RageBot(StudentWorld* world, int startX, int startY, int startDir,int tickRate);
    virtual void doSomething();
};



//Thief Bot Theif Bot
class ThiefBot : public Robot
{
public:
    ThiefBot(StudentWorld* world, int startX, int startY, int imageID,
        int hitPoints, int score, int tickRate);
    ~ThiefBot();
    virtual void doSomething();
    virtual bool countsInFactoryCensus() const;
    virtual void damage(int damageAmt);
protected:
    Actor* getGoodie() const;
private:
    int disBefTurnCalc();
    int randomDir(int d1,int d2, int d3);
    int disBefTurn;
    int distance;
    Actor* stolenGoodie;
};


class RegularThiefBot : public ThiefBot
{
public:
    RegularThiefBot(StudentWorld* world, int startX, int startY, int tickRate);
    bool isShootingRobot() const;
private:
};

class MeanThiefBot : public ThiefBot
{
public:
    MeanThiefBot(StudentWorld* world, int startX, int startY, int tickRate);
};

//EXIT EXIT

class Exit : public Actor
{
public:
    Exit(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual bool allowsAgentColocation() const;
};

class Wall : public Actor
{
public:
    Wall(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual bool stopsPea() const;
};

class Marble : public Actor
{
public:
    Marble(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual bool playerMove(int x, int y, int dir);
    virtual bool isDamageable() const;
    virtual void damage(int damageAmt);
    virtual bool isSwallowable() const;
};

class Pit : public Actor
{
public:
    Pit(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual bool allowsMarble() const;
    virtual bool stopsPea() const;
};

class Pea : public Actor
{
public:
    Pea(StudentWorld* world, int startX, int startY, int startDir);
    virtual void doSomething();
    virtual bool allowsAgentColocation() const;
};

class ThiefBotFactory : public Actor
{
public:
    enum ProductType { REGULAR, MEAN };

    ThiefBotFactory(StudentWorld* world, int startX, int startY, ProductType type);
    virtual void doSomething();
private:
    ProductType myProduct;
};

class PickupableItem : public Actor
{
public:
    PickupableItem(StudentWorld* world, int startX, int startY, int imageID,
        int score);
    virtual void doSomething();
    virtual bool allowsAgentColocation() const;
    virtual bool stopsPea() const;
    ~PickupableItem();
private:
    int myScore;
};

class Crystal : public PickupableItem
{
public:
    Crystal(StudentWorld* world, int startX, int startY);
    ~Crystal();
};

class Goodie : public PickupableItem
{
public:
    Goodie(StudentWorld* world, int startX, int startY, int imageID,
        int score);
    ~Goodie();
    virtual void doSomething();
    virtual bool isStealable() const;
    virtual bool allowsMarble() const;
    // Set whether this goodie is currently stolen.
    virtual void setStealable(bool b);
private:
    bool myStealable;
};

class ExtraLifeGoodie : public Goodie
{
public:
    ExtraLifeGoodie(StudentWorld* world, int startX, int startY);
    ~ExtraLifeGoodie();
};

class RestoreHealthGoodie : public Goodie
{
public:
    RestoreHealthGoodie(StudentWorld* world, int startX, int startY);
    ~RestoreHealthGoodie();
};

class AmmoGoodie : public Goodie
{
public:
    AmmoGoodie(StudentWorld* world, int startX, int startY);
    ~AmmoGoodie();
};

#endif // ACTOR_H_

