#include "Actor.h"
#include "StudentWorld.h"

const int PLAYER_INITIAL_HEALTH = 20;
const int NON_PLAYER_INITIAL_HEALTH = 10;
const int PEA_DAMAGE = 2;
const int AMMO_INCREASE = 20;
const int RAGEBOT_HP = 10;
const int RAGEBOT_PTS = 100;

//ACTOR ACTOR
Actor::Actor(int startX, int startY, int imageID, StudentWorld* world, int hitPoints, int startDir)
	: GraphObject(imageID,startX,startY,startDir), myHealth(hitPoints),myWorld(world)
{
	setDirection(startDir);
	setVisible(true);
	setAlive(true);
}


bool Actor::isAlive() const
{
	return myAlive;
}

void Actor::setAlive(bool b)
{
	myAlive = b;
}

void Actor::decHitPoints(int amt)
{
	myHealth -= amt;
}

StudentWorld* Actor::getWorld() const
{
	return myWorld;
}

bool Actor::allowsAgentColocation() const
{
	return false;
}
bool Actor::allowsMarble() const
{
	return false;
}
bool Actor::countsInFactoryCensus() const
{
	return false;
}
bool Actor::stopsPea() const
{
	return true;
}
bool Actor::isDamageable() const
{
	return false;
}
void Actor::damage(int damageAmt)
{
	return;
}

bool Actor::isSwallowable() const
{
	return false;
}
bool Actor::isStealable() const
{
	return false;
}
void Actor::setStealable(bool b)
{
	return;
}
int Actor::getHitPoints() const
{
	return false;
}
void Actor::setHitPoints(int amt)
{
	myHealth = amt;
}

bool Actor::playerMove(int x, int y, int dir)
{
	return false;
}

int Actor::health() const
{
	return myHealth;
}

int Actor::getx() const
{
	return (getX() + .4999999);
}
int Actor::gety() const
{
	return (getY() + .4999999);
}

//AGENT AGENT

Agent::Agent(int startX, int startY, int imageID, StudentWorld* world, int hitPoints, int startDir)
	: Actor(startX, startY, imageID, world, hitPoints, startDir)
{

}

bool Agent::canPushMarbles() const
{
	return false;
}
bool Agent::needsClearShot() const
{
	return false;
}

bool Agent::isDamageable() const
{
	return true;
}
void Agent::damage(int damageAmt)
{
	decHitPoints(PEA_DAMAGE);
	if (health() <= 0)
	{
 		setAlive(false);
		setVisible(false);
	}
}

//Player Player

Player::Player(StudentWorld* world, int startX, int startY)
	: Agent(startX, startY, IID_PLAYER, world, PLAYER_INITIAL_HEALTH, right), myAmmo(20)
{

}
void Player::doSomething()
{
	if (!isAlive())
		return;
	int key;
	if (getWorld()->getKey(key))
	{
		int x = getx();
		int y = gety();
		switch (key)
		{
		case KEY_PRESS_SPACE:
			if (myAmmo > 0)
			{
				getWorld()->shoot(x, y, getDirection());
				getWorld()->playSound(SOUND_PLAYER_FIRE);
				--myAmmo;
			}
			return;
			break;
		case KEY_PRESS_ESCAPE:
			setHitPoints(0);
			setAlive(false);
			return;
			break;
		case KEY_PRESS_LEFT:
			setDirection(left);
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			break;
		}
		getWorld()->direction(x, y, getDirection());
		Actor* temp = getWorld()->actorAt(x, y);
		if (temp == nullptr || temp->playerMove(x, y, getDirection()) || temp->allowsAgentColocation())
		{
			moveAngle(getDirection());
		}
	}
}

bool Player::canPushMarbles() const
{
	return false;
}


void Player::damage(int damageAmt)
{
	Agent::damage(damageAmt);
	if (!isAlive())
		getWorld()->playSound(SOUND_PLAYER_DIE);
	else
		getWorld()->playSound(SOUND_PLAYER_IMPACT);
}

// Get player's health percentage
int Player::getHealthPct() const
{
	return health() * 5;
}

// Get player's amount of ammunition
int Player::getAmmo() const
{
	return myAmmo;
}

// Restore player's health to the full amount.
void Player::restoreHealth()
{
	return;
}

// Increase player's amount of ammunition.
void Player::increaseAmmo()
{
	myAmmo += AMMO_INCREASE;
}

