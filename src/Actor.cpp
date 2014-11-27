#include "Actor.h"
#include "StudentWorld.h"
#include <vector>
#include <queue>
#include <stack>
#include<iostream>
using namespace std;
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
void Player::doSomething()
{
	if(!isAlive())                          //Check if the player is currently alive
		return;
	if(hasWalkThroughWallGoodie())          //Check if the player has walk through wall goodie
	{
		if(getWalkThruGoodieTick()>0)         
			setWalkThruGoodieTick(getWalkThruGoodieTick()-1); //Decrease walk through goodie tick by 1
		else if( getWalkThruGoodieTick() == 0)
		{
			setWalkThroughWallGoodie(false);              //Set haw walk through wall goodie to false, if tick is 0
			if( getWorld()->isDesBrick(getX(),getY()))    //Check the player doesn't has walk through wall goodie
			{                                             //but still on the same square of the destroyable brick
				setDead();
				getWorld()->playSound(SOUND_PLAYER_DIE);
			}
		}
	}

	if(hasIncreaseSprayerGoodie())           //Check if the player has increase sprayer  goodie
	{
		if(getIncreaseSprayerGoodieTick()>0)        
			setIncreaseSprayerGoodieTick(getIncreaseSprayerGoodieTick()-1);  //Decrease increase sprayer goodie tick by 1
		else if(getIncreaseSprayerGoodieTick() == 0)
			setIncreaseSprayerGoodie(false);         //Set has increase sprayer goodie to false, if tick is 0
	}


	std::vector<ActorObject*>* temp; 
	temp = getWorld()->getVectorActor();                  
	
	for(int i=0;i<temp->size();i++)
	{
		if(getWorld()->isSpray( (*temp)[i]->getX(),(*temp)[i]->getY() ) )   //Check if the player is at the same square with spray
		{
			if((*temp)[i]->getX() == getX() && (*temp)[i]->getY() == getY() ) 
			{
				getWorld()->getPlayer()->setDead();
				getWorld()->playSound(SOUND_PLAYER_DIE);
			}
		}
		else if(getWorld()->isSimpleZumi( (*temp)[i]->getX(),(*temp)[i]->getY() ) )  //Check if the player is at the same square with zumi
		{
			if((*temp)[i]->getX() == getX() && (*temp)[i]->getY() == getY() ) 
			{
				getWorld()->getPlayer()->setDead();
				getWorld()->playSound(SOUND_PLAYER_DIE);
			}
		}
	}

	int ch;                             //Put key value in ch variable
	int x,y;                            //Using x and y to specify moveTo function
	if(getWorld()->getKey(ch)) 
	{
		switch(ch)
		{
			case KEY_PRESS_LEFT:
				x = getX()-1; y = getY();          //Move left
				break;
			case KEY_PRESS_RIGHT:
				x = getX()+1; y = getY();             //Move right
				break;
			case KEY_PRESS_UP:
				x = getX(); y = getY()+1;          //Move upward
				break;
			case KEY_PRESS_DOWN:
				x = getX(); y = getY()-1;           //Move downward
				break;
			case KEY_PRESS_SPACE:                   //Drop the sprayer
				if(getWorld()->isDesBrick(getX(),getY()))   //Cannot drop the sprayer if the player is at the same square with destoruable brick
					return;
				if(hasIncreaseSprayerGoodie())              //If the player has increase sprayer goodie, the number of sprayers the player can drop is different             
				{
					if(countSprayers < getWorld()->getMaxBoostedSprayer())
					{
						if(getWorld()->isSprayer(getX(),getY()))   //Cannot drop two sprayers at the same square
							return;
						else
						{
							temp->push_back(new Sprayer(getWorld(),IID_BUGSPRAYER ,getX(),getY(),40));  //Create sprayer
							countSprayers++;                //Increase sprayer counter
							return;
						}
					}
					else
						return;
				}
				else if(countSprayers < numofSprayers)
				{
					if(getWorld()->isSprayer(getX(),getY()))       //Cannot drop two sprayers at the same square
						return;
					else
					{
						temp->push_back(new Sprayer(getWorld(),IID_BUGSPRAYER ,getX(),getY(),40)); //Create sprayer with 40 ticks
						countSprayers++;
						return;
					}
				}
				else
					return;
			default:
				return;
		}
		if(hasWalkThroughWallGoodie())            //If the player has walk through wall goodie, the player can walk through destroyable wall
		{
			if( !getWorld()->isPermBrick(x,y))
				moveTo(x,y);
		}
		else if( !getWorld()->isPermBrick(x,y) && !getWorld()->isDesBrick(x,y) )
			moveTo(x,y);
	}
}

