//
//  Dungeon.cpp
//  Project3
//
//  Created by Siddharth Ladha on 12/05/20.
//  Copyright © 2020 CS 32. All rights reserved.
//
#include <vector>
#include "Monster.h"
#include <iostream>
#include "utilities.h"
#include "Dungeon.h"
#include "Actor.h"
#include <list>
#include <algorithm>
#include <cmath>
using namespace std;
Dungeon::Dungeon( int n, int smell)
: level(n), T(), smellDistanceforGoblin(smell)
{
    //initially, every cell is a wall
    for(int i =0; i<18; i++){
              for (int j =0; j<70; j++){
                    maze[i][j] = '#';
              }
          }
    //new code for creating a more complex maze
    vector<Room> myRooms;
    //create the first room
    for(;;){
    Room r1;
        if(isInBounds(r1)){
            myRooms.push_back(r1);
            break;
        }
    }
    //create the other rooms
    int numberofRooms = randInt(4,6);
    while(myRooms.size()<numberofRooms){
        bool isValidwiththeOtherRooms = true;
        Room r2;
        vector<Room>::iterator it = myRooms.begin();
        while(it!= myRooms.end()){ //for any of the premade rooms
            if(donotOverlap(r2, *it) == false){ //if r2 overlaps with anyone of them
                isValidwiththeOtherRooms = false; //then r2 is not a valid room. setisValidwiththeOtherrooms to false, and break.
                break;
            }
            it++;
        }
        //if isValidwiththeOtherRooms is true, then r2 does not overlap with any of the previous rooms.
        if(isInBounds(r2) && isValidwiththeOtherRooms){
            myRooms.push_back(r2);
            
        }
    }
    
    
   
    //all the rooms have now been designated. now, we need to draw them.
    vector<Room>::iterator it = myRooms.begin();
    while(it!=myRooms.end()){
        Room a = *it;
        for(int i = a.topLefty; i<=a.bottomrighty; i++){
            for(int j = a.topLeftx; j<=a.bottomrightx; j++){
                maze[i][j] = ' ';
            }
        }
        it++;
    }
    //draw the corridoors.
    //first, sort the list of rooms from left to right
    sort(myRooms.begin(),myRooms.end(),&Customcompare);
    vector<Room>::iterator p = myRooms.begin();
    while(p!=myRooms.end()-1){
     Room r1 = *p;
     p++;
     Room r2 = *(p);
     if(r1.midy>=r2.midy){ //r1 is below r2, we need to connect up and right
    //connect the midpoints of the two rooms together to make a corridoor
         int x = r1.midx;
         int y = r1.midy;
         while(y!=r2.midy){
         maze[y][r1.midx] = ' ';
             y--;
         }
         while(x!=r2.midx){
         maze[y][x] = ' ';
             x++;
         }
     }
     else{//we need to connect down and right
         int x = r1.midx;
         int y = r1.midy;
         while(y!=r2.midy){
             maze[y][r1.midx] = ' ';
             y++;
         }
         while(x!=r2.midx){
             maze[y][x] = ' ';
             x++;
         }
         
         }
     }

   // cout << myRooms.size() << endl;
    ///////////////////////////////////////////////////////////////////create the maze
    /*
    for(int i =0; i<18; i++)
        for (int j =0; j<70; j++)
            maze[i][j] = ' ';

    for(int i =0; i<70; i++)
        maze[0][i] = '#';
    for(int i =0; i<70; i++)
        maze[17][i] = '#';
    for(int i =0; i<18; i++)
        maze[i][0] = '#';
    for(int i =0; i<18; i++)
        maze[i][69] = '#';
    ////////////////////////////////////////////////////////////////all this needs to be gone later
     */
    
    //set the Position of the ladder and the Idol
    for(;;)
    {
        int y = randInt(1,68);
        int x = randInt(1,16);
        if(maze[x][y] == ' '){
                if(n!=4)
                    maze[x][y] = '>';
                else
                    maze[x][y] = '&';
            T.setPosition(y, x);
            break;
        }
            
    }
    //set the position of the objects
    int numberofObjects = randInt(2,3);
    int i =0;
    int x,y =0;
    while(i!=numberofObjects){
        for(;;)//get a set of valid coordinates
           {
               y = randInt(1,68); //change this to make it valid for the more complex maze
               x = randInt(1,16);
               if(maze[x][y] == ' ' || maze[x][y] == '@'){
                   break;
               }
                   
           }
        int j = randInt(1,7); //randomly pick a weapon or scroll from one of 7 possibilities and add it to your DungeonInventory
        if(j==1){
            Object*p = new Weapon("short sword",2,0,y,x);
            DungeonInventory.push_back(p);
            maze[x][y] = ')';
        }
        else if(j==2){
            Object*p = new Weapon("long sword",2,0,y,x);
            DungeonInventory.push_back(p);
            maze[x][y] = ')';
        }
        else if(j==3){
            Object*p = new Weapon("mace",2,0,y,x);
            DungeonInventory.push_back(p);
            maze[x][y] = ')';
        }
        else if(j==4){
            Object*p = new Scroll("scroll of enhance armor",y,x,-1,-1);
            DungeonInventory.push_back(p);
            maze[x][y] = '?';
        }
        else if(j==5){
            Object*p = new Scroll("scroll of strength",y,x,-1,-1);
            DungeonInventory.push_back(p);
            maze[x][y] = '?';
        }
        else if(j==6){
            Object*p = new Scroll("scroll of enhance health",y,x,-1,-1);
            DungeonInventory.push_back(p);
            maze[x][y] = '?';
        }
        else if(j==7){
        Object*p = new Scroll("scroll of enhance dexterity",y,x,-1,-1);
        DungeonInventory.push_back(p);
        maze[x][y] = '?';
        }
        
    i++;
    }
    //set the position of the monsters, depending on the level
    int numberofMonsters = randInt(2, 5*(level+1)+1);
    if(level==0 || level == 1){
        int m = 0;
        int a,b = 0;
        while(m!=numberofMonsters){
            for(;;){//find a valid set of coordinates
                b = randInt(1,68); //change this to make it valid for the more complex maze
                a = randInt(1,16);
                if(maze[a][b] == ' ' || maze[a][b] == '>'|| maze[a][b] == '&' || maze[a][b] == ')' || maze[a][b] == '?'){
                    break;
                }
                    
            }
            int j = randInt(1,2);
            if(j==1){
                Monster*p = new Snakewoman(b,a,"Snakewoman",smellDistanceforGoblin);
                ListofMonsters.push_back(p);
                maze[a][b] = 'S';
            }
            else if(j==2){
            Monster*p = new Goblin(b,a,"Goblin",smellDistanceforGoblin);
            ListofMonsters.push_back(p);
            maze[a][b] = 'G';
            }
          m++;
        }
    }
  
    else if(level == 2){
        int m = 0;
        int a,b = 0;
        while(m!=numberofMonsters){
             for(;;){//find a valid set of coordinates
                 b = randInt(1,68); //change this to make it valid for the more complex maze
                 a = randInt(1,16);
                 if(maze[a][b] == ' ' || maze[a][b] == '>'|| maze[a][b] == '&' || maze[a][b] == ')' || maze[a][b] == '?'){
                     break;
                 }
                     
             }
             int j = randInt(1,3);
             if(j==1){
                 Monster*p = new Snakewoman(b,a,"Snakewoman",smellDistanceforGoblin);
                 ListofMonsters.push_back(p);
                 maze[a][b] = 'S';
             }
             else if(j==2){
             Monster*p = new Goblin(b,a,"Goblin",smellDistanceforGoblin);
             ListofMonsters.push_back(p);
             maze[a][b] = 'G';
             }
             else if(j==3){
             Monster*p = new Bogeyman(b,a,"Bogeyman",smellDistanceforGoblin);
             ListofMonsters.push_back(p);
             maze[a][b] = 'B';
             }
           m++;
         }
    }
    
    else if (level == 3 || level == 4){
        int m = 0;
        int a,b = 0;
        while(m!=numberofMonsters){
            for(;;){//find a valid set of coordinates
                 b = randInt(1,68); //change this to make it valid for the more complex maze
                 a = randInt(1,16);
                 if(maze[a][b] == ' ' || maze[a][b] == '>'|| maze[a][b] == '&' || maze[a][b] == ')' || maze[a][b] == '?'){
                     break;
                 }
                     
             }
             int j = randInt(1,4);
             if(j==1){
                 Monster*p = new Snakewoman(b,a,"Snakewoman",smellDistanceforGoblin);
                 ListofMonsters.push_back(p);
                 maze[a][b] = 'S';
             }
             else if(j==2){
             Monster*p = new Goblin(b,a,"Goblin",smellDistanceforGoblin);
             ListofMonsters.push_back(p);
             maze[a][b] = 'G';
             }
             else if(j==3){
            Monster*p = new Bogeyman(b,a,"Bogeyman",smellDistanceforGoblin);
            ListofMonsters.push_back(p);
            maze[a][b] = 'B';
             }
             else if(j==4){
             Monster*p = new Dragon(b,a,"Dragon",smellDistanceforGoblin);
             ListofMonsters.push_back(p);
             maze[a][b] = 'D';
             }
           m++;
         }
    }
    
}