//RObot Robot ROBOT
Robot::Robot(StudentWorld* world, int startX, int startY, int imageID,
	int hitPoints, int score, int startDir,int tickRate)
	: Agent(startX,startY,imageID,world,hitPoints,startDir), myScore(score),myTickRate(tickRate),myTick(0)
{
	if (tickRate < 3)
		myTickRate = 3;
}
void Robot::doSomething() const
{
	return;
}
bool Robot::needsClearShot() const
{
	StudentWorld* world = getWorld();
	int x = getx(), y = gety(), dir = getDirection();
	int x2 = x;
	int y2 = y;
	while (world->actorAt(x, y, dir) == nullptr || !world->actorAt(x, y, dir)->stopsPea())
	{
		if (world->playerAt(x, y, dir))
		{
			world->shoot(x2, y2, dir);
			world->playSound(SOUND_ENEMY_FIRE);
			return true;
		}
		world->direction(x, y, dir);
	}
	return false;
}


// Does this robot shoot?
bool Robot::isShootingRobot() const
{
	return true;
}

void Robot::damage(int damageAmt)
{
	Agent::damage(damageAmt);
	if (isAlive())
		getWorld()->playSound(SOUND_ROBOT_IMPACT);
}

Robot::~Robot()
{
	StudentWorld* world = getWorld();
	if (world->player() != nullptr)
	{
		world->playSound(SOUND_ROBOT_DIE);
		world->increaseScore(myScore);
	}
}

int Robot::tick() const
{
	return myTick;
}
void Robot::addTick()
{
	++myTick;
}
void Robot::setTick(int n)
{
	myTick = n;
}
int Robot::tickRate() const
{
	return myTickRate;
}

//Rage Bot Rage Bot
RageBot::RageBot(StudentWorld* world, int startX, int startY, int startDir,int tickRate)
	: Robot(world,startX,startY,IID_RAGEBOT, RAGEBOT_HP, RAGEBOT_PTS,startDir,tickRate)
{

}
void RageBot::doSomething()
{
	if (!isAlive())
		return;
	else if (tick() < tickRate())
	{
		addTick();
		return;
	}
	setTick(1);
	int dir = getDirection();
	if (Robot::needsClearShot())
		return;
	Actor* temp = getWorld()->actorAt(getx(), gety(), dir);
	if (temp == nullptr || temp->allowsAgentColocation())
		moveAngle(dir);
	else
		switch (dir)
		{
		case right:
			setDirection(left);
			break;
		case left:
			setDirection(right);
			break;
		case up:
			setDirection(down);
			break;
		case down:
			setDirection(up);
			break;
		}
}

//THIEF BOT THIEF BOT
ThiefBot::ThiefBot(StudentWorld* world, int startX, int startY, int imageID, int hitPoints, int score,int tickRate)
	: Robot(world,startX,startY,imageID,hitPoints,score,right,tickRate), stolenGoodie(nullptr), distance(0)
{
	disBefTurn = disBefTurnCalc();
}

ThiefBot::~ThiefBot()
{
	
}

bool ThiefBot::countsInFactoryCensus() const
{
	return true;
}

int ThiefBot::disBefTurnCalc()
{
	return randInt(1, 6);
}

int ThiefBot::randomDir(int d1, int d2, int d3)
{
	int returnDir;
	do
	{
		switch (randInt(0, 3))
		{
		case 0:
			returnDir = right;
			break;
		case 1:
			returnDir = up;
			break;
		case 2:
			returnDir = left;
			break;
		case 3:
			returnDir = down;
			break;
		}
	} while (returnDir == d1 || returnDir == d2 || returnDir == d3);
	
	return returnDir;
}
void ThiefBot::doSomething()
{
	if (!isAlive())
	{
		return;
	}
	else if (tick() < tickRate())
	{
		addTick();
		return;
	}
	setTick(1);
	if (isShootingRobot())
		if (Robot::needsClearShot())
			return;
	StudentWorld* world = getWorld();
	int x = getx(), y = gety();
	Actor* actorTemp = world->actorAt(x, y,this);

	if (actorTemp != nullptr && actorTemp->isStealable() && stolenGoodie == nullptr)
	{
		if (randInt(1, 10) == 10)
		{
			stolenGoodie = actorTemp;
			stolenGoodie->setStealable(false);
			world->playSound(SOUND_ROBOT_MUNCH);
			return;
		}
	}


	actorTemp = world->actorAt(x, y, getDirection());
	if (distance < disBefTurn && !world->playerAt(x,y,getDirection()) && (actorTemp == nullptr || actorTemp->allowsAgentColocation()))
	{
		moveAngle(getDirection());
		++distance;
		return;
	}
	
	distance = 0;
	disBefTurn = disBefTurnCalc();
	int dir1 = randomDir(-1, -1, -1);
	actorTemp = world->actorAt(x, y, dir1);
	if (!world->playerAt(x, y, dir1) && (actorTemp == nullptr || actorTemp->allowsAgentColocation()) )
	{
		setDirection(dir1);
		moveAngle(dir1);
		++distance;
		return;
	}

	int dir2 = randomDir(dir1, -1, -1);
	actorTemp = world->actorAt(x, y, dir2);
	if (!world->playerAt(x, y, dir2) && (actorTemp == nullptr || actorTemp->allowsAgentColocation()))
	{
		setDirection(dir2);
		moveAngle(dir2);
		++distance;
		return;
	}

	int dir3 = randomDir(dir1, dir2, -1);
	actorTemp = world->actorAt(x, y, dir3);
	if (!world->playerAt(x, y, dir3) && (actorTemp == nullptr || actorTemp->allowsAgentColocation()))
	{
		setDirection(dir3);
		moveAngle(dir3);
		++distance;
		return;
	}

	int dir4 = randomDir(dir1, dir2, dir3);
	actorTemp = world->actorAt(x, y, dir4);
	if (!world->playerAt(x, y, dir4) && (actorTemp == nullptr || actorTemp->allowsAgentColocation()))
	{
		setDirection(dir4);
		moveAngle(dir4);
		++distance;
		return;
	}

	setDirection(dir1);

}