void Exit::doSomething()
{
	//If the player is at the same square with exit and there is no zumi 
	//Set isFinished flag to be true;
	if( (getX() == getWorld()->getPlayer()->getX()) && (getY() == getWorld()->getPlayer()->getY()) && (getWorld()->getNumOfZumi() == 0))
	{
		getWorld()->playSound(SOUND_FINISHED_LEVEL);
		getWorld()->setIsFinished(true);
	}
	
}
void ExtraLifeGoodie::doSomething()
{
	if(!isAlive())
		return;
	if(getTick()==0)
		setDead();
	else
		setTick(getTick()-1);

	if(getWorld()->getPlayer()->getX() == getX() && getWorld()->getPlayer()->getY() == getY())  //Check if player is at the same square as goodie
	{
		getWorld()->incLives();              //Increase player's life by one
		getWorld()->increaseScore(1000);     //Increase score by 1000
		setDead();                           //Set goodie's state dead
		getWorld()->playSound(SOUND_GOT_GOODIE);  //Play got goodie sound
	}
}

void WalkThroughWallsGoodie::doSomething()
{
	if(!isAlive())
		return;
	if(getTick()==0)
		setDead();
	else
		setTick(getTick()-1);
	if(getWorld()->getPlayer()->getX() == getX() && getWorld()->getPlayer()->getY() == getY())
	{
		getWorld()->getPlayer()->setWalkThroughWallGoodie(true); //Set player to have the ability to walk through the destroyable wall
		getWorld()->getPlayer()->setWalkThruGoodieTick(getWorld()->getwalkThruLifetimeTick()); //Set the time of gooide the player can have 
		getWorld()->increaseScore(1000);              //Increase score by 1000
		setDead();                                    //Set goodie's state dead
		getWorld()->playSound(SOUND_GOT_GOODIE);      //Play got goodie sound
	}
}

void IncreaseSprayerGoodie::doSomething()
{
	if(!isAlive())
		return;
	if(getTick()==0)
		setDead();
	else
		setTick(getTick()-1);
	if(getWorld()->getPlayer()->getX() == getX() && getWorld()->getPlayer()->getY() == getY())
	{
		getWorld()->getPlayer()->setIncreaseSprayerGoodie(true);   //Set player to have the ability to place more sprayers
		getWorld()->getPlayer()->setIncreaseSprayerGoodieTick(getWorld()->getIncreaseSprayerTimeTick());  //Set the time of gooide the player can have 
		getWorld()->increaseScore(1000);                     //Increase score by 1000
		setDead();                                            //Set goodie's state dead
		getWorld()->playSound(SOUND_GOT_GOODIE);               //Play got goodie sound
	}
}

void Sprayer::doSomething()
{
	if(!isAlive())
		return;
	std::vector<ActorObject*>* temp;
	temp = getWorld()->getVectorActor();
	for(int i =0;i< (temp->size());i++)               //This for loop is used to check if the position of sprayer has spray,
	{                                                 //then this sprayer will be triggered.
		if( (*temp)[i]->getX() == getX() && (*temp)[i]->getY() == getY())
		{
			if (getWorld()->isSpray( (*temp)[i]->getX(),(*temp)[i]->getY() ) ) 
			{
					setTick(0);   //Set tick to zero to trigger this sprayer
			}  
		}
	} 

	if(getTick() == 0)              
	{	
		creatSpray(getX(),getY());       //Tick is zero. Trigger the spray
		for(int i=1;i<3;i++)                         //Try to create up spray
		{
			if(getWorld()->isPermBrick(getX(),getY()+i))  //If the spray which will be created  is at the same square with permanent brick
				break;                                    //the spray will not be created. 
			else if(getWorld()->isDesBrick(getX(),getY()+i))  //If the spray which will be created  is at the same square with destroyable brick
				{                                            //the spray will be created.
					creatSpray(getX(),getY()+i);            
					break;
				}
			else
				creatSpray(getX(),getY()+i);              //Create spray at the position without brick
		}
		for(int i=1;i<3;i++)                  //Try to create down spray
		{
			if(getWorld()->isPermBrick(getX(),getY()-i))
				break;
			else if(getWorld()->isDesBrick(getX(),getY()-i))
				{
					creatSpray(getX(),getY()-i);
					break;
				}	
			else
				creatSpray(getX(),getY()-i);

		}
		for(int i=1;i<3;i++)                           //Try to create right spray
		{
			if(getWorld()->isPermBrick(getX()+i,getY()))
				break;
			else if(getWorld()->isDesBrick(getX()+i,getY()))
				{
					creatSpray(getX()+i,getY());
					break;
				}	
			else
				creatSpray(getX()+i,getY());
		}
		for(int i=1;i<3;i++)                        //Try to create left spray
		{
			if(getWorld()->isPermBrick(getX()-i,getY()))
				break;
			else if(getWorld()->isDesBrick(getX()-i,getY()))
				{
					creatSpray(getX()-i,getY());
					break;
				}	
			else	
				creatSpray(getX()-i,getY());
		}
		setDead();                //Set sprayer to dead after it created spray
		getWorld()->playSound(SOUND_SPRAY);  //Play sound
		getWorld()->getPlayer()->setCountSprayers(getWorld()->getPlayer()->getCountofSprayers()-1);//Decrease the sprayer count by 1
		return;
	}
	else
		setTick(getTick()-1);  //Decrease the sprayer tick by 1
}

