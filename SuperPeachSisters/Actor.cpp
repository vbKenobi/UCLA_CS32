#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"

using namespace std;

///////////////////////////BaseActor Definitions//////////////////////////////
BaseActor::BaseActor(StudentWorld* sw, int x, int y, int nameID, int dir, int depth, double size) : GraphObject(nameID, x, y, dir, depth, size), m_world(sw) {
    alive = true;
}

//return the world
StudentWorld* BaseActor::getWorld() {
    return m_world;
}

//check if is alive
bool BaseActor::isAlive() {
    return alive;
}

//All actors can be damaged unless specified
bool BaseActor::canBeDamaged() {
    return true;
}

//Do nothing if actor doesn't have bonk specified
void BaseActor::bonk() {
    //Do nothing
}

void BaseActor::setDead() {
    alive = false;
}

//All actors do not act as a wall unless specified
bool BaseActor::actsWall() {
    return false;
}

//////////////////////GOODIE BASE CLASS IMPLEMENTATION////////////////////////////
Goodie::Goodie(StudentWorld* sw, int x, int y, int ID) : BaseActor(sw, x, y, ID, 0, 1, 1.0)
{}

void Goodie::moveGoodie() {
    //If there is an empty pixel, 2 spaces below, lower the goodie by 2 pixles
    if(!getWorld()->actorsCollideWall(getX(), getY()-2, this) && !getWorld()->actorsCollideWall(getX(), getY()-1, this)) {
        moveTo(getX(), getY()-2);
    }
    //If the direction is to the left
    if(getDirection() == 180) {
        //If there is nothing blocking the movement to the left
        if(!getWorld()->actorsCollideWall(getX()-2, getY(), this))
            //Move the actor to the left
            moveTo(getX()-2, getY());
        else {
            //Otherwise reverse the direction
            setDirection(0);
            return;
        }
    }
    else {
        //If there is nothing blocking the movement to the right
        if(!getWorld()->actorsCollideWall(getX()+2, getY(), this))
            //Move the actor to the right
            moveTo(getX()+2, getY());
        else {
            //Otherwise reverse the direction
            setDirection(180);
            return;
        }
    }
    
}

bool Goodie::collidePeach(int points) {
    //If goodie is colliding with peach
    if(getWorld()->collideWithPeach(getX(), getY())) {
        //Tell each that she has recieved a power up
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        getWorld()->increasePeachHitPoints();
        getWorld()->increaseScore(points);
        setDead();
        return true;
    }
    
    return false;
}

bool Goodie::canBeDamaged() {
    return false;
}


//////////////////////ENEMY BASE CLASS IMPLEMENTATION////////////////////////////
Enemy::Enemy(StudentWorld* sw, int x, int y, int ID, int dir) : BaseActor(sw, x, y, ID, dir, 0, 1)
{}

void Enemy::moveEnemy() {
    if(!isAlive())
        return;
    
    //Check of there is a collision with peach
    if(getWorld()->collideWithPeach(getX(), getY())) {
        //If peach has the star power
        if(getWorld()->peachHasInviz()) {
            //Call the bonk function of this object and set to dead
            getWorld()->increaseScore(100);
            bonk();
            getWorld()->playSound(SOUND_PLAYER_KICK);
            setDead();
            return;
        }
        //Else bonk peack
        getWorld()->bonkPeach();
        return;
    }
    
    //If direction is left
    if(getDirection() == 180) {
        //And cannot move to the left
        if(getWorld()->actorsCollideWall(getX()-1, getY(), this))
            //Reverse direction
            setDirection(0);
        //If there is an empty space to the left
        else if(getWorld()->determineIfEmptySpace(getX()-1, getY()-1, this))
            //Reverse the direction
           setDirection(0);

    }
    else {
        //If cannot move to the right
        if(getWorld()->actorsCollideWall(getX()+1, getY(), this))
            //Reverse direction
            setDirection(180);
        
        //If empty space to the right
        else if(getWorld()->determineIfEmptySpace(getX()+1+SPRITE_WIDTH, getY()-1, this))
            //reverse direction
             setDirection(180);
    }
    
    if(getDirection() == 0) {
        //if can move to right without falling off or colliding
        if(!getWorld()->actorsCollideWall(getX()+1, getY(), this) && !getWorld()->determineIfEmptySpace(getX()+1+SPRITE_WIDTH, getY()-1, this))
            //move right
            moveTo(getX()+1, getY());
    }
    else if(getDirection() == 180) {
        //if can move left without falling of or colliding
        if(!getWorld()->actorsCollideWall(getX()-1, getY(), this) && !getWorld()->determineIfEmptySpace(getX()-1, getY()-1, this))
            //move left
            moveTo(getX()-1, getY());
    }
     
}