Actor* ThiefBot::getGoodie() const
{
	return stolenGoodie;
}

void ThiefBot::damage(int damageAmt)
{
	decHitPoints(PEA_DAMAGE);
	if (health() <= 0)
	{
		setAlive(false);
		setVisible(false);
		if (stolenGoodie != nullptr)
		{
			stolenGoodie->moveTo(getx(), gety());
			stolenGoodie->setVisible(true);
			stolenGoodie->setStealable(true);
		}
	}
}
//REGULAR THEIF BOT REGULAR THEIF B0T
RegularThiefBot::RegularThiefBot(StudentWorld* world, int startX, int startY,int tickRate)
	: ThiefBot(world,startX,startY,IID_THIEFBOT,5,10, tickRate)
{

}


bool RegularThiefBot::isShootingRobot() const
{
	return false;
}

//MEAN THEIF BOT MEAN THEIF BOT

MeanThiefBot::MeanThiefBot(StudentWorld* world, int startX, int startY, int tickRate) 
	: ThiefBot(world,startX,startY,IID_MEAN_THIEFBOT,8,20,tickRate)
{

}


//EXIT EXIT
Exit::Exit(StudentWorld* world, int startX, int startY)
	: Actor(startX,startY,IID_EXIT,world)
{
	setVisible(false);
}
void Exit::doSomething()
{
	if (!getWorld()->anyCrystals())
	{
		setVisible(true);
		getWorld()->playSound(SOUND_REVEAL_EXIT);
		getWorld()->incCrystals();
	}
	if (isVisible() && getWorld()->playerAt(getx(), gety()))
		getWorld()->setLevelFinished();
}
bool Exit::allowsAgentColocation() const
{
	return true;
}

//WALL WALL WALL
Wall::Wall(StudentWorld* world, int startX, int startY)
	: Actor(startX,startY,IID_WALL,world)
{

}
void Wall::doSomething()
{
	return;
}
bool Wall::stopsPea() const
{
	return true;
}

//Marble Marble Marble

Marble::Marble(StudentWorld* world, int startX, int startY)
	: Actor(startX, startY, IID_MARBLE, world, NON_PLAYER_INITIAL_HEALTH, -1)
{
	setHitPoints(NON_PLAYER_INITIAL_HEALTH);
}
void Marble::doSomething()
{
	return;
}
bool Marble::playerMove(int x, int y, int dir)
{
	if (getWorld()->canMarbleMoveTo(x, y, dir))
	{
		moveAngle(dir);
		return true;
	}
	else
		return false;
}

bool Marble::isDamageable() const
{
	return true;
}
void Marble::damage(int damageAmt)
{
	decHitPoints(PEA_DAMAGE);
	if (health() <= 0)
	{
		setAlive(false);
		setVisible(false);
	}
}

bool Marble::isSwallowable() const
{
	return true;
}

//Pit Pit Pit 

Pit::Pit(StudentWorld* world, int startX, int startY)
	: Actor(startX, startY, IID_PIT, world, 0, -1)
{

}
void Pit::doSomething()
{
	if (!isAlive())
	{
		return;
		setVisible(false);
	}
	Actor* temp = getWorld()->actorAt(getx(), gety());
	if (temp->isSwallowable())
	{
		setAlive(false);
		setVisible(false);
		temp->setAlive(false);
		temp->setVisible(false);

	}
}
bool Pit::allowsMarble() const
{
	return true;
}