void Sprayer::creatSpray(int x,int y)  //Create spray in the game's world
{
	std::vector<ActorObject*>* temp;
	temp = getWorld()->getVectorActor();
	temp->push_back(new Spray(getWorld(),x,y,3)); //The life time of spray is 3 tick
	temp->back()->setVisible(true);
}

void Spray::doSomething()
{
	if(!isAlive())             //If spray is not alive, return
		return;
	if(getTick() == 0)         //If tick is zero, set dead
	{	
		setDead();
		return;
	}
	else
	{
		checkLocation();         //Check any actor in the same position as spray
		setTick(getTick()-1);    //Decrease the spray tick by 1
	}
}

void Spray::checkLocation()
{
	std::vector<ActorObject*>* temp;
	temp = getWorld()->getVectorActor();
	for(int i =0;i< (temp->size());i++)
	{
		if( (*temp)[i]->getX() == getX() && (*temp)[i]->getY() == getY())
		{
			if (getWorld()->isDesBrick( (*temp)[i]->getX(),(*temp)[i]->getY() ) )  // If the spray is on the same square as a destroyable brick,
			{                                                                      // then spray will set that brick's state to dead
				(*temp)[i]->setDead();
			}
			else if (getWorld()->isComplexZumi( (*temp)[i]->getX(),(*temp)[i]->getY() ) ) // If the spray is on the same square as a complex zumi,
			{                                                                             // then spray will set that complex zumi's state to dead    
				getWorld()->increaseScore(500);                                           //Increase score by 500
				(*temp)[i]->setDead();
				getWorld()->playSound(SOUND_ENEMY_DIE);
			    getWorld()->setNumOfZumi(getWorld()->getNumOfZumi()-1);                   //Reduce number of zumi in the game's world by 1
			}
			else if (getWorld()->isSimpleZumi( (*temp)[i]->getX(),(*temp)[i]->getY() ) ) // If the spray is on the same square as a simple zumi,
			{																			 // then spray will set that simple zumi's state to dead 		
				getWorld()->increaseScore(100);                                          //Increase score by 100          
				(*temp)[i]->setDead();
				getWorld()->playSound(SOUND_ENEMY_DIE);
			    getWorld()->setNumOfZumi(getWorld()->getNumOfZumi()-1);                     //Reduce number of zumi in the game's world by 1
			}
		}
	}
}

void SimpleZumi::doSomething()                        
{
	if(isAlive() == false)                  //Check if it is alive
	{return;} 

	if(getTick() == 1)
	{
		setTick(getWorld()->getTicksPerSimpleZumiMove()); //reset the tick to TicksPerSimpleZumiMove
		int x,y;                                    //Using x and y to store moving location
		switch(currentDirection)                    // 0 is north, 1 is east
		{                                           // 2 is south and 3 is west
		case 0:                      
			x=getX();y=getY()+1;
			break;
		case 1:
			x=getX()+1;y=getY();
			break;
		case 2:
			x=getX();y=getY()-1;
			break;
		case 3:
			x=getX()-1;y=getY();
			break;
		default:
			break;
		}
		
		if( !getWorld()->isPermBrick(x,y) && !getWorld()->isDesBrick(x,y) && !getWorld()->isSprayer(x,y) )
			moveTo(x,y);                       //If x and y don't have brick or sprayer, then zumi will move to that position
		else
			setCurrentDirection(rand()%4);     //If there are brick or sprayer in that direction, zumi will choose another direction
	}
	else
	{
		setTick(getTick()-1);   //Decrease the tick by 1
	}
}

