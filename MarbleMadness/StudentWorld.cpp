#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

const int NUM_TICKS = 28;
const int HORIZ_RAGEBOT_DIR = 0;
const int VERT_RAGEBOT_DIR = 90;
const int BONUS_SCORE = 1000;
const int COMPLETE_LEVEL_POINTS = 2000;


GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}


StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath), strLevel(""), lev(assetPath), myFinished(false), numCrystals(0), bonusScore(BONUS_SCORE), myPlayer_(nullptr)
{

}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    int intLevel = getLevel();
    strLevel = "level";

    if (intLevel < 0 || intLevel > 99)
        return GWSTATUS_LEVEL_ERROR;
    else if (intLevel < 10)
        strLevel += "0";
    strLevel += to_string(intLevel);
    strLevel += ".txt";
    numCrystals = 0;

    Level::LoadResult result = lev.loadLevel(strLevel);

    if (result == Level::load_fail_file_not_found ||
        result == Level::load_fail_bad_format)
        return -1;
    for (int x = 0; x < VIEW_WIDTH; ++x)
        for (int y = 0; y < VIEW_HEIGHT; ++y)
        {
            Level::MazeEntry item = lev.getContentsOf(x, y);
            if (item == Level::wall)
                myActors_.push_back(new Wall(this, x, y));
            else if (item == Level::marble)
               myActors_.push_front(new Marble(this, x, y));
            else if (item == Level::pit)
                myActors_.push_back(new Pit(this, x, y));
            else if (item == Level::horiz_ragebot)
                myActors_.push_front(new RageBot(this,x,y, HORIZ_RAGEBOT_DIR, (NUM_TICKS - intLevel)/4));
            else if (item == Level::vert_ragebot)
                myActors_.push_front(new RageBot(this, x, y, VERT_RAGEBOT_DIR, (NUM_TICKS - intLevel) / 4));
            else if (item == Level::crystal)
            {
                myActors_.push_back(new Crystal(this, x, y));
                ++numCrystals;
            }
            else if (item == Level::thiefbot_factory)
                myActors_.push_back(new ThiefBotFactory(this,x,y, ThiefBotFactory::REGULAR));
            else if (item == Level::mean_thiefbot_factory)
                myActors_.push_back(new ThiefBotFactory(this, x, y, ThiefBotFactory::MEAN));
            else if (item == Level::ammo)
                myActors_.push_back(new AmmoGoodie(this, x, y));
            else if (item == Level::extra_life)
                myActors_.push_back(new ExtraLifeGoodie(this, x, y));
            else if (item == Level::restore_health)
                myActors_.push_back(new RestoreHealthGoodie(this, x, y));
            else if (item == Level::exit)
                myActors_.push_back(new Exit(this, x, y));
            else if (item == Level::player)
                myPlayer_ = new Player(this,x, y);
        }

    bonusScore = BONUS_SCORE;
    myFinished = false;

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you type q

    myPlayer_->doSomething();
    for (list<Actor*>::iterator it = myActors_.begin(); it != myActors_.end(); ++it)
    {
        (*it)->doSomething();
        if (!myPlayer_->isAlive())
        {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
    }
    for (list<Actor*>::iterator it = myActors_.begin(); it != myActors_.end();)
    {
        if (!(*it)->isAlive())
        {
            delete (*it);
            it = myActors_.erase(it);
        }
        else
            ++it;
    }
    
    if (myFinished)
    {
        increaseScore(COMPLETE_LEVEL_POINTS);
        increaseScore(bonusScore);
        return  GWSTATUS_FINISHED_LEVEL;
    }
    if (bonusScore)
        --bonusScore;
    display();
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete myPlayer_;
    myPlayer_ = nullptr;
    for (list<Actor*>::iterator it = myActors_.begin(); it != myActors_.end();)
    {
        delete* it;
        it = myActors_.erase(it);
    }
}

void StudentWorld::direction(int& x, int& y, int dir)
{
    switch (dir / 90)
    {
    case 0:
        ++x;
        break;
    case 1:
        ++y;
        break;
    case 2:
        --x;
        break;
    case 3:
        --y;
        break;
    }
}