bool Enemy::canBeDamaged() {
    return true;
}

void Enemy::bonk() {
    getWorld()->increaseScore(100);
    setDead();
}

///////////////////////Projectile BASE CLASS IMPLEMENTAITON/////////////////////
Projectile::Projectile(StudentWorld *sw, int x, int y, int dir, int ID) : BaseActor(sw, x, y, ID, dir, 1, 1.0)
{}

void Projectile::moveProjectile() {
    //If projectile can fall, move it down two pixels
    if(!getWorld()->actorsCollideWall(getX(), getY()-2, this) && !getWorld()->actorsCollideWall(getX(), getY()-1, this)) {
        moveTo(getX(), getY()-2);
    }
    if(getDirection() == 180) {
        //If can move to the left move two pixels to the left
        if(!getWorld()->actorsCollideWall(getX()-2, getY(), this))
            moveTo(getX()-2, getY());
        //Else cannot move anymore, so set dead
        else {
            setDead();
        }
    }
    else {
        //If can move to the right, move two to the right
        if(!getWorld()->actorsCollideWall(getX()+2, getY(), this))
            moveTo(getX()+2, getY());
        //Else cannot move anymore, so set dead
        else {
            setDead();
        }
    }
}

bool Projectile::canBeDamaged() {
    return false;
}

////////////////////////END MARKER IMPLEMENTATION///////////////////////////////
EndMarker::EndMarker(StudentWorld* sw, int x, int y, int dir, int ID) : BaseActor(sw, x, y, ID, dir, 0, 1)
{}

bool EndMarker::canBeDamaged() {
    return false;
}

//////////////////////PEACH CLASS IMPLEMENTATION////////////////////////////////
Peach::Peach(StudentWorld* sw, int x, int y) : BaseActor(sw, x, y, IID_PEACH, 0, 0, 1.0) {
    health_point = 1;
    jump_distance = 0;
    time_to_next_shot = 0;
    temp_invicibility = 0;
    starPower = false;
    jumpPower = false;
    shootPower = false;
    
}