void SimpleZumi::createGoodie(int x,int y)   //Using this function to determine what kind of goodie the zumi will drop
{
	std::vector<ActorObject*>* temp;
	temp = getWorld()->getVectorActor();
	int tempValue = rand()%100;               //Produce a temp value between 0 - 99 randomly. 
	if(tempValue < getWorld()->getProbOfExtraLifeGoodie() )   //If temp value is smaller than the probability of extra life goodie, then the zumi will drop this goodie
		temp->push_back(new ExtraLifeGoodie(getWorld(),x,y,getWorld()->getGoodieTick()));
	else if( (tempValue >= getWorld()->getProbOfExtraLifeGoodie() ) && (tempValue < getWorld()->getProbOfMoreSprayersGoodie() + getWorld()->getProbOfExtraLifeGoodie() ) )  //If temp value is bigger than the probability of extra life goodie, 
			temp->push_back(new IncreaseSprayerGoodie(getWorld(),x,y,getWorld()->getGoodieTick()));																	        //but smaller than the probability of extra life goodie plus more sprayers goodie, 
	else																																									//then the zumi will drop this goodie
		temp->push_back(new WalkThroughWallsGoodie(getWorld(),x,y,getWorld()->getGoodieTick()));   //If temp value is bigger than the probability of extra life goodie plus more sprayers goodie, 
	                                                                                               //then the zumi will drop walk through wall goodie
	
	temp->back()->setVisible(true);  //Set goodie visible
}

void ComplexZumi::doSomething()
{
	if(isAlive() == false)         //Check if it is alive
	{return;}

	if(getTick() == 1)            //If current tick is 1, zumi will move depending on the algorithm
	{

		setTick(getWorld()->getTicksPerComplexZumiMove());                 //Reset the tick 
		int x,y;															//The variable x and y store the position zumi will move
		int horizDist = abs(getX()-getWorld()->getPlayer()->getX());       
		int verticalDist = abs(getY()-getWorld()->getPlayer()->getY());
		if( (horizDist <= smellDistance) && (verticalDist <= smellDistance) )   //Detect if the player is in the smell range of complex zumi
		{
			if(searchAlgorithm(x,y))                  //Check if the player can be reached by the complex zumi.
			{
				if(!getWorld()->isSprayer(x,y) )      //If the player is not on the same square with the sprayer, then zumi will move
					moveTo(x,y);
				else                                //If player is on the sprayer, then complex zumi acts like simple zumi
				{
					switch(getCurrentDirection())   //Move according to the currentdirection
					{
						case 0:
							x=getX();y=getY()+1;
							break;
						case 1:
							x=getX()+1;y=getY();
							break;
						case 2:
							x=getX();y=getY()-1;
							break;
						case 3:
							x=getX()-1;y=getY();
							break;
						default:
							break;
					}
					if( !getWorld()->isPermBrick(x,y) && !getWorld()->isDesBrick(x,y) && !getWorld()->isSprayer(x,y) )
						moveTo(x,y);    //Check if the position has the brick or sprayer
					else
						setCurrentDirection(rand()%4);   //Change the current direction
				}
			}

			else                           //If the player cannot be reached by complex zumi, then complex zumi will act like simple zumi
			{
				switch(getCurrentDirection())
				{
				case 0:
					x=getX();y=getY()+1;
					break;
				case 1:
					x=getX()+1;y=getY();
					break;
				case 2:
					x=getX();y=getY()-1;
					break;
				case 3:
					x=getX()-1;y=getY();
					break;
				default:
					break;
			}	
			if( !getWorld()->isPermBrick(x,y) && !getWorld()->isDesBrick(x,y) && !getWorld()->isSprayer(x,y) )
				moveTo(x,y);
			else
				setCurrentDirection(rand()%4);
			}
		}
		else              //If the player is not in the smell range of complex zumi, then it will act like simple zumi
		{
			switch(getCurrentDirection())
			{
			case 0:
				x=getX();y=getY()+1;
				break;
			case 1:
				x=getX()+1;y=getY();
				break;
			case 2:
				x=getX();y=getY()-1;
				break;
			case 3:
				x=getX()-1;y=getY();
				break;
			default:
				break;
			}	
			if( !getWorld()->isPermBrick(x,y) && !getWorld()->isDesBrick(x,y) && !getWorld()->isSprayer(x,y) )
				moveTo(x,y);
			else
				setCurrentDirection(rand()%4);
		}
	}
	else
	{
		setTick(getTick()-1);
	}
}

