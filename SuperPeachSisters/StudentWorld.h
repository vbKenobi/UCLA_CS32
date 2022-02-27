#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class BaseActor;
class Peach;

class StudentWorld : public GameWorld
{
public:
  //Housekeeping functions
  StudentWorld(std::string assetPath);
  ~StudentWorld();
    
  //Functions that help play the game
  virtual int init();
  virtual int move();
  virtual void cleanUp();

  //Generate a random direction used by the init function
  int generateRandDirection();

    
  //Determine if two actors collide (overalp function_
  bool actorsCollide(int x1, int y1, int x2, int y2);
    
  //Determines if the actor collides with a wall
  bool actorsCollideWall(int x, int y, BaseActor* actor);

  //Takes in the x,y cordiante and calls the bonk function of the associated actor
  void callBonk(int x, int y, BaseActor* actor);
  //Returns true if there is an empty space at an x,y coordinate
  bool determineIfEmptySpace(int x, int y, BaseActor* actor);
  //Call the bonk function for a nonDamagable actor object
  bool callBonkNonDamagable(int x, int y, BaseActor* actor);
    
  //Determine if the at the x,y there is a collision with peach
  bool collideWithPeach(int x, int y);
     
  //Peach accersor and setter functions
  bool peachHasInviz();
  void addShootToPeach();
  void addJumpToPeach();
  void addStarToPeach();
  int getPeachX();
  int getPeachY();
    
  //Tell peach to get bonked
  void bonkPeach();
  //Increases Peach's hit points
  void increasePeachHitPoints();
    
  //Indicate when a level has been completed
  void finishLevel();
  //Indiacte when the game has been finished
  void finishGame();
    
  //Delete the passed in actor from the actors vector
  bool deleteSprite(BaseActor* actor);
  //Add the actor to the actors vector
  void addSprite(BaseActor* actor);
    
private:
  std::vector<BaseActor*> actors;
  Peach* peach;
  bool finishedLevel = false;
  bool finishedGame = false;
};

#endif // STUDENTWORLD_H_

