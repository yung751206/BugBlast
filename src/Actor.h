#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"
#include "GameConstants.h"
#include "StudentWorld.h"

class StudentWorld;
class ActorObject : public GraphObject
{
  public:
	ActorObject(StudentWorld* studentworld,int imageID,int startX, int startY):GraphObject(imageID,startX,startY)
	{
		m_studentWorld = studentworld;             //Set the studentworld the actorObject is in
		m_alive = true;                            //Every object is alive initially
		setVisible(true);                          //Every object except exit is visible initially
	}
	virtual ~ActorObject()
	{setVisible(false);}                           //If delete the object, the object is invisible
	virtual void doSomething() = 0;                //Pure vitrual function 

	//Get and set private data members
	void setDead()
	{m_alive = false;}
	bool isAlive()
	{return m_alive;}
	StudentWorld* getWorld()
	{return m_studentWorld;}

  private:
	StudentWorld* m_studentWorld;
	bool m_alive;
};

class Exit:public ActorObject
{
public:
	Exit(StudentWorld* studentworld,int startX, int startY):ActorObject(studentworld,IID_EXIT,startX,startY)
	{
		setVisible(false);          //It's not visible unless the there is no zumi in this level
	}
	virtual void doSomething();    //Override doSomething function
private:
};

class ActorwithTick:public ActorObject
{
public:
	ActorwithTick(StudentWorld* studentworld,int imageID,int startX, int startY,int tick):ActorObject(studentworld,imageID,startX,startY)
	{m_tick = tick;}                       //Initialize tick
	virtual void doSomething() = 0;        //Pure virtual function
	void setTick(int tick)                 //Set tick and get tick
	{m_tick = tick;}
	int getTick() const
	{return m_tick;}

private:
	int m_tick;
};

class Goodie:public ActorwithTick   //Inherit actorwithTick class because goodie has tick
{
  public:
	Goodie(StudentWorld* world,int IID,int startX,int startY,int tick):ActorwithTick(world,IID,startX,startY,tick)
	{;}
  private:
};

class ExtraLifeGoodie:public Goodie //Inherit ExtraLifeGoodie class because is kind of goodie
{
  public:
	ExtraLifeGoodie(StudentWorld* world,int startX,int startY,int tick):Goodie(world,IID_EXTRA_LIFE_GOODIE,startX,startY,tick) ////Set image id as IID_EXTRA_LIFE_GOODIE
	{;}
	virtual void doSomething();
  private:
};

class WalkThroughWallsGoodie:public Goodie //Inherit WalkThroughWallsGoodie class because is kind of goodie
{
  public:
	WalkThroughWallsGoodie(StudentWorld* world,int startX,int startY,int tick):Goodie(world,IID_WALK_THRU_GOODIE,startX,startY,tick) //Set image id as IID_WALK_THRU_GOODIE
	{;}
	virtual void doSomething();
  private:
};

class IncreaseSprayerGoodie:public Goodie  //Inherit IncreaseSprayerGoodie class because is kind of goodie
{
  public:
	IncreaseSprayerGoodie(StudentWorld* world,int startX,int startY,int tick):Goodie(world,IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE,startX,startY,tick) //Set image id as IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE
	{;}
	virtual void doSomething();
  private:
};
 
class Sprayer:public ActorwithTick        //Inherit actorwithTick class because sprayer has tick
{
  public :
	Sprayer(StudentWorld* world,int IID,int startX,int startY,int tick):ActorwithTick(world,IID,startX,startY,tick)
	{;}
	virtual void doSomething();

  private:
	void creatSpray(int x,int y);         //When the tick reaches zero, the sprayer produces spray
};

class Spray:public Sprayer
{
public:
	Spray(StudentWorld* world,int startX,int startY,int tick):Sprayer(world,IID_BUGSPRAY,startX,startY,tick)  //Set image id as IID_BUGSPRAY
	{;}
	virtual void doSomething(); //Override doSomehting function

private:
	void checkLocation();          //Private function to check any actor in the same position as spray
};