class Coord                        //This class is to help the implementation of searching algorithm. 
{								   //This class is basically to store the information about position x and  y and previous position x and y
  public:
	Coord(int x, int y, int preX,int preY) : m_x(x), m_y(y),m_preX(preX),m_preY(preY) {}
	int x() const { return m_x; }
	int y() const { return m_y; }
	int preX() const {return m_preX;}
	int preY() const {return m_preY;}
  private:
	int m_x;
	int m_y;
	int m_preX;
	int m_preY;
};

bool ComplexZumi::searchAlgorithm(int &x,int &y)
{
	queue<Coord> coordQueue;							//Using this queue to do breadth-first searching algorithm
	stack<Coord> reverseStack;							//Using this stack to store the information about each search done by breadth-first search algorithm
	char maze[15][15];                                  //Creat a maze to store the game's world information
	for(int i=0;i<VIEW_WIDTH;i++)
		for(int j=0;j<VIEW_HEIGHT;j++)
			maze[i][j] = '.';                           //Using '.' to represent that spot can be reached

	std::vector<ActorObject*>* temp;					//Get the information from the game's world
	temp = getWorld()->getVectorActor();
	
	for(int i =0;i< (temp->size());i++)
	{	
		if( getWorld()->isDesBrick( (*temp)[i]->getX(),(*temp)[i]->getY() ) || getWorld()->isPermBrick( (*temp)[i]->getX(),(*temp)[i]->getY() ) )
			maze[(*temp)[i]->getX()][(*temp)[i]->getY()] = 'X';        //Using 'X' to represent that spot is brick 
	}
	coordQueue.push(Coord( getX(),getY(),getX(),getY() ) );

	maze[getX()][getY()] = '#';					//Using '#' to represent that spot is explored

	
	while(!coordQueue.empty())
	{
		Coord tempCoord = coordQueue.front();
		coordQueue.pop();
		int m_x = tempCoord.x();int m_y = tempCoord.y();
		int pre_x = tempCoord.preX(); int pre_y = tempCoord.preY();
		reverseStack.push(Coord( m_x,m_y,pre_x,pre_y ) );          //Store all the coordinate that is popped by the search algorithm
	
		if( (m_x == getWorld()->getPlayer()->getX()) && (m_y == getWorld()->getPlayer()->getY()) )  //If this statement is true,
		{																						//it means the player can be reached by the zumi
			Coord tempReverse = reverseStack.top();
			while(!reverseStack.empty())												//Explain this part in the report.txt document
			{																						
				Coord tempReversePre = reverseStack.top();
				reverseStack.pop();
				if(tempReversePre.x() == tempReversePre.preX() && tempReversePre.y() == tempReversePre.preY() )
				{
					x = tempReverse.x();
					y = tempReverse.y();
					break;
				}
				if(tempReverse.preX() == tempReversePre.x() && tempReverse.preY() == tempReversePre.y())
					tempReverse = tempReversePre;
			}
			return true;                                              //Return true to indicate that the player can be reached by zumi
		}
		else                                                          //Do the breadth-first algorithm
			if(maze[m_x+1][m_y] != 'X' && maze[m_x+1][m_y] != '#')
			{
				maze[m_x+1][m_y] = '#';
				coordQueue.push(Coord(m_x+1,m_y,m_x,m_y));
			}
			if(maze[m_x][m_y-1] != 'X' && maze[m_x][m_y-1] != '#')
			{
				maze[m_x][m_y-1] = '#';
				coordQueue.push(Coord(m_x,m_y-1,m_x,m_y));
			}
			if(maze[m_x-1][m_y] != 'X' && maze[m_x-1][m_y] != '#')
			{
				maze[m_x-1][m_y] = '#';
				coordQueue.push(Coord(m_x-1,m_y,m_x,m_y));
			}
			if(maze[m_x][m_y+1] != 'X' && maze[m_x][m_y+1] != '#')
			{
				maze[m_x][m_y+1] = '#';
				coordQueue.push(Coord(m_x,m_y+1,m_x,m_y));
			}
	}
	return false;					//Return false to indicate that the player cannot be reached by zumi
}