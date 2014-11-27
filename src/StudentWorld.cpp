#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "StudentWorld.h"
#include "Actor.h"
#include "Level.h"

using namespace std;

GameWorld* createStudentWorld()
{
	return new StudentWorld();
}

int StudentWorld::init()
{
	setNumOfZumi(0);				 //Initialize number of zumi in the game world
	return loadLevel();              //Initialize the data structure used to keep track
									 //of the game's world and load the level data file
}

int StudentWorld::move()
{
	
	updateDisplayText();                      //Update display text
	actorPlayer->doSomething();				  //Ask player to do something
	for(int i =0;i<actorVector.size();i++)
	{
		if(actorVector[i]->isAlive())
			actorVector[i]->doSomething();		  //Ask for each alive actor to do something;

		if(actorPlayer->isAlive() == false)   //If actor does something that causes the player to die, immediately return;
		{
			decLives();
			return GWSTATUS_PLAYER_DIED;
		}
	}
	removeDeadActors();								//Remove all the dead actors
	
	if(playExitSound && (getNumOfZumi() == 0))                 //If there is no Zumi in the gameWorld
	{                                               //display the exit actor, and play the reveal exit sound
		playExitSound = false;						//Set flag to false, in case the game's world play sound repeatedly
		for(int i =0;i<actorVector.size();i++)
		{
			if( isExit(actorVector[i]->getX(),actorVector[i]->getY() ) ) 
			{
				actorVector[i]->setVisible(true);  
			}
		}
		playSound(SOUND_REVEAL_EXIT);
	}  

	if(isFinished)                                 //If player is on the same square of the exit, then the level is finished
	{
		increaseScore(getBonus());				  //Increase the player score by the rest of the bonus
		isFinished = false;						  // Set this flag to false to let player to play next level
		playExitSound = true;                   
		return GWSTATUS_FINISHED_LEVEL;
	}

	if(actorPlayer->isAlive() == false)
	{
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	return GWSTATUS_CONTINUE_GAME;              //If player is alive and there is a zumi in the game's world, the game will continue
}

void StudentWorld::removeDeadActors()			//To delete all dead object
{
	vector<ActorObject*>::iterator itr;
	itr = actorVector.begin();
	while(itr!=actorVector.end())
	{
		if((*itr)->isAlive()==false)
		{
			delete *itr;
			actorVector.erase(itr);
			itr = actorVector.begin();      //Address of vector changes so assign itr to the beginning address of the vector
			continue;						//Using continue command to avoid using itr++
		}
		itr++;
	}
}

void StudentWorld::updateDisplayText()
{
	int score = getScore();
	int level = getLevel();
	int livesLeft = getLives();
	unsigned int bonus = getBonus();
	
	ostringstream oss;                       //Using ostringstream to update display text
	oss<<"Score: "; 
	oss.fill('0');                          //Fill out empty with zero
	oss<<setw(8)<<score<<"  "<<"Level: "<<setw(2)<<level<<"  "<<"Lives: "<<setw(3)<<livesLeft<<"  ";
	oss.fill(' ');							//Fill out empty 
	oss<<"Bonus: "<<setw(6)<<bonus;
	string temp = oss.str();				//Convert ostringstream to string to display
	setGameStatText(temp);
	if(bonus > 0)							//Decrease bonus by 1
		setBonus(bonus-1);

}

void StudentWorld::cleanUp()              //Same as destructor to delete all dynamic data to prevent memory leak
{
	vector<ActorObject*>::iterator itr;
	itr = actorVector.begin();
	while(itr!=actorVector.end())
	{
			delete *itr;
			actorVector.erase(itr);
			itr = actorVector.begin();
	}
	delete actorPlayer;
}


//Check if player is at the position of x and y
bool StudentWorld::isPlayer(int x,int y)
{
	vector<ActorObject*> temp = getAnActorAtTheLocation(x,y);   //get all the actor at the position of x and y
	if(!temp.empty())
	{
		for(int i =0;i<temp.size();i++)
		{
			Player* db = dynamic_cast<Player*>(temp[i]);   //Using dynamic cast to check who is who at that position
			if(db != nullptr)
			{
				if(!db->isAlive())                //If that object is dead, return false
					return false;
				else                              
					return true;
			}
		}
	}
	else
		return false;
	return false;
}

//Check if exit is at the position of x and y
bool StudentWorld::isExit(int x,int y)
{
	vector<ActorObject*> temp = getAnActorAtTheLocation(x,y);
	if(!temp.empty())
	{
		for(int i =0;i<temp.size();i++)
		{
			Exit* db = dynamic_cast<Exit*>(temp[i]);
			if(db != nullptr)
			{
				if(!db->isAlive())
					return false;
				else
					return true;
			}
		}
	}
	else
		return false;
	return false;
}

//Check if zumi is at the position of x and y
bool StudentWorld::isSimpleZumi(int x,int y)
{
	vector<ActorObject*> temp = getAnActorAtTheLocation(x,y);
	if(!temp.empty())
	{
		for(int i =0;i<temp.size();i++)
		{
			SimpleZumi* db = dynamic_cast<SimpleZumi*>(temp[i]);
			if(db != nullptr)
			{
				if(!db->isAlive())
					return false;
				else
					return true;
			}
		}
	}
	else
		return false;
	return false;
}

//Check if permanent brick is at the position of x and y
bool StudentWorld::isPermBrick(int x,int y)
{
	vector<ActorObject*> temp = getAnActorAtTheLocation(x,y);
	if(!temp.empty())
	{
		for(int i =0;i<temp.size();i++)
		{
			PermaBrick* db = dynamic_cast<PermaBrick*>(temp[i]);
			if(db != nullptr)
			{
				if(!db->isAlive())
					return false;
				else
					return true;
			}
		}
	}
	else
		return false;
	return false;
}

//Check if destroyable brick is at the position of x and y
bool StudentWorld::isDesBrick(int x,int y)
{
	vector<ActorObject*> temp = getAnActorAtTheLocation(x,y);
	if(!temp.empty())
	{
		for(int i =0;i<temp.size();i++)
		{
			DestroyableBrick* db = dynamic_cast<DestroyableBrick*>(temp[i]);
			if(db != nullptr)
			{
				if(!db->isAlive())
					return false;
				else
					return true;
			}
		}
	}
	else
		return false;
	return false;
}

//Check if sprayer is at the position of x and y
bool StudentWorld::isSprayer(int x,int y)
{
	vector<ActorObject*> temp = getAnActorAtTheLocation(x,y);
	if(!temp.empty())
	{
		for(int i =0;i<temp.size();i++)
		{
			Sprayer* db = dynamic_cast<Sprayer*>(temp[i]);
			if(db != nullptr)
			{
				if(!db->isAlive())
					return false;
				else
					return true;
			}
		}
	}
	else
		return false;
	return false;
}

//Check if spray is at the position of x and y
bool StudentWorld::isSpray(int x,int y)
{
	vector<ActorObject*> temp = getAnActorAtTheLocation(x,y);
	if(!temp.empty())
	{
		for(int i =0;i<temp.size();i++)
		{
			Spray* db = dynamic_cast<Spray*>(temp[i]);
			if(db != nullptr)
			{
				if(!db->isAlive())
					return false;
				else
					return true;
			}
		}
	}
	else
		return false;
	return false;
}
//Check if complexZumi is at the position of x and y
bool StudentWorld::isComplexZumi(int x,int y)
{
	vector<ActorObject*> temp = getAnActorAtTheLocation(x,y);
	if(!temp.empty())
	{
		for(int i =0;i<temp.size();i++)
		{
			ComplexZumi* db = dynamic_cast<ComplexZumi*>(temp[i]);
			if(db != nullptr)
			{
				if(!db->isAlive())
					return false;
				else
					return true;
			}
		}
	}
	else
		return false;
	return false;
}

// Return all the actor at the position x and y
vector<ActorObject*> StudentWorld::getAnActorAtTheLocation(int x,int y)
{
	vector<ActorObject*> temp;
	vector<ActorObject*>::iterator itr;
	itr = actorVector.begin();
	while(itr!=actorVector.end())
	{
		if((*itr)->getX()==x && (*itr)->getY() == y)
		{

			temp.push_back(*itr);
		}
		itr++;
	}
	return temp;
}



/*bool StudentWorld::anyZumi()
{
	bool temp = false;
	for(int i =0;i<actorVector.size();i++)
	{
		if( isSimpleZumi(actorVector[i]->getX(),actorVector[i]->getY()) )
			temp = true;
	}
	return temp;
} */

int StudentWorld::loadLevel()
{
	Level lev;
	ostringstream oss;
	oss<<"level0"<<getLevel()<<".dat";              //Using ostringsteam to load data file
	string curLevel = oss.str();					//Convert oss to string

	Level::LoadResult result = lev.loadLevel(curLevel);
	if (result == Level::load_fail_file_not_found && (curLevel != "level00.dat")) //In this situation the player is won
		return GWSTATUS_PLAYER_WON;
	else if(result == Level::load_fail_bad_format)
		return GWSTATUS_LEVEL_ERROR;
	else if(result == Level::load_fail_file_not_found )
		return GWSTATUS_NO_FIRST_LEVEL;
	else if(result == Level::load_success)
	{
		setBonus(lev.getOptionValue(optionLevelBonus));												//Load all the optionvalues in the level data
		setGoodieTick(lev.getOptionValue(optionGoodieLifetimeInTicks));
		setIncreaseSprayerTimeTick(lev.getOptionValue(optionBoostedSprayerLifetimeTicks));
		setwalkThruLifetimeTick(lev.getOptionValue(optionWalkThruLifetimeTicks));
		setMaxBoostedSprayer(lev.getOptionValue(optionMaxBoostedSprayers));
		setProbOfGoodieOverall(lev.getOptionValue(optionProbOfGoodieOverall));
		setProbOfExtraLifeGoodie(lev.getOptionValue(optionProbOfExtraLifeGoodie));
		setProbOfMoreSprayersGoodie(lev.getOptionValue(optionProbOfMoreSprayersGoodie));
		setProbOfWalkThruGoodie(lev.getOptionValue(optionProbOfWalkThruGoodie));
		setTicksPerSimpleZumiMove(lev.getOptionValue(optionTicksPerSimpleZumiMove));
		setTicksPerComplexZumiMove(lev.getOptionValue(optionTicksPerComplexZumiMove));
		setComplexZumiSearchDistance(lev.getOptionValue(optionComplexZumiSearchDistance));

		for(int i=0;i<VIEW_WIDTH;i++)                                     //Initialize the data structure used to keep track of the game's world
		{                                                                 //i and j are used to specify the positon of each object
			for(int j=0;j<VIEW_HEIGHT;j++)
			{
				Level::MazeEntry ge = lev.getContentsOf(i,j);
				switch(ge)
				{
				case Level::empty:
					break;
				case Level::simple_zumi:
					actorVector.push_back(new SimpleZumi(this,IID_SIMPLE_ZUMI,i,j,getTicksPerSimpleZumiMove()));
					setNumOfZumi(getNumOfZumi()+1);
					break;
				case Level::complex_zumi:
					actorVector.push_back(new ComplexZumi(this,i,j,getTicksPerComplexZumiMove()));
					setNumOfZumi(getNumOfZumi()+1);
					break;
				case Level::player:
					actorPlayer = new Player(this,i,j);
					break;
				case Level::exit:
					actorVector.push_back(new Exit(this,i,j));
					break;
				case Level::perma_brick:
					actorVector.push_back(new PermaBrick(this,i,j));
					break;
				case Level::destroyable_brick:
					actorVector.push_back(new DestroyableBrick(this,i,j));
					break;
				}
			}
		}
		return GWSTATUS_CONTINUE_GAME;    //Load data successfully
	}
}