void Peach::doSomething() {
    
    //If peach is not alive return immediatly;
    if(!isAlive())
        return;
    
    //If the temp_invincibiltiy is greater than zero decrement by 1
    if(temp_invicibility > 0) {
        temp_invicibility--;
    }
    //If our invicibiltiy is zero but we have the starPower remove the star power
    if(temp_invicibility == 0 && starPower)
        starPower = false;
    
    //If we have zero health set dead and return
    if(health_point == 0) {
        setDead();
        return;
    }

    //Decrement the time to the next shot
    time_to_next_shot--;
    
    //If peach decided to jump
    if(jump_distance > 0) {
        //Check to see if we can move 4 pixels upward
        if(!getWorld()->actorsCollideWall(getX(), getY()+4, this)) {
            moveTo(getX(), getY()+4);
            jump_distance--;
        }
        //Otherwise, bonk the object peach collided with, and set peach to fall
        else {
            getWorld()->callBonk(getX(), getY()+SPRITE_HEIGHT, this);
            jump_distance = 0;
        }
        
    }
    //If there is nothing three pixels below peach move her down 4 pixels
    else if(!getWorld()->actorsCollideWall(getX(), getY(), this) && !getWorld()->actorsCollideWall(getX(), getY()-1, this) && !getWorld()->actorsCollideWall(getX(), getY()-2, this) && !getWorld()->actorsCollideWall(getX(), getY()-3, this)) {
        moveTo(getX(), getY()-4);
    }
    
    //Get a key press if there was a key press
    int key;
    if (getWorld()->getKey(key)) {
        switch (key)
        {
        //If the key press was the left key
          case KEY_PRESS_LEFT:
            //Set the direction to the left
            setDirection(180);
            //If we can move 4 pixels to the left move 4 to the left
            if(!getWorld()->actorsCollideWall(getX()-4, getY(), this))
                moveTo(getX()-4, getY());
            //If we can't call the bonk function of the object we collided with
            else
                getWorld()->callBonk(getX()-4, getY(), this);;
                
            break;
                
        //If the key press was the right key
        case KEY_PRESS_RIGHT:
            //Set the direction to the right
            setDirection(0);
            //If we can move 4 picels to the right, move 4 to the right
            if(!getWorld()->actorsCollideWall(getX()+4, getY(), this))
                moveTo(getX()+4, getY());
            //Otherwise call the bonk object that we collided with
            else
                getWorld()->callBonk(getX()+4, getY(), this);
            
            break;
                
        //If the up key was pressed
        case KEY_PRESS_UP:
            //See if there is something that we have to jump off of
            if(getWorld()->actorsCollideWall(getX(), getY()-1, this)) {
                //If we had the jump power set the jump distacne to 12
                if(jumpPower)
                    jump_distance = 12;
                else
                    jump_distance = 8;
                //Play the sound of player jumping
                getWorld()->playSound(SOUND_PLAYER_JUMP);
            }
            break;
        //If the space bar was pressed
        case KEY_PRESS_SPACE:
            //If no shoot power, do nothing
            if(shootPower) {
                //Do nothing if we are still in reload
                if(time_to_next_shot > 0)
                { }
                else {
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                    time_to_next_shot = 8;
                    //Depending on the direction generate a fireball and create it 4 pixels away in the direction that peach is facing
                    if(getDirection() == 0)
                        getWorld()->addSprite(new PeachFireball(getWorld(), getX()+4, getY(), getDirection()));
                    else
                        getWorld()->addSprite(new PeachFireball(getWorld(), getX()-4, getY(), getDirection()));
                }
                
            }

        }
    }
    
}

void Peach::bonk()
{
    //If peach has a starpower or invinciblity, do nothing
    if(getStarPower() || temp_invicibility > 0) {
        //Do nothing
    }
    else {
        //Deceremnt the health point by one
        health_point--;
        temp_invicibility = 10;
        jumpPower = false;
        shootPower = false;
        
        if(health_point >= 1) {
            getWorld()->playSound(SOUND_PLAYER_HURT);
        }
        else
            setDead();
    }
}

//Helper function to set the hitpoints for peach
void Peach::setHitPoints(int x) {
    health_point = x;
}

//Adds the shooting power to peach
void Peach::addShooting() {
    shootPower = true;
}

//Adds the jump power to peach
void Peach::addJump() {
    jumpPower = true;
}

//Gives peach the the starPower
void Peach::addStarPower() {
    starPower = true;
    temp_invicibility = 150;
}

//Check if peach has jump
bool Peach::getJump() {
    return jumpPower;
}

//Check if peach has the shoot
bool Peach::getShoot() {
    return shootPower;
}

//Check if peach has the star
bool Peach::getStarPower() {
    return starPower;
}

//See if peach has invincibiltiy
bool Peach::hasInvicincibility() {
    return temp_invicibility > 0;
}

//Set the time peach is inviz for
void Peach::setTimeInviz(int time) {
    temp_invicibility = time;
}

//////////////////////////////Block Class Defintions:///////////////////////////
Block::Block(StudentWorld* sw, int x, int y, bool flower, bool jump, bool star) : BaseActor(sw, x, y, IID_BLOCK, 0, 2, 1.0) {
    containsFlower = flower;
    containsJump = jump;
    containsStar = star;
}

void Block::doSomething()
{}

bool Block::canBeDamaged() {
    return false;
}

