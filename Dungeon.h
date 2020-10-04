//
//  Dungeon.hpp
//  Project3
//
//  Created by Siddharth Ladha on 12/05/20.
//  Copyright © 2020 CS 32. All rights reserved.
//

#ifndef Dungeon_h
#define Dungeon_h
#include "Objects.h"
#include <stdio.h>
#include <list>
#include <vector>
class Monster;
class Player;
class Dungeon{
public:
    Dungeon( int n, int smell);
    ~Dungeon();
    void setCell(int r, int c, char value);
    char getCell(int r, int c) const;
    void display(Player*p);
    int getLevel() const;
    ladderOrIdol getLadder() const;
    bool hasObject(int x, int y);
    list<Object*>& getDungeonInventory();
    list<Monster*>& getListofMonsters();
private:
    char maze[18][70];
    int level;
    ladderOrIdol T;
    list<Object*> DungeonInventory;
    list<Monster*> ListofMonsters;
    int smellDistanceforGoblin;
    //experimental
};
struct Room{
public:
    Room();
    int topLeftx;
    int topLefty;
    int bottomrightx;
    int bottomrighty;
    int width;
    int depth;
    int midx;
    int midy;
};
bool donotOverlap(Room a, Room b);
bool isInBounds(Room a);
bool Customcompare(const Room& a, const Room&b);

#endif /* Dungeon_h */

