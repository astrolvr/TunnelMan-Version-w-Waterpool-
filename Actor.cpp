#include "Actor.h"
#include <string>
#include <iostream>
using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

const string earth = "earth";
const string tunnelman = "tunnelman";
const string boulder = "boulder";
const string barrel = "barrel";
const string waterpool = "waterpool";

Actor::Actor(StudentWorld* sw, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth) : GraphObject(imageID, startX, startY, dir, size, depth) {
    m_world = sw;
    m_isAlive = true;
}

Actor::~Actor() {

}
bool Actor::isAlive() {
    return m_isAlive;
}
void Actor::setDead() {
    m_isAlive = false;
}


StudentWorld* Actor::getWorld() {
    return m_world;
}
//void Actor::setWorld(StudentWorld *world) {
//    m_world = world;
//}

Earth::Earth(StudentWorld* sw, int startX, int startY) : Actor(sw, TID_EARTH, startX, startY, right, .25, 3) {
    setVisible(true);
}
void Earth::doSomething() {
    return;
}
Earth::~Earth() {

}

string Earth::classType() {
    return earth;
}


Tunnelman::Tunnelman(StudentWorld* sw) : Actor(sw, TID_PLAYER, 30, 60, right, 1, 0) {
    setVisible(true);
    m_hp = 10;
    m_waterUnits = 5;
    m_numSonar = 1;
    m_numGold = 0;

}
Tunnelman::~Tunnelman() {

}

string Tunnelman::classType() {
    return tunnelman;
}

int Tunnelman::hp() const {
    return m_hp;
}
int Tunnelman::numWater() const {
    return m_waterUnits;
}
void Tunnelman::increaseSquirts(unsigned int howMuch)
{
    m_waterUnits += howMuch;
}
int Tunnelman::numSonar() const {
    return m_numSonar;
}
int Tunnelman::numGold() const {
    return m_numGold;
}

Boulder::Boulder(StudentWorld* sw, int startX, int startY) : Actor(sw, TID_BOULDER, startX, startY, down, 1, 1) {
    setVisible(true);
    m_state = stable;
    waitingNum = 30;

}

Boulder::~Boulder() {
}
string Boulder::classType() {
    return boulder;
}

bool Actor::isCoordinate(int otherX, int otherY) {
    int x = getX();
    int y = getY();
    for (int k = x; k <= x + 3; k++) {
        for (int j = y; j <= y + 3; j++) {
            if (k == otherX && j == otherY) {
                
                return true;
            }
        }
    }
    return false;
}

void Boulder::setState(string state) {
    m_state = state;
}

void Boulder::doSomething() {
    if (!isAlive()) {
        return;
    }
    int x = getX();
    int y = getY();
    //if it's stable, set to waiting.
    if (getState() == stable) {
        if (!getWorld()->isthereEarth(x, y - 1)) {
            setState(waiting);
        }
    }
    if (waitingNum == 0) {
        setState(falling);
        getWorld()->playSound(SOUND_FALLING_ROCK);
        waitingNum = -1;
    }

    if (getState() == falling) {
        // As long as there isn't earth below and y is valid we can move down
        // TODO: check for a bolder
        string type = "";
        if (y - 1 == -1 || getWorld()->isthereEarth(x, y - 1) || getWorld()->isBoulderthere(x, y - 5)) {
            setDead();

        }

        if (!getWorld()->isthereEarth(x, y - 1)) {
            moveTo(x, y - 1);
        }

        // Need to figure out how to not run into other boulders. I think something like we had in Zion like nRobotsAt?? like a function to check if a boulder/object is at that location. Maybe it could take in a class type so we can template it for other classess???

    }
    // TODO: check if it's near a Protestor or Tunnelman & annoy them.
    // Decrement each tick if waiting to fall
    if (getState() == waiting) {
        waitingNum--;
    }
}
Goodies::Goodies(StudentWorld* sw, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth) : Actor(sw, imageID, startX, startY, dir, size, depth) {

}
Goodies::~Goodies() {

}

Barrel::Barrel(StudentWorld* sw, int startX, int startY, Tunnelman* p) : Goodies(sw, TID_BARREL, startX, startY, right, 1, 2) {
    //    m_x = startX;
    //    m_y = startY;
    playerInGame = p;
    //count++;
}
string Barrel::classType() {
    return barrel;
}

