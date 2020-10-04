//
//  Monster.hpp
//  Project3
//
//  Created by Siddharth Ladha on 16/05/20.
//  Copyright © 2020 CS 32. All rights reserved.
//

#ifndef Monster_h
#define Monster_h
#include <stdio.h>
#include <string>
using namespace std;
class Object;
class Dungeon;
class Player;
class Monster{
public:
    Monster(int x, int y, string s, int range);
    virtual ~Monster();
    int getXCoord() const;
    int getYCoord() const;
    void setPosition(int a, int b);
    virtual char whatMonster() const;
    string getName() const;
    virtual string Move(Dungeon*dp, Player*p) = 0;
    int getArmour();
    int getStrength();
    int getDexterity();
    int getSleeptime();
    int getHP();
    void reduceHPby(int x);
    Object* getWeapon();
    void setSleeptime(int x);
    virtual string attackPlayer( Dungeon*dp,Player*p);
    int max(int a, int b) const;
    bool CanMove(int x, int y, Dungeon*dp);
    int getinitialMaxHP() const;
    void setHP(int x);
    int getGoblinRange() const;
private:
    int xCoord;
    int yCoord;
    string name;
    Object* myWeapon; //change to typename Weapon later-->for the inherited classes
    int Armour;
    int Strength;
    int Dexterity;
    int Sleeptime;
    int HP;
    int initialMaxHP;
    int Goblin_Range;
};



class Bogeyman : public Monster{
public:
    Bogeyman(int x, int y, string s, int range);
    virtual char whatMonster() const;
    virtual string Move(Dungeon*dp, Player*p);
private:
};

class Snakewoman : public Monster{
public:
    Snakewoman(int x, int y, string s, int range);
    virtual char whatMonster() const;
    virtual string Move(Dungeon*dp, Player*p);
private:
};

class Dragon: public Monster{
public:
    Dragon(int x, int y, string s, int range);
    virtual char whatMonster() const;
    virtual string Move(Dungeon*dp, Player*p);
private:
};

class Goblin: public Monster{
public:
    void findShortestPath(int x, int y, int a, int b, int depth, int& min_dist_to_player, int array[18][70], Dungeon*dp);//recursive function
    char findOptimumdirection(int array [18][70], int x, int y, int a, int b, int& min_dist_to_player, int depth,Dungeon*dp);//helper
    Goblin(int x, int y, string s, int range);
    virtual char whatMonster() const;
    virtual string Move(Dungeon*dp, Player*p);
private:
};

#endif /* Monster_h*/

bool canSmell(int x, int y, int a, int b, const int distance);

