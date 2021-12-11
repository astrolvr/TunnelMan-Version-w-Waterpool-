#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

const std::string stable = "stable";
const std::string waiting = "waiting";
const std::string falling = "falling";
const std::string temp = "temp";

/// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject {
public:
    Actor(StudentWorld* sw, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);
    ~Actor();
    StudentWorld* getWorld();
    virtual void doSomething() = 0;
    bool isAlive();
    void setDead();
    bool isCoordinate(int otherX, int otherY);
    virtual std::string classType() = 0;

private:
    StudentWorld* m_world;

    bool m_isAlive;
};

class Earth : public Actor {
public:
    Earth(StudentWorld* sw, int startX, int startY);
    ~Earth();
    void doSomething();
    std::string classType();
};


class Tunnelman : public Actor {
public:
    Tunnelman(StudentWorld* sw);
    void doSomething();
    void getLocation();
    ~Tunnelman();
    std::string classType();

    int hp() const;
    int numWater() const;
    int numSonar() const;
    int numGold() const;
    void increaseSquirts(unsigned int howMuch);

private:
    int m_hp;
    int m_waterUnits;
    int m_numSonar;
    int m_numGold;
    int x;
    int y;


};

class Boulder : public Actor {
public:
    Boulder(StudentWorld* sw, int startX, int startY);
    ~Boulder();
    virtual void doSomething();
    std::string getState() {
        return m_state;
    }
    void setState(std::string state);
    std::string classType();

private:
    std::string m_state;
    int waitingNum;
    int x;
    int y;

};


// Base class for pick-uppable items: Sonar kits, Water, Gold nuggets
class Goodies : public Actor {
public:
    Goodies(StudentWorld* sw, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);
    ~Goodies();
private:

};

class Barrel : public Goodies {
public:
    Barrel(StudentWorld* sw, int startX, int startY, Tunnelman* p);
    ~Barrel();
    void doSomething();
    void setState(std::string state);
    void makeVisible();
    std::string classType();

private:
    std::string m_state;
    int m_x;
    int m_y;
    bool found;
    int count;
    Tunnelman* playerInGame;
};

class WaterPool : public Goodies {
public:
    WaterPool(StudentWorld* sw, int startX, int startY, Tunnelman* p);
    ~WaterPool();
    void setState(std::string state);
    void doSomething();
    std::string classType();
    std::string getID();
private:
    std::string m_state;
    int m_x;
    int m_y;
    //bool found;
    int waitingNum;
    Tunnelman* playerInGame;
};


class Protestor : public Actor {
public:
    Protestor(StudentWorld* sw, int startX, int startY, Tunnelman* p);
    ~Protestor();

};

#endif // ACTOR_H_
