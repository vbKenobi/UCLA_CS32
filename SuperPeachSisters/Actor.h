#ifndef ACTOR_H_
#define ACTOR_H_

#include "StudentWorld.h"
#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class BaseActor : public GraphObject
{
  public:
    //Housekeeping functions
    BaseActor(StudentWorld* sw, int x, int y, int nameID, int dir = 0, int depth = 1, double size = 1.0);
    
    //Indicates what to do at each tick
    virtual void doSomething() = 0;
    //Return the world
    StudentWorld* getWorld();
    
    //Check if alive
    bool isAlive();
    //Check if can be damaged
    virtual bool canBeDamaged();
    //Set the status of actor to dead
    void setDead();
    
    //Bonk the actor
    virtual void bonk();
    //Check if the object blocks movement
    virtual bool actsWall();
    
  private:
    StudentWorld* m_world;
    bool damaged;
    bool alive;
    
};

///////////////////////DERIVATIONS OF THE BASEACTOR CLASS//////////
class Goodie : public BaseActor
{
  public:
    Goodie(StudentWorld* sw, int x, int y, int ID);
    //Determines how a goodie must move
    void moveGoodie();
    //Determiens if there is a collision with peach
    bool collidePeach(int points);
    virtual void doSomething() = 0;
    //Tells that a goodie cannot be damaged
    bool canBeDamaged();
};

class Enemy : public BaseActor
{
  public:
    Enemy(StudentWorld* sw, int x, int y, int ID, int dir);
    //Determines how a enemy must move
    virtual void moveEnemy();
    //Tells that an enemy can be damaged
    bool canBeDamaged();
    virtual void doSomething() = 0;
    //Tells how each enemy acts when bonked
    void bonk();
};

class Projectile : public BaseActor
{
  public:
    Projectile(StudentWorld* sw, int x, int y, int dir, int ID);
    virtual void doSomething() = 0;
    //Determines how a projectile must move
    virtual void moveProjectile();
    //States a projectile cannot be damaged
    bool canBeDamaged();
};

class EndMarker : public BaseActor
{
  public:
    EndMarker(StudentWorld* sw, int x, int y, int dir, int ID);
    virtual void doSomething() = 0;
    //States a endMarker cannot be damaged
    bool canBeDamaged();
};


class Peach : public BaseActor
{
  public:
    Peach(StudentWorld* sw, int x, int y);
    virtual void doSomething();
    //Setter and Getter functions
    void addShooting();
    void addJump();
    void addStarPower();
    bool hasInvicincibility(); 
    bool getShoot();
    bool getJump();
    bool getStarPower();
    
    virtual void bonk();
    
    void setHitPoints(int x);
    void setTimeInviz(int time); 
    
  private:
    int health_point;
    int jump_distance;
    int time_to_next_shot;
    int temp_invicibility;
    bool starPower;
    bool shootPower;
    bool jumpPower;
};

class Block : public BaseActor
{
  public:
    Block(StudentWorld* sw, int x, int y, bool containsFlower, bool jump, bool star);
    virtual void doSomething();
    virtual bool canBeDamaged();
    virtual void bonk();
    virtual bool actsWall();
    
  private:
    bool containsFlower = false;
    bool containsJump = false;
    bool containsStar = false;
};


class Pipe : public BaseActor
{
  public:
    Pipe(StudentWorld* sw, int x, int y);
    virtual void doSomething();
    virtual bool canBeDamaged();
    virtual bool actsWall();
    
  private:
};


class Flower : public Goodie
{
  public:
    Flower(StudentWorld* sw, int x, int y);
    virtual void doSomething();
};

class Mushroom : public Goodie
{
  public:
    Mushroom(StudentWorld* sw, int x, int y);
    virtual void doSomething();
};

class Star : public Goodie
{
  public:
    Star(StudentWorld* sw, int x, int y);
    virtual void doSomething();
};

class PeachFireball : public Projectile
{
  public:
    PeachFireball(StudentWorld* sw, int x, int y, int dir);
    virtual void doSomething();
};

class PiranhaFireball : public Projectile
{
  public:
    PiranhaFireball(StudentWorld* sw, int x, int y, int dir);
    virtual void doSomething();
};

class Shell : public Projectile
{
  public:
    Shell(StudentWorld* sw, int x, int y, int dir);
    virtual void doSomething();
};

class Goomba : public Enemy
{
  public:
    Goomba(StudentWorld* sw, int x, int y, int dir);
    virtual void doSomething();
};

class Koopa : public Enemy
{
  public:
    Koopa(StudentWorld* sw, int x, int y, int dir);
    virtual void doSomething();
    virtual void bonk();
};

class Piranha : public Enemy
{
  public:
    Piranha(StudentWorld* sw, int x, int y, int dir);
    virtual void doSomething();
    virtual void bonk();
    virtual void moveEnemy();
    void addPiranhaFire();
    
  private:
    int delayToFire;
};


class Flag : public EndMarker
{
  public:
    Flag(StudentWorld* sw, int x, int y, int dir);
    virtual void doSomething();
};


class Mario : public EndMarker
{
  public:
    Mario(StudentWorld* sw, int x, int y, int dir);
    virtual void doSomething();
};

#endif // ACTOR_H_