Actor* StudentWorld::actorAt(int x, int y,Actor* same)
{
    for (list<Actor*>::iterator it = myActors_.begin(); it != myActors_.end(); ++it)
        if ((*it) != same && (*it)->getx() == x && (*it)->gety() == y)
            return *it;
    return nullptr;
}

Actor* StudentWorld::actorAt(int x, int y)
{
    for (list<Actor*>::iterator it = myActors_.begin(); it != myActors_.end(); ++it)
        if ((*it)->getx() == x && (*it)->gety() == y)
            return *it;
    return nullptr;
}

Actor* StudentWorld::actorAt(int x, int y, int dir)
{
    direction(x, y, dir);
    for (list<Actor*>::iterator it = myActors_.begin(); it != myActors_.end(); ++it)
        if ((*it)->getx() == x && (*it)->gety() == y)
            return *it;
    return nullptr;
}

bool StudentWorld::playerAt(int x, int y, int dir)
{
    direction(x, y, dir);
    if (myPlayer_->getx() == x && myPlayer_->gety() == y)
        return true;
    return false;
}

bool StudentWorld::playerAt(int x, int y)
{
    if (myPlayer_->getx() == x && myPlayer_->gety() == y)
        return true;
    return false;
}

bool StudentWorld::canAgentMoveTo(Agent* agent, int x, int y, int dir) const
{
    return true;
}

bool StudentWorld::canMarbleMoveTo(int x, int y, int dir)
{
    int x2 = x, y2 = y;
    direction(x, y, dir);
    Actor* tempActor = actorAt(x, y);
    if (tempActor == nullptr)
        return true;
    else if (tempActor->allowsMarble())
    {
        //// During one of the OH one of the TAs said something where using the name of an actor is not allowed anywhere I forgot
        //// What was actually said, but Carey said this was okay so yeah.....
        //if (lev.getContentsOf(x, y) == Level::pit)
        //{
        //    tempActor->setAlive(false);
        //    tempActor->setVisible(false);
        //    tempActor = actorAt(x2, y2);
        //    tempActor->setAlive(false);
        //    tempActor->setVisible(false);
        //}
        return true;
    }
    return false;
}

Player* StudentWorld::player() const
{
    return myPlayer_;
}

void StudentWorld::shoot(int x, int y, int dir)
{
    myActors_.push_back(new Pea(this, x, y, dir));
}

void StudentWorld::display()
{
    ostringstream output;
    output << "Score: " << setw(7) << setfill('0') << getScore();
    output << "  Level: " << setw(2) << setfill('0') << getLevel();
    output << "  Lives: " << setw(2) << setfill(' ') << getLives();
    output << "  Health: " << setw(3) << myPlayer_->getHealthPct();
    output << "%  Ammo: " << setw(3) << myPlayer_->getAmmo();
    output << "  Bonus: " << setw(4) << bonusScore;
    setGameStatText(output.str());
}

int StudentWorld::thiefBotCount(int xfact, int yfact)
{
    int count = 0;
    for (int x = xfact - 3; x <= xfact + 3; ++x)
        for (int y = yfact - 3; y <= yfact + 3; ++y)
        {
            Actor* temp = actorAt(x, y);
            if (temp != nullptr && temp->countsInFactoryCensus())
                ++count;
        }
    return count;
}

void StudentWorld::newThiefBot(int x, int y, ThiefBotFactory::ProductType type)
{
    if (type == ThiefBotFactory::REGULAR)
        myActors_.push_front(new RegularThiefBot(this, x, y, ( (NUM_TICKS - getLevel() ) / 4)));
    if (type == ThiefBotFactory::MEAN)
        myActors_.push_front(new MeanThiefBot(this, x, y, ((NUM_TICKS - getLevel()) / 4)));
}

bool StudentWorld::anyCrystals() const
{
    if (numCrystals <= 0)
        return false;
    else
        return true;
}
void StudentWorld::decCrystals()
{
    --numCrystals;
}
void  StudentWorld::incCrystals()
{
    ++numCrystals;
}
void StudentWorld::setLevelFinished()
{
    myFinished = true;
    playSound(SOUND_FINISHED_LEVEL);
}
void StudentWorld::addActor(Actor* a)
{
    return;
}