//why does removing this work

Dungeon::~Dungeon() //get rid of all the newed items in the DungeonInventory + The monsters later on
{
    list<Object*>::iterator it = DungeonInventory.begin();
    while(it!=DungeonInventory.end()){
        delete (*it);
        it = DungeonInventory.erase(it);
    }
    list<Monster*>::iterator arrow = ListofMonsters.begin();
    while(arrow!=ListofMonsters.end()){
        delete(*arrow);
        arrow = ListofMonsters.erase(arrow);
    }
    
  //cerr<<"Dungeon deleted" << endl;
}


void Dungeon::display(Player*p){
    //reset all actors to blanks so that we can update them to their new positions->players+monsters
    for(int i =0; i<18; i++){
    for(int j =0; j<70; j++){
    if(maze[i][j]=='@' || maze[i][j]=='S' || maze[i][j]=='D' || maze[i][j]=='G' || maze[i][j]=='B')
        maze[i][j] = ' ';
    }
    }
    
    //display the position of the idol or the staircase
    if(level==4)
    maze[T.getyCoord()][T.getxCoord()] = '&';
    else
    maze[T.getyCoord()][T.getxCoord()] = '>';
    
    //display the weapons and scrolls using dynamic cast
    //only displays the items in the Dungeon Inventory. So we're good.
    list<Object*>::iterator it = DungeonInventory.begin();
    while(it!=DungeonInventory.end()){
        if ((*it)->value() =='W')  // op really points to a Weapon
            maze[(*it)->getY()][(*it)->getX()] = ')';
        else if((*it)->value()=='S') // means op pointed to a scroll
            maze[(*it)->getY()][(*it)->getX()] = '?';
        it++;
    }
    
    
    //display the player, along with the monsters. This must be done after printing out the Objects, since actors can move on top of objects
    list<Monster*>::iterator arrow = ListofMonsters.begin();
    while(arrow!=ListofMonsters.end()){
        Monster*m = *arrow;
        maze[m->getYCoord()][m->getXCoord()] = m->whatMonster();
        arrow++;
    }
    maze[p->getYCoord()][p->getXCoord()]  = '@';//set everything else here
    for(int i =0; i<18; i++){
        for(int j =0; j<70; j++){
            cout << maze[i][j];
        }
        cout << endl;
    }
   // cout << endl;
    cout << "Dungeon Level: " << level << ", Hit points: " << p->getHP() << ", Armor: " << p->getArmour() << ", Strength: " << p->getStrength() << ", Dexterity: " << p->getDexterity() << endl;
}

