#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include <vector>
#include "GameWorld.h"
#include "GameConstants.h"
#include "GameController.h"

class ActorObject;
class Player;


class StudentWorld : public GameWorld
{
  public:
	StudentWorld()                            //Constructor to initialize the private
	{										  //data member
		actorPlayer = nullptr;                
		playExitSound = true;				  //If exit is shown, then this flag will become false and play exit sound once.
		isFinished = false;					  //This flag is used to indicate whether the player win the current level or not.
		m_numOfZumi = 0;					  //Zumi counter
	}
	~StudentWorld()                             //Destructor to delete all dynamic data to prevent memory leak
	{
		std::vector<ActorObject*>::iterator itr;
		itr = actorVector.begin();
		while(itr!=actorVector.end())       //Check if beginning address is equal to end address
		{
			delete *itr;                    //delete pointer which is created by new command
			actorVector.erase(itr);			//erase the data in the itr address
			itr = actorVector.begin();		//Address of vector changes so assign itr to the beginning address of the vector
		}
	delete actorPlayer;
	}

	virtual int init();						//Implement pure virtual function
	virtual int move();
	virtual void cleanUp();

	void updateDisplayText();               //Using this function to update the display text

	//The following function are used to check who is who at the position specified by x and y
	bool isPermBrick(int x,int y);
	bool isDesBrick(int x,int y);
	bool isSprayer(int x,int y);
	bool isSpray(int x,int y);
	bool isPlayer(int x,int y);
	bool isSimpleZumi(int x,int y);
	bool isComplexZumi(int x,int y);
	bool isExit(int x,int y);

	//bool anyZumi();							//Check if there is any Zumi in the game world
	void removeDeadActors();				//Using this function to remove all the dead actor

	void setIsFinished(bool set)			//Set the finished flag
	{isFinished = set;}

	//Using the following functions to get and set the optionvalues in the level data file
	int getBonus() const
	{return m_bonus;}
	void setBonus(int bonus)
	{m_bonus = bonus;}
	int getGoodieTick() const
	{return m_goodieTick;}
	void setGoodieTick(int tick)
	{m_goodieTick = tick;}
	int getwalkThruLifetimeTick() const
	{return m_walkThruLifetimeTick;}
	void setwalkThruLifetimeTick(int tick)
	{m_walkThruLifetimeTick = tick;}
	int getIncreaseSprayerTimeTick() const
	{return m_increaseSprayerGoodieTick;}
	void setIncreaseSprayerTimeTick(int tick)
	{m_increaseSprayerGoodieTick = tick;}
	int getMaxBoostedSprayer() const
	{return m_maxBoostedSprayer;}
	void setMaxBoostedSprayer(int maxSprayers)
	{m_maxBoostedSprayer = maxSprayers;}
	int getProbOfGoodieOverall() const
	{return m_probOfGoodieOverall;}
	void setProbOfGoodieOverall(int prob)
	{m_probOfGoodieOverall = prob;}
	int getProbOfExtraLifeGoodie() const
	{return m_probOfExtraLifeGoodie;}
	void setProbOfExtraLifeGoodie(int prob)
	{m_probOfExtraLifeGoodie = prob;}
	int getProbOfWalkThruGoodie() const
	{return m_probOfWalkThruGoodie;}
	void setProbOfWalkThruGoodie(int prob)
	{m_probOfWalkThruGoodie = prob;}
	int getProbOfMoreSprayersGoodie() const
	{return m_probOfMoreSprayersGoodie;}
	void setProbOfMoreSprayersGoodie(int prob)
	{m_probOfMoreSprayersGoodie = prob;}
	int getTicksPerSimpleZumiMove() const
	{return m_ticksPerSimpleZumiMove;}
	void setTicksPerSimpleZumiMove(int tick)
	{m_ticksPerSimpleZumiMove = tick;}
	int getTicksPerComplexZumiMove() const
	{return m_ticksPerComplexZumiMove;}
	void setTicksPerComplexZumiMove(int tick)
	{m_ticksPerComplexZumiMove = tick;}
	int getComplexZumiSearchDistance() const
	{return m_complexZumiSearchDistance;}
	void setComplexZumiSearchDistance(int distance)
	{m_complexZumiSearchDistance = distance;}
	int getNumOfZumi() const
	{return m_numOfZumi;}
	void setNumOfZumi(int numZumi)
	{m_numOfZumi = numZumi;}

	std::vector<ActorObject*> getAnActorAtTheLocation(int x,int y);// Return the actors at the position x and y
	std::vector<ActorObject*>* getVectorActor()   //Return a pointer points to actorVector
	{
		return &actorVector;
	}
	Player* getPlayer()          //Return a pointer points to actorPlayer
	{
		return actorPlayer;
	}

  private:
	int loadLevel();                       //private funtion used to load data file
	bool playExitSound;                    //Using this flag to play the exit sound
	bool isFinished;                       //Using this flag to play the finished sound
	std::vector<ActorObject*> actorVector; //Store all the objects except player in this container
	Player* actorPlayer;                   //Player
	// The following private data are used for option values in the level file.
	int m_bonus;      
	int m_goodieTick;
	int m_walkThruLifetimeTick;
	int m_increaseSprayerGoodieTick;
	int m_maxBoostedSprayer;
	int m_probOfGoodieOverall;
	int	m_probOfExtraLifeGoodie; 
	int m_probOfWalkThruGoodie;
	int m_probOfMoreSprayersGoodie;
	int m_ticksPerSimpleZumiMove;
	int m_ticksPerComplexZumiMove;
	int m_complexZumiSearchDistance;
	int m_numOfZumi;
};

#endif // STUDENTWORLD_H_