void Block::bonk() {
    //If a block has a flower
    if(containsFlower) {
        //Tell the block it no-longer carries the flower
        containsFlower = false;
        //Add the flower object to the world
        getWorld()->addSprite(new Flower(getWorld(), getX(), getY()+SPRITE_HEIGHT));
        
    }
    //If a block has a mushroom
    else if(containsJump) {
        //Tell the block it no longer carries the mushroom
        containsJump = false;
        //Add the mushroom object to the world
        getWorld()->addSprite(new Mushroom(getWorld(), getX(), getY()+SPRITE_HEIGHT));
    }
    //If a block contains a star
    else if(containsStar) {
        //Tell the block it no longer carries the star
        containsStar = false;
        //Add the star object to the world
        getWorld()->addSprite(new Star(getWorld(), getX(), getY()+SPRITE_HEIGHT));
    }
    else {
        getWorld()->playSound(SOUND_PLAYER_BONK);
    }
    
}

//block does prevent objects from moving
bool Block::actsWall() {
    return true;
}

//////////////////////////Pipe Class Definitions////////////////////////////////
Pipe::Pipe(StudentWorld* sw, int x, int y) : BaseActor(sw, x, y, IID_PIPE, 0, 2, 1.0)
{}

void Pipe::doSomething()
{}

bool Pipe::canBeDamaged() {
    return false;
}

//pipe does prevent objects from moving
bool Pipe::actsWall() {
    return true;
}

//////////////////////////FLOWER CLASS DEFINITIONS/////////////////////////////
Flower::Flower(StudentWorld* sw, int x, int y) : Goodie(sw, x, y, IID_FLOWER) {
    getWorld()->playSound(SOUND_POWERUP_APPEARS);
}

void Flower::doSomething() {
    //If colliding with peach
    if(collidePeach(50)) {
        //Add the shoot power to peach
        getWorld()->addShootToPeach();
        return;
    }
    //Move the flower goodie to the left
    moveGoodie();
}

/////////////////////////MUSHROOM CLASS IMPLEMENTATION//////////////////////////
Mushroom::Mushroom(StudentWorld* sw, int x, int y) : Goodie(sw, x, y, IID_MUSHROOM) {
    getWorld()->playSound(SOUND_POWERUP_APPEARS);
}

void Mushroom::doSomething()
{
    //If colliding with peach
    if(collidePeach(75)) {
        //Add the jump power to peach
        getWorld()->addJumpToPeach();
        return;
    }
    //Move the flower goodie to the left
    moveGoodie();
}

//////////////////////IMPLEMENTATION OF THE STAR CLASS//////////////////////////
Star::Star(StudentWorld* sw, int x, int y) : Goodie(sw, x, y, IID_STAR) {
    getWorld()->playSound(SOUND_POWERUP_APPEARS);
}

void Star::doSomething() {
    //If collding with peach
    if(collidePeach(100)) {
        //Add the star power to peach
        getWorld()->addStarToPeach();
        return;
    }
    //Move the star goodie
    moveGoodie();
}


//////////////////IMPLEMENTATION OF THE PEACH FIREBALL CLASS/////////////////////
PeachFireball::PeachFireball(StudentWorld* sw, int x, int y, int dir) : Projectile(sw, x, y, dir, IID_PEACH_FIRE)
{}

void PeachFireball::doSomething() {
    //If the fireball hits something that is not peach
    if(getWorld()->callBonkNonDamagable(getX(), getY(), this)) {
        setDead();
        return;
    }
    
    moveProjectile();

}


//////////////////IMPLEMENTATION OF THE SHELL FIREBALL CLASS/////////////////////
Shell::Shell(StudentWorld* sw, int x, int y, int dir) : Projectile(sw, x, y, dir, IID_SHELL)
{}

void Shell::doSomething() {
    
    //If the shell hits something that is not peach and non Damagable
    if(getWorld()->callBonkNonDamagable(getX(), getY(), this)) {
        setDead();
        return;
    }
    //Continue to move
    moveProjectile();

}


//////////////////IMPLEMENTATION OF THE PIRANHA FIREBALL CLASS/////////////////////
PiranhaFireball::PiranhaFireball(StudentWorld* sw, int x, int y, int dir) : Projectile(sw, x, y, dir, IID_PIRANHA_FIRE)
{}

void PiranhaFireball::doSomething() {
    //If collding with peach
    if(getWorld()->collideWithPeach(getX(), getY())) {
        //Bonk peach
        getWorld()->bonkPeach();
        setDead();
        return;
    }
    
    moveProjectile();
}

//////////////////////IMPLEMENTATION OF THE GOOMBA CLASS///////////////////////
Goomba::Goomba(StudentWorld *sw, int x, int y, int dir) : Enemy(sw, x, y, IID_GOOMBA, dir)
{}

void Goomba::doSomething() {
    if(!isAlive())
        return;
    
    moveEnemy();
}

//////////////////////IMPLEMENTATION OF THE KOOPA CLASS///////////////////////
Koopa::Koopa(StudentWorld *sw, int x, int y, int dir) : Enemy(sw, x, y, IID_KOOPA, dir)
{}

void Koopa::doSomething() {
    if(!isAlive())
        return;
    
    moveEnemy();
}

void Koopa::bonk() {
    setDead();
    getWorld()->increaseScore(100);
    //Create the shell once the koopa has died
    getWorld()->addSprite(new Shell(getWorld(), getX(), getY(), getDirection()));
}

//////////////////////IMPLEMENTATION OF THE PIRANHA CLASS///////////////////////
Piranha::Piranha(StudentWorld *sw, int x, int y, int dir) : Enemy(sw, x, y, IID_PIRANHA, dir) {
    delayToFire = 0;
}

void Piranha::moveEnemy() {
    //Do nothing since the Piranha cannot move
}

void Piranha::addPiranhaFire() {
    //Add new piranaha fireball
    getWorld()->playSound(SOUND_PIRANHA_FIRE);
    getWorld()->addSprite(new PiranhaFireball(getWorld(), getX(), getY(), getDirection()));
    delayToFire = 40;
}

void Piranha::doSomething() {
    if(!isAlive())
        return;
    
    increaseAnimationNumber();
    
    //If the piranha is colliding with peach, bonk peach
    if(getWorld()->collideWithPeach(getX(), getY())) {
        getWorld()->bonkPeach();
        return;
    }
    
    //If peach is on the same level
    if(getY() + 1.5*SPRITE_HEIGHT >= getWorld()->getPeachY() && getY() - 1.5*SPRITE_HEIGHT <= getWorld()->getPeachY()) {
        //Have the piranha face peach
        if(getX() <= getWorld()->getPeachX())
            setDirection(0);
        else
            setDirection(180);
        
        if(delayToFire > 0) {
            delayToFire--;
            return;
        }
        else {
            if(getDirection() == 0) {
                //If peach is within an 64 pixel range fire
                if((getX()+8*SPRITE_WIDTH) > getWorld()->getPeachX())
                    addPiranhaFire();
            }
            else {
                //If peach is within an 64 pixel range fire
                if((getX()-8*SPRITE_WIDTH) < getWorld()->getPeachX())
                    addPiranhaFire();
            }
        }
    }
    else
        return;
    
    
}

void Piranha::bonk() {
    getWorld()->increaseScore(100);
    setDead();
}

//////////////////////IMPLEMENTATION OF THE FLAG CLASS///////////////////////
Flag::Flag(StudentWorld *sw, int x, int y, int dir) : EndMarker(sw, x, y, dir, IID_FLAG)
{}

void Flag::doSomething() {
    if(!isAlive())
        return;
    //If we are colliding with peach, set flag to dead and indicate to the world that the level has been finished
    if(getWorld()->collideWithPeach(getX(), getY())) {
        getWorld()->increaseScore(1000);
        setDead();
        getWorld()->finishLevel();
    }
}

//////////////////////IMPLEMENTATION OF THE MARIO CLASS///////////////////////
Mario::Mario(StudentWorld *sw, int x, int y, int dir) : EndMarker(sw, x, y, dir, IID_MARIO)
{}

void Mario::doSomething() {
    if(!isAlive())
        return;
    //If we are colliding with peach, set mario to dead, and indicate to the world that hte game has been finished
    if(getWorld()->collideWithPeach(getX(), getY())) {
        getWorld()->increaseScore(1000);
        setDead();
        getWorld()->finishGame();
    }
}
