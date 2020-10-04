//
//  Actor.hpp
//  Project3
//
//  Created by Siddharth Ladha on 13/05/20.
//  Copyright © 2020 CS 32. All rights reserved.
//

#ifndef Actor_h
#define Actor_h
#include <list>
#include <stdio.h>
#include <iostream>
class Object;
class Dungeon;
using namespace std;
class Player{
public:
    Player(Dungeon*dp);
    ~Player();
    string move(char Direction);
    string attack(char Direction);
    string PickUp();
    string wieldWeapon();
    string readScroll();
    void seeInventory();
    void Quit();
    string Cheat();
    int getXCoord() const;
    int getYCoord() const;
    bool isonStaircaseorIdol() const;
    bool isAbouttoMoveonMonster(char Direction) const;
   // bool isonWeapon() const;
    bool isonObject() const;
    void setPosition(int a, int b);
    void setDungeon(Dungeon*dp);
    int getHP() const;
    int getArmour() const;
    int getStrength() const;
    int getDexterity() const;
    int getMaxHP() const;
    void mightincreaseHP();
    int max(int a, int b) const;
    int getSleeptime() const;
    void setSleeptime(int x);
    bool isPlayerDead() const;
    void reduceHpby(int x);
private:
    Dungeon* mDungeon;
    int xCoord;
    int yCoord;
    int level;
    int HP;
    int maxHealth;
    Object* myWeapon; //change to typename Weapon later
    int Armour;
    int Strength;
    int Dexterity;
    int Sleeptime;
    list<Object*> Inventory; //change to typename "non-actor later"
    //in testing
};
#endif /* Actor_h */