void Barrel::doSomething() {
    if (!isAlive()) {
        return;
    }
    int x = playerInGame->getX();
    int y = playerInGame->getY();


    // If not foudn yet, if it's within 4 of tunnelman, found it
    if (found != true) {
        if (getX() - 4 <= x && x <= getX() + 4) {
            if (getY() - 4 <= y && y <= getY() + 4) {
                setVisible(true);
                found = true;
                return;
            }

        }
    }
    
    // If found, set Dead, play sound, increase score, decrement barrels
    else {
        if (getX() - 3 <= x && x <= getX() + 3) {
            if (getY() - 3 <= y && y <= getY() + 3) {
                getWorld()->playSound(SOUND_FOUND_OIL);
                getWorld()->increaseScore(1000);
                getWorld()->decrementBarrelCount();
                setDead(); // here
            }
        }
     
    }

}
void Barrel::makeVisible() {
    found = true;

}
Barrel::~Barrel() {
}
void Barrel::setState(string state) {
    m_state = state;
}



WaterPool::WaterPool(StudentWorld* sw, int startX, int startY, Tunnelman* p) : Goodies(sw, TID_WATER_POOL, startX, startY, right, 1, 2) {
    //    m_x = startX;
    //    m_y = startY;
    playerInGame = p;
    setVisible(true);
    m_state = stable; // stable aka waiting
    //found = false;
     waitingNum = std::max(100, 300 - 10 * (static_cast<int>(getWorld()->getLevel())));
 
}
void WaterPool::setState(std::string state) {
    m_state = state;
}
void WaterPool::doSomething() {
    int x = playerInGame->getX();
    int y = playerInGame->getY();
    if (!isAlive()) {
        return;
    }
    if (waitingNum == 0) {
        setVisible(false);
        setDead();
    } 
    // If found
    //if (getX() == x && getY()== y) {         
    if (getX() - 3 <= x && x <= getX() + 3) {
        if (getY() - 3 <= y && y <= getY() + 3) {
            getWorld()->playSound(SOUND_GOT_GOODIE);
            playerInGame->increaseSquirts(5);
            getWorld()->increaseScore(100);
            setDead(); // ?
        }
    }
    
    waitingNum--;

}
string WaterPool::classType() {
    return waterpool;
}

WaterPool::~WaterPool() {
}





void Tunnelman::doSomething() {

    int ch;
    int x = getX();
    int y = getY();

    if (getWorld()->getKey(ch) == true)
    {
        // user hit a key this tick!
        string type = "";
        switch (ch)
        {
        case KEY_PRESS_LEFT:
            if (this->getDirection() != left) {
                this->setDirection(left);
                break;
            }
            if (x != 0) {
                
                if (!getWorld()->whatsAtThisLocation(x - 1, y)) {
                    //if (type != boulder) {
                        moveTo(x - 1, y);
                    //}
                }
               
              
            }
            break;
        case KEY_PRESS_RIGHT:
            if (this->getDirection() != right) {
                this->setDirection(right);
                break;
            }
            if (x != 60) {
               
                if (!getWorld()->whatsAtThisLocation(x + 1, y)) {
                   // if (type != boulder) {
                        moveTo(x + 1, y);
                    //}
                }
                
               
            }
            break;
        case KEY_PRESS_UP:
            if (this->getDirection() != up) {
                this->setDirection(up);
                break;
            }

            if (y != 60) {
                
                
                if (!getWorld()->whatsAtThisLocation(x, y + 1)) {
                    //if (type != boulder) {
                        moveTo(x, y + 1);
                    //}
                }
                
               
            }

            break;
        case KEY_PRESS_DOWN:
            if (this->getDirection() != down) {
                this->setDirection(down);
                break;
            }
            if (y != 0) {
                
                if (!getWorld()->whatsAtThisLocation(x, y - 1)) {
                    //if (type != boulder) {
                        moveTo(x, y - 1);
                    //}
                }

               
            }
            break;
        case KEY_PRESS_SPACE:

            break;
        case KEY_PRESS_ESCAPE:
            setDead();
            break;
        case none:
            return;
            // etcÖ
        }
        //        Direction d = getDirection();
        //        int x = getX();
        //        int y = getY();

        if (getWorld()->isthereEarth(getX(), getY())) {
            getWorld()->digField(getX(), getY());
            getWorld()->playSound(SOUND_DIG);

        }

    }
}
//void Tunnelman::getLocation() {
//    x = getX();
//    y = getY();
//}