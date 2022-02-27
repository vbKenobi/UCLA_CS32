#include "StudentWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include "GameWorld.h"
#include <string>
#include <vector>
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw
using namespace std;

//////////////////////////////////DEFINITION OF THE HOUSEKEEPING FUNCTIONS/////////////////////////////////
GameWorld* createStudentWorld(string assetPath) {
    return new StudentWorld(assetPath);
}

//CONSTRUCTOR FOR STUDENTWORLD:
StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}


//DESTRUCTOR FOR STUDENTWORLD:
StudentWorld::~StudentWorld() {
    cleanUp();
}

//GENERATE THE RANDOM DIRECTION USED IN THE INTIT FUNCTION
int StudentWorld::generateRandDirection() {
    if(randInt(1,2) == 1)
        return 0;
    return 180;
}

////////////////////////////////////////FUNCTIONS THAT HELP PLAY THE GAME///////////////////////////////////
//STUDENT WORLD INIT FUNCTION CREATES AND GETS THE LEVEL
int StudentWorld::init() {
    
    //New level has been created so we have not finished the level
    finishedLevel = false;
    
    //Get the level file using the string stream
    ostringstream oss;
    int k = getLevel();
    oss.fill('0');
    oss << setw(2) << k;
    
    Level lev(assetPath());
    string level_file = "level" + oss.str() + ".txt";
    Level::LoadResult result = lev.loadLevel(level_file);
    
    //Checks to see if the level file is of a valid format
    if (result == Level::load_fail_file_not_found)
        cerr << "Could not find level01.txt data file" << endl;
     
    else if (result == Level::load_fail_bad_format)
        cerr << "level01.txt is improperly formatted" << endl;
    
    else if (result == Level::load_success) {
        cerr << "Successfully loaded level" << endl;
        
        //For ever element in the level file:
        Level::GridEntry ge;
        for(int i = GRID_WIDTH-1; i >= 0; i--) {
            for(int j = GRID_HEIGHT; j >= 0; j--) {
                ge = lev.getContentsOf(i, j);
                
                //If the content matches one of the objects create and allocate the object and add it to the actor vector
                switch (ge)
                {
                    case Level::empty:
                        break;
                        
                    case Level::koopa:
                        actors.push_back(new Koopa(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, generateRandDirection()));
                        break;
                        
                    case Level::goomba:
                        actors.push_back(new Goomba(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, generateRandDirection()));
                        break;
                        
                    case Level::peach:
                        peach = new Peach(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT);
                        break;
                        
                    case Level::flag:
                        actors.push_back(new Flag(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, 0));
                        break;
                        
                    case Level::block:
                        actors.push_back(new Block(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, false, false, false));
                        break;
                        
                    case Level::star_goodie_block:
                        actors.push_back(new Block(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, false, false, true));
                        break;
                        
                    case Level::piranha:
                        actors.push_back(new Piranha(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, generateRandDirection()));
                        break;
                        
                    case Level::mushroom_goodie_block:
                        actors.push_back(new Block(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, false, true, false));
                        break;
                        
                    case Level::flower_goodie_block:
                        actors.push_back(new Block(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, true, false, false));
                        break;
                        
                    case Level::pipe:
                        actors.push_back(new Pipe(this, i*SPRITE_WIDTH, j*SPRITE_WIDTH));
                        break;
                        
                    case Level::mario:
                        actors.push_back(new Mario(this, i*SPRITE_WIDTH, j*SPRITE_WIDTH, 0));
                        break;
                }
            }
        }
        
    }
    return GWSTATUS_CONTINUE_GAME;
}


//AT EVERY TICK THIS CODE IS EXECUTED AND CALLS THE DO SOMETHING OF ALL OF THE ACTORS
int StudentWorld::move()
{
    //Create the text at the top of the screen
    
    ostringstream oss;
    int k = getScore();
    oss.fill('0');
    oss << "Lives: " << getLives() << "  Level: " << setw(2) << getLevel() << " Score: " << setw(6) << k;
    
    if(peach->getStarPower())
        oss << " StarPower!";
    if(peach->getShoot())
        oss << " ShootPower!";
    if(peach->getJump())
        oss << " JumpPower!";

    //Display the text at the top of the screen
    setGameStatText(oss.str());
    
    //IF PEACH IS NOT ALIVE RESTART THE GAME
    if(!peach->isAlive()) {
        decLives();
        playSound(SOUND_PLAYER_DIE);
        return GWSTATUS_PLAYER_DIED;
    }
    //If we have finished the game
    if(finishedGame) {
        playSound(SOUND_GAME_OVER);
        return GWSTATUS_PLAYER_WON;
    }
    //If we have finished a level
    if(finishedLevel) {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    //HAVE THE ACTORS DO SOMETHING:
    peach->doSomething();
    for(int i = 0; i < actors.size(); i++)
        actors[i]->doSomething();
    
    //DELETE ALL OF THE DEAD ACTORS:
    for(int i = 0; i < actors.size(); i++) {
        if(!actors[i]->isAlive()) {
            BaseActor* temp = actors[i];
            actors[i] = actors[actors.size()-1];
            delete temp;
            actors.pop_back();
        }
    }
        
    return GWSTATUS_CONTINUE_GAME;
}

//CLEAN UP AND DELETE ALL OF THE ACTORS
void StudentWorld::cleanUp()
{
    while(actors.size() != 0) {
        delete actors[actors.size()-1];
        actors.pop_back();
    }
    
    delete peach;
}


////////////////////////////HELPER FUNCTIONS THAT DETECT THE DIFFERENT COLLISIONS////////////////////////////

//Given two (x,y) cordinates determien if the two rectangles would overalp
bool StudentWorld::actorsCollide(int x1, int y1, int x2, int y2)
{
    //Case 1, where x1 is less than x2
    if(x2 >= x1 && x2 < x1 + SPRITE_WIDTH) {
        if(y1 >= y2 && y1 < y2 + SPRITE_HEIGHT) {
                return true;
        }
        if(y2 >= y1 && y2 < y1 + SPRITE_HEIGHT) {
                return true;
        }
    }
    //Case 2, where x2 is les than x1
    if(x1 >= x2 && x1 < x2 + SPRITE_WIDTH) {
        if(y1 >= y2 && y1 < y2 + SPRITE_HEIGHT) {
                return true;
        }
        if(y2 >= y1 && y2 < y1 + SPRITE_HEIGHT) {
                return true;
        }
    }
    //Case 3, where x1 is in between x2 and x2+SPRITE_WIDTH
    if(x1+SPRITE_WIDTH-1 >= x2 && x2+SPRITE_WIDTH > x1 + SPRITE_WIDTH-1) {
        if(y1 >= y2 && y1 < y2 + SPRITE_HEIGHT) {
                return true;
        }
        if(y2 >= y1 && y2 < y1 + SPRITE_HEIGHT) {
                return true;
        }
    }
    
    return false;
}

//See if the actor would collide with a wall
bool StudentWorld::actorsCollideWall(int x, int y, BaseActor* actor)
{
    for(int i = 0; i < actors.size(); i++) {
        //check to see that we are not comparing the same actor to itself
        if(actor == actors[i])
            continue;
        
        if(actors[i]->actsWall() && actorsCollide(x, y, actors[i]->getX(), actors[i]->getY())) {
            return true;
        }
    }
    
    return false;
}

//Determine if the actor would collide with peach
bool StudentWorld::collideWithPeach(int x, int y)
{
    if(actorsCollide(x, y, peach->getX(), peach->getY())) {
        return true;
    }
    return false;
}

//Determine if there is an empty space at the given x, y coordinate
bool StudentWorld::determineIfEmptySpace(int x, int y, BaseActor* actor)
{
    bool foundBlock = false;
    
    for(int i = 0; i < actors.size(); i++) {
        //Check that the block object is not compared to the same object
       if(actor == actors[i] || !actors[i]->actsWall())
           continue;
        
        //Check to see if the point is within the rectnagle
        if(actors[i]->getX() <= x  && actors[i]->getX()+SPRITE_WIDTH > x && actors[i]->getY() <= y && actors[i]->getY()+SPRITE_HEIGHT > y) {
            foundBlock = true;
            break;
        }
    }
    
    return !foundBlock;
}

//////////////////////////////////FUNCTIONS THAT HELP BONK ACTORS///////////////////////////////////////////

bool StudentWorld::callBonkNonDamagable(int x, int y, BaseActor* actor)
{
    for(int i = 0; i < actors.size(); i++) {
        //Check that the block object is not compared to the same object
       if(actor == actors[i] || !actors[i]->canBeDamaged())
           continue;
        //If the actor is not alive, do not bonk it
        if(!actors[i]->isAlive())
            continue;
        
        //Call the bonk function of the colliding actor
        if(actorsCollide(x, y, actors[i]->getX(), actors[i]->getY())) {
            actors[i]->bonk();
            return true;
            
        }
    }
    return false;
}

void StudentWorld::callBonk(int x, int y, BaseActor* actor)
{
    for(int i = 0; i < actors.size(); i++) {
        //Check that the block object is not compared to the same object
       if(actor == actors[i])
           continue;
        
        //If they are colliding, call the bonk function
        if(actorsCollide(x, y, actors[i]->getX(), actors[i]->getY())) {
           actors[i]->bonk();
            return;
        }
    }
}

void StudentWorld::bonkPeach() {
    peach->bonk();
}

////////////////////////////////FUNCTIONS THAT INDICATE STATUS OF GAME///////////////////////////////////////

void StudentWorld::finishGame() {
    finishedGame = true;
}

void StudentWorld::finishLevel() {
    finishedLevel = true;
}

////////////////////////////////FUNCTIONS THAT ADD AND REMOVE ACTORS FROM THE VECTOR/////////////////////////
void StudentWorld::addSprite(BaseActor* actor) {
    actors.push_back(actor);
}

bool StudentWorld::deleteSprite(BaseActor* actor) {
    for(int i = 0; i < actors.size(); i++) {
        if(actors[i] == actor) {
            BaseActor* temp = actors[i];
            actors[i] = actors[actors.size()-1];
            actors.pop_back();
            delete temp;
            return true;
        }
    }
    
    return false;
}

/////////////////////////////////HELPER FUNCTIONS THAT CHANGE PEACH////////////////////////////////////////
void StudentWorld::increasePeachHitPoints() {
    peach->setHitPoints(2);
}

void StudentWorld::addShootToPeach() {
    peach->addShooting();
}

void StudentWorld::addJumpToPeach() {
    peach->addJump();
}

void StudentWorld::addStarToPeach() {
    peach->addStarPower();
}

bool StudentWorld::peachHasInviz() {
    return peach->getStarPower();
}

int StudentWorld::getPeachX() {
    return peach->getX();
}

int StudentWorld::getPeachY() {
    return peach->getY(); 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////