bool Pit::stopsPea() const
{
	return false;
}

//PEA PEA
Pea::Pea(StudentWorld* world, int startX, int startY, int startDir)
	: Actor(startX,startY,IID_PEA,world,0,startDir)
{

}
void Pea::doSomething()
{
	if (!isAlive())
		return;
	int x = getx(), y = gety();
	StudentWorld* world = getWorld();


	if (world->playerAt(x, y, getDirection()))
	{
		world->player()->damage(PEA_DAMAGE);
		setAlive(false);
		return;
	}
	Actor* temp = world->actorAt(x, y, getDirection());
	if (temp == nullptr || !temp->stopsPea())
		moveAngle(getDirection());
	else
	{
		temp->damage(PEA_DAMAGE);
		setAlive(false);
		return;
	}


}
bool Pea::allowsAgentColocation() const
{
	return true;
}

// Theif Bot Factory
ThiefBotFactory::ThiefBotFactory(StudentWorld* world, int startX, int startY, ProductType type)
	: Actor(startX, startY, IID_ROBOT_FACTORY, world), myProduct(type)
{

}
void ThiefBotFactory::doSomething()
{
	StudentWorld* world = getWorld();
	int count = world->thiefBotCount(getx(), gety());
	if (count < 3 && world->actorAt(getx(), gety()) == this)
		if (randInt(1, 50) == 50)
		{
			world->newThiefBot(getx(), gety(), myProduct);
			world->playSound(SOUND_ROBOT_BORN);
		}
}
//Pickable Pickable 

PickupableItem::PickupableItem(StudentWorld* world, int startX, int startY, int imageID, int score)
	: Actor(startX, startY, imageID, world), myScore(score)
{
	
}
void PickupableItem::doSomething()
{
	if (getWorld()->playerAt(getx(), gety()))
	{
		setVisible(false);
		setAlive(false);
	}
}
bool PickupableItem::allowsAgentColocation() const
{
	return true;
}


bool PickupableItem::stopsPea() const
{
	return false;
}

PickupableItem::~PickupableItem()
{
	getWorld()->increaseScore(myScore);
}

//Crystal Crystal

Crystal::Crystal(StudentWorld* world, int startX, int startY)
	: PickupableItem(world,startX,startY,IID_CRYSTAL,50)
{

}
Crystal::~Crystal()
{
	getWorld()->decCrystals();
}

//Goodies Goodies

Goodie::Goodie(StudentWorld* world, int startX, int startY, int imageID, int score)
	: PickupableItem(world, startX, startY, imageID, score), myStealable(true)
{

}
Goodie::~Goodie()
{
	StudentWorld* world = getWorld();
	if (world->player() != nullptr)
	{
		world->playSound(SOUND_GOT_GOODIE);
	}
}
void Goodie::doSomething()
{
	if (!isAlive())
		return;
	if (!myStealable)
	{
		setVisible(false);
		return;
	}
	int x = getx(), y = gety();
	if (getWorld()->playerAt(x, y))
		setAlive(false);
}
bool Goodie::isStealable() const
{
	return myStealable;
}

// Set whether this goodie is currently stolen.
void Goodie::setStealable(bool b)
{
	myStealable = b;
}


bool Goodie::allowsMarble() const
{
	return !myStealable;
}


//Extra Life Extra Life
ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld* world, int startX, int startY)
	: Goodie(world,startX,startY,IID_EXTRA_LIFE,1000)
{

}
ExtraLifeGoodie::~ExtraLifeGoodie()
{
	Player* p = getWorld()->player();
	if (p != nullptr)
		getWorld()->incLives();
}

// RestoreHealthGoodie RestoreHealthGoodie
RestoreHealthGoodie::RestoreHealthGoodie(StudentWorld* world, int startX, int startY)
	: Goodie(world, startX, startY, IID_RESTORE_HEALTH, 500)
{

}
RestoreHealthGoodie::~RestoreHealthGoodie()
{
	Player* p = getWorld()->player();
	if (p != nullptr)
		p->setHitPoints(20);
}

// AMMO

AmmoGoodie::AmmoGoodie(StudentWorld* world, int startX, int startY)
	: Goodie(world,startX,startY,IID_AMMO,100)
{

}

AmmoGoodie::~AmmoGoodie()
{
	Player* p = getWorld()->player();
	if (p != nullptr)
		p->increaseAmmo();
}