char Dungeon::getCell(int r, int c) const{
    return maze[r][c];
}

void Dungeon::setCell(int r, int c, char value){
    maze[r][c] = value;
    return;
}

int Dungeon::getLevel() const{
    return level;
}
ladderOrIdol Dungeon::getLadder() const{
    return T;
}

list<Object*>& Dungeon::getDungeonInventory(){
    return DungeonInventory;
}
list<Monster*>& Dungeon::getListofMonsters(){
    return ListofMonsters;
}

bool Dungeon::hasObject(int x, int y){
    if(T.getxCoord()==x && T.getyCoord()==y)//there's a ladder or an idol there
        return true;
    list<Object*>::iterator it = DungeonInventory.begin();
    while(it!=DungeonInventory.end()){
        Object*op = *it;
        if(op->getX()==x && op->getY()==y) //there's a weapon or a scroll there
            return true;
        it++;
    }
        return false;
}


Room::Room(){
    topLeftx = randInt(1,68);
    topLefty = randInt(1,16);
    width = randInt(5,12);
    depth = randInt(3,6);
    bottomrightx = topLeftx+width;
    bottomrighty = topLefty+depth;
    midx = (topLeftx+bottomrightx)/2;
    midy = (topLefty+bottomrighty)/2;
}

bool donotOverlap(Room a, Room b){
      // no row overlap and no column overlap
    if ((a.topLeftx > b.bottomrightx+1 || b.topLeftx > a.bottomrightx+1) &&  (a.bottomrighty> b.topLefty+1 || b.bottomrighty> a.topLefty+1))
          return true;
    
      //otherwise, they overlap
    else
          return false;
}

bool isInBounds(Room a){
    if(a.bottomrightx<=68 && a.bottomrighty<=16)
        return true;
    else
        return false;
}

bool Customcompare(const Room& a, const Room&b){
    if(a.topLeftx<b.topLeftx)
        return true;
    else
    return false;
    
}