class Player : public ActorObject
{
  public:
	Player(StudentWorld* world,int startX,int startY):ActorObject(world,IID_PLAYER,startX,startY)  //Set image id as IID_PLAYER
	{
		numofSprayers = 2;                                        //The number of sprayers can be dropped
		countSprayers = 0;                                        //Sprayer counter
		m_hasWalkThroughWallGoodie = false;                       //Don't have any goodie
		m_hasIncreaseSprayerGoodie = false;
	}
	virtual void doSomething();                    //Override doSomething method

	//The following function are used to set and get private data members
	void setNumofSprayers(int num) 
	{numofSprayers = num;}
	void setCountSprayers(int count)
	{countSprayers = count;}
	int getCountofSprayers() const
	{return countSprayers;}
	void setWalkThroughWallGoodie(bool goodie)
	{m_hasWalkThroughWallGoodie = goodie;}
	bool hasWalkThroughWallGoodie()
	{return m_hasWalkThroughWallGoodie;}
	void setWalkThruGoodieTick(int tick)
	{m_walkThruGoodieTick = tick;}
	int getWalkThruGoodieTick() const
	{return m_walkThruGoodieTick;}
	void setIncreaseSprayerGoodie(bool goodie)
	{m_hasIncreaseSprayerGoodie = goodie;}
	bool hasIncreaseSprayerGoodie()
	{return m_hasIncreaseSprayerGoodie;}
	void setIncreaseSprayerGoodieTick(int tick)
	{m_increseSprayerTick = tick;}
	int getIncreaseSprayerGoodieTick() const
	{return m_increseSprayerTick;}

  private:
	  int numofSprayers,countSprayers;
	  int m_walkThruGoodieTick,m_increseSprayerTick;
	  bool m_hasWalkThroughWallGoodie,m_hasIncreaseSprayerGoodie;
};


class Brick : public ActorObject            
{
  public:
	  Brick(StudentWorld* world,int imageID,int startX, int startY):ActorObject(world,imageID,startX,startY)
	  {}
	  virtual void doSomething()               //do nothing
	  {}
};

class PermaBrick : public Brick
{
  public:
	  PermaBrick(StudentWorld* world,int startX,int startY):Brick(world,IID_PERMA_BRICK,startX,startY)   //Set image id as IID_PERMA_BRICK
	  {}
  private:
};

class DestroyableBrick : public Brick
{
  public:
	  DestroyableBrick(StudentWorld* world,int startX,int startY):Brick(world,IID_DESTROYABLE_BRICK,startX,startY) //Set image id as IID_DESTROYABLE_BRICK
	  {}
  private:
};

class SimpleZumi:public ActorwithTick                       //Inherit actorwithTick class because zumi has tick               
{
  public:
	  SimpleZumi(StudentWorld* world,int IID,int startX,int startY,int tick):ActorwithTick(world,IID,startX,startY,tick)
	  {
		  currentDirection = rand()%4;     //randomly choose a currentDirection - up, down, left or right
	  }
	  virtual ~SimpleZumi()
	  {
		  int temp = rand()%100 ;         //When zumi dies, it has the chance to drop the goodie
		  if( temp < (getWorld()->getProbOfGoodieOverall()) ) //If temp value is bigger than the probability of droping goodie
			createGoodie(getX(),getY());                      //then it will drop a goodie. This function is uesd to determine what 
	  }                                                       // kind of goodie the zumi dropped

	  virtual void doSomething();
	  int getCurrentDirection() const
	  {
		  return currentDirection;
	  }

	  void setCurrentDirection(int direction)
	  {
		  currentDirection = direction;
	  }
	  void createGoodie(int x,int y);


  private:
	  int currentDirection;
};

class ComplexZumi:public SimpleZumi  //Inherit ComplexZumi class because is kind of simple zumi
{
public:
	ComplexZumi(StudentWorld* world,int startX,int startY,int tick):SimpleZumi(world,IID_COMPLEX_ZUMI ,startX,startY,tick)
	{
		smellDistance = getWorld()->getComplexZumiSearchDistance();  //Set the smellDistance of complex zumi
	}


	virtual void doSomething();
private:
	bool searchAlgorithm(int &x,int &y);          //The searching algorithm for complex zumi
	int smellDistance;

};

#endif // ACTOR_H_