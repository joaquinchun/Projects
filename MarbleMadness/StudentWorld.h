#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "Level.h"
#include <list>

//class Actor;
//class Agent;
//class Player;



class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();

    // Can agent move to x,y?  (dx and dy indicate the direction of motion)
    bool canAgentMoveTo(Agent* agent, int x, int y, int dir) const;

    // Can a marble move to x,y?
    bool canMarbleMoveTo(int x, int y,int dir);

    // Is the player on the same square as an Actor?

    // Are there any crystals left on this level?
    bool anyCrystals() const;

    // Reduce the count of crystals on this level by 1.
    void decCrystals();

    void incCrystals();

    // Indicate that the player has finished the level.
    void setLevelFinished();

    // Add an actor to the world
    void addActor(Actor* a);

    void direction(int& x, int& y, int dir);
    Actor* actorAt(int x, int y,Actor* same);
    Actor* actorAt(int x, int y);
    Actor* actorAt(int x, int y, int dir);
    bool playerAt(int x, int y, int dir);
    bool playerAt(int x, int y);
    Player* player() const;
    void shoot(int x, int y, int dir);
    void display();
    int thiefBotCount(int x, int y);
    void newThiefBot(int x, int y, ThiefBotFactory::ProductType type);
private:
    std::string strLevel;
    Player* myPlayer_;
    std::list<Actor*> myActors_;
    Level lev;
    int bonusScore;
    int numCrystals;
    bool myFinished;
};

#endif // STUDENTWORLD_H_