//
//  Monster.cpp
//  Project3
//
//  Created by Siddharth Ladha on 16/05/20.
//  Copyright © 2020 CS 32. All rights reserved.
//
#include <vector>
#include "Actor.h"
#include "Objects.h"
#include "Monster.h"
#include "utilities.h"
#include "Dungeon.h"
#include <algorithm>
Monster::Monster(int x, int y, string s, int range){//Construct member variables based on monster type
    xCoord = x;
    yCoord = y;
    name = s;
    Goblin_Range = range;
    if(name=="Bogeyman"){
       myWeapon = new Weapon("short sword",2,0,getXCoord(),getYCoord()); //change to typename Weapon later-->for the inherited classes
       Armour = 2;
       Strength = randInt(2,3);
       Dexterity = randInt(2,3);
       Sleeptime = 0;
       HP = randInt(5,10);
    }
    else if(name == "Snakewoman"){
        myWeapon = new Weapon("magic fangs of sleep",2,3,xCoord,yCoord); //change to typename Weapon later-->for the inherited classes
        Armour = 3;
        Strength = 2;
        Dexterity = 3;
        Sleeptime = 0;
        HP = randInt(3,6);
    }
    else if(name == "Goblin"){
        myWeapon = new Weapon("short sword",2,0,xCoord,yCoord); //change to typename Weapon later-->for the inherited classes
        Armour = 1;
        Strength = 3;
        Dexterity = 1;
        Sleeptime = 0;
        HP = randInt(15,20);
    }
    else{
    myWeapon = new Weapon("long sword",2,0,xCoord,yCoord); //change to typename Weapon later-->for the inherited classes
        Armour = 4;
        Strength = 4;
        Dexterity = 4;
        Sleeptime = 0;
        HP = randInt(20,25);
    }
    initialMaxHP = HP;
}
Monster::~Monster()
{
    delete myWeapon;
    //cerr << Monster deleted << endl;
}
//accessors for all member variables
int Monster::getXCoord() const{
    return xCoord;
}
int Monster::getYCoord() const{
    return yCoord;
}
//set the Position to a new coordinate
void Monster::setPosition(int a, int b){
    xCoord = a;
    yCoord = b;
}
//set the HP to a new value
void Monster::reduceHPby(int x){
    HP-=x;
}
char Monster::whatMonster() const{
    return 'M';
}
string Monster::getName() const{
    return name;
}
int Monster::getArmour(){
    return Armour;
}
int Monster::getStrength(){
    return Strength;
}

int Monster::getDexterity(){
    return Dexterity;
}
int Monster::getSleeptime(){
    return Sleeptime;
}

int Monster::getHP(){
    return HP;
}
Object* Monster::getWeapon(){
    return myWeapon;
}
void Monster::setSleeptime(int x){
    Sleeptime = x;
}
string Monster::attackPlayer(Dungeon*dp, Player*p){
    string q = myWeapon->getName();
    string response = "the "; //build up the string response of the monster
    response+= getName();
    response+=" ";
    response+=myWeapon->effect();
    response+= " Player";
    int attackerPoints = Dexterity + myWeapon->getdexterity();     // WeaponDexterityBonus;
    int defenderPoints = p->getDexterity() + p->getArmour();
    int damagePoints = randInt(0, Strength + myWeapon->getDamage() - 1); //Weapon damage
    if (randInt(1, attackerPoints) >= randInt(1, defenderPoints)){ //the Monster has struck the Player
        p->reduceHpby(damagePoints);//reduce the player HP;
        if(p->getHP()<=0){ //if it's dead
        response+= " and hits, dealing a final blow."; //player is dead
        }
        else{
                if(myWeapon->getName()=="magic fangs of sleep" && trueWithProbability(1.0/5.0)){
                //account for the player being put to sleep
                    int X = randInt(2,6);
                    int Y = p->getSleeptime();
                    p->setSleeptime(max(X,Y));
                    response+=" and hits, putting ";
                    response+="Player";
                    response+=" to sleep.";
                }
                else{
                response+=" and hits.";
                }
            }
        }//the Monster has missed
        else{
            response += " and misses.";
        }
    return response;
}

int Monster::getinitialMaxHP() const{
    return initialMaxHP;
}
void Monster::setHP(int x){
    HP = x;
}
int Monster::getGoblinRange() const{
    return Goblin_Range;
}

Bogeyman::Bogeyman(int x, int y, string s, int range):Monster(x,y,s,range)
{}
char Bogeyman::whatMonster() const{
    return 'B';
}

string Bogeyman::Move(Dungeon*dp, Player*p){
    int row = getYCoord();
    int column = getXCoord();
    if(getSleeptime()!=0){ //if the monster is not awake, it can't do anything. So just decrease the sleeptime.
        setSleeptime(getSleeptime()-1);
        return "";
    }
    //if the monster is next to a player, attack it
    else if(((row-1) == p->getYCoord() && column == p->getXCoord()) || ((row+1) ==p->getYCoord() && column == p->getXCoord()) ||((row)==p->getYCoord() && (column-1) == p->getXCoord()) || ((row)==p->getYCoord() && (column+1) == p->getXCoord())){
        return attackPlayer(dp, p);
    }
    else if(canSmell(getXCoord(), getYCoord(), p->getXCoord(), p->getYCoord(),5)){ //if the Bogeyman smells the player
        if(CanMove(getXCoord(),getYCoord()-1,dp) && p->getYCoord()<getYCoord()){//moving up gets you closer
            dp->setCell(getYCoord(), getXCoord(), ' '); //old position to empty
            setPosition(getXCoord(),getYCoord()-1);//move up
            dp->setCell(getYCoord(), getXCoord(), whatMonster()); //new position to value
        }
        else if(CanMove(getXCoord(),getYCoord()+1,dp) && p->getYCoord()>getYCoord()){//moving down gets you closer
            dp->setCell(getYCoord(), getXCoord(), ' '); //old position to empty
            setPosition(getXCoord(),getYCoord()+1);//move down
            dp->setCell(getYCoord(), getXCoord(), whatMonster()); //new position to value
        }
        else if(CanMove(getXCoord()-1,getYCoord(),dp) && p->getXCoord()<getXCoord()){//moving left gets you closer
            dp->setCell(getYCoord(), getXCoord(), ' '); //old position to empty
            setPosition(getXCoord()-1,getYCoord());//move left
            dp->setCell(getYCoord(), getXCoord(), whatMonster()); //new position to value
        }
        else if(CanMove(getXCoord()+1,getYCoord(),dp) && p->getXCoord()>getXCoord()){//moving right gets you closer
            dp->setCell(getYCoord(), getXCoord(), ' '); //old position to empty
            setPosition(getXCoord()+1,getYCoord()); //move right
            dp->setCell(getYCoord(), getXCoord(), whatMonster()); //new position to value
        }
        
        
    }
     //move the monster here
    return "";
}


Snakewoman::Snakewoman(int x, int y , string s, int range): Monster(x,y,s,range)
{}
char Snakewoman::whatMonster() const{
    return 'S';
}
string Snakewoman::Move(Dungeon*dp, Player*p){
    int row = getYCoord();
    int column = getXCoord();
    if(getSleeptime()!=0){ //if the monster is not awake, it can't do anything. So just decrease the sleeptime.
        setSleeptime(getSleeptime()-1);
        return "";
    }
    //if the monster is next to a player, attack it
    else if(((row-1) == p->getYCoord() && column == p->getXCoord()) || ((row+1) ==p->getYCoord() && column == p->getXCoord()) ||((row)==p->getYCoord() && (column-1) == p->getXCoord()) || ((row)==p->getYCoord() && (column+1) == p->getXCoord())){
        return attackPlayer(dp, p);
    }
    else if(canSmell(getXCoord(), getYCoord(), p->getXCoord(), p->getYCoord(),3)){ //if the SnakeWoman smells the player
        if(CanMove(getXCoord(),getYCoord()-1,dp) && p->getYCoord()<getYCoord()){//moving up gets you closer
            dp->setCell(getYCoord(), getXCoord(), ' '); //old position to empty
            setPosition(getXCoord(),getYCoord()-1);//move up
            dp->setCell(getYCoord(), getXCoord(), whatMonster()); //new position to value
        }
        else if(CanMove(getXCoord(),getYCoord()+1,dp) && p->getYCoord()>getYCoord()){//moving down gets you closer
            dp->setCell(getYCoord(), getXCoord(), ' '); //old position to empty
            setPosition(getXCoord(),getYCoord()+1);//move down
            dp->setCell(getYCoord(), getXCoord(), whatMonster()); //new position to value
        }
        else if(CanMove(getXCoord()-1,getYCoord(),dp) && p->getXCoord()<getXCoord()){//moving left gets you closer
            dp->setCell(getYCoord(), getXCoord(), ' '); //old position to empty
            setPosition(getXCoord()-1,getYCoord());//move left
            dp->setCell(getYCoord(), getXCoord(), whatMonster()); //new position to value
        }
        else if(CanMove(getXCoord()+1,getYCoord(),dp) && p->getXCoord()>getXCoord()){//moving right gets you closer
            dp->setCell(getYCoord(), getXCoord(), ' '); //old position to empty
            setPosition(getXCoord()+1,getYCoord()); //move right
            dp->setCell(getYCoord(), getXCoord(), whatMonster()); //new position to value
        }
        
        
    }
    return ""; //move the monster here
}


Goblin::Goblin(int x, int y , string s, int range): Monster(x,y,s,range)
{}
char Goblin::whatMonster() const{
    return 'G';
}
string Goblin::Move(Dungeon*dp, Player*p){
    int row = getYCoord();
    int column = getXCoord();
    if(getSleeptime()!=0){ //if the monster is not awake, it can't do anything. So just decrease the sleeptime.
        setSleeptime(getSleeptime()-1);
        return "";
    }
    //if the monster is next to a player, attack it
    else if(((row-1) == p->getYCoord() && column == p->getXCoord()) || ((row+1) ==p->getYCoord() && column == p->getXCoord()) ||((row)==p->getYCoord() && (column-1) == p->getXCoord()) || ((row)==p->getYCoord() && (column+1) == p->getXCoord())){
        return attackPlayer(dp, p);
    }
    else{ //move the goblin here
        int isValid[18][70];
        for(int i =0; i<18; i++){
            for(int j =0; j<70; j++){
                isValid[i][j] = 200; //can't move here
            }
        }
        
        for(int i = 0; i<18; i++){
            for(int j = 0; j<70; j++){
                if(CanMove(j,i,dp))
                    isValid[i][j] = 40; //can move here
            }
        }
        
        isValid[getYCoord()][getXCoord()] = 200;
        isValid[p->getYCoord()][p->getXCoord()] = 40;
//
//        for(int i = 0; i<18; i++){
//            for(int j = 0; j<70; j++){
//            cout << isValid[i][j];
//            }
//            cout << endl;
//        }
        

//        int min_dist_to_player = 40; //random value
        char whichway = 'O';
//        whichway = findOptimumdirection(isValid, getXCoord(),getYCoord(), p->getXCoord(), p->getYCoord(),min_dist_to_player,0,dp);
//        cout <<whichway << endl;
        switch(whichway){
//            case 'O':
//                return "";
//                break;
            case'U':
                dp->setCell(getYCoord(), getXCoord(), ' '); //old position to empty
                setPosition(getXCoord(),getYCoord()-1);//move up
                dp->setCell(getYCoord(), getXCoord(), whatMonster()); //new position to value
                return "";
                break;
            case 'D':
                dp->setCell(getYCoord(), getXCoord(), ' '); //old position to empty
                setPosition(getXCoord(),getYCoord()+1);//move down
                dp->setCell(getYCoord(), getXCoord(), whatMonster()); //new position to value
                return "";
                break;
            case 'R' :
                dp->setCell(getYCoord(), getXCoord(), ' '); //old position to empty
                setPosition(getXCoord()+1,getYCoord()); //move right
                dp->setCell(getYCoord(), getXCoord(), whatMonster()); //new position to value
                return "";
                break;
            case 'L':
                dp->setCell(getYCoord(), getXCoord(), ' '); //old position to empty
                setPosition(getXCoord()-1,getYCoord());//move left
                dp->setCell(getYCoord(), getXCoord(), whatMonster()); //new position to value
                return "";
                break;
            default:
                return "";
                break;
        }

        //now, all the spots the goblin can move to, plus it's position and the player's position, are marked as true.
    return ""; //move the monster here
    }
    
}


Dragon::Dragon(int x, int y , string s,int range): Monster(x,y,s,range)
{}
char Dragon::whatMonster() const{
    return 'D';
}
string Dragon::Move(Dungeon*dp, Player*p){
    //might increase HP
    //Before each turn a dragon takes, there is a 1 in 10 chance the dragon will regain 1 hit point, up to its initial maximum number of hit points, even if it's asleep? yes for now
    if(trueWithProbability(0.1) && getHP()<getinitialMaxHP()){
        setHP(getHP()+1);
    }
        
    int row = getYCoord();
    int column = getXCoord();
    if(getSleeptime()!=0){ //if the monster is not awake, it can't do anything. So just decrease the sleeptime.
        setSleeptime(getSleeptime()-1);
        return "";
    }
    //if the monster is next to a player, attack it
    else if(((row-1) == p->getYCoord() && column == p->getXCoord()) || ((row+1) == p->getYCoord() && column == p->getXCoord()) ||((row)==p->getYCoord() && (column-1) == p->getXCoord()) || ((row)==p->getYCoord() && (column+1) == p->getXCoord())){
        return attackPlayer(dp, p);
    }
    else{
    return ""; //dragon's don't move!
    }
    
}
int Monster::max(int a, int b) const{
    if(a>b)
        return a;
    else
        return b;
}



bool canSmell(int x, int y, int a, int b, const int distance){ //return true if the monster can smell the player
    for(int i =0; i<=distance; i++ ){
        if(x+i == a || x-i == a){
            for(int j =0; j<=distance-i; j++){
            if(y+j == b || y-j == b)
                return true;
            }
        }
        else if(y+i==b || y-i == b){
            for(int j =0; j<=distance-i; j++){
            if(x+j == a || x-j == a)
                return true;
            }
        }
    }
    return false;
}

bool Monster::CanMove(int x, int y, Dungeon*dp){
    if(dp->getCell(y, x) == ' ' || dp->getCell(y, x) == '>' || dp->getCell(y, x) == '&' || dp->getCell(y, x) == ')' || dp->getCell(y, x) ==  '?'){
        return true;
    }
    return false;
}


void Goblin::findShortestPath(int x, int y, int a, int b, int depth, int& min_dist_to_player, int array[18][70],Dungeon*dp){
   
    if((x==a) && (y==b)){
        min_dist_to_player = min(depth,min_dist_to_player);
        return;
    }
        
    if(depth>15) //if you've taken more than 15 steps, return immediateley
        return;
    
    array[y][x] = depth;
    //check up
    if(CanMove(x,y-1,dp) || dp->getCell(y-1, x) == '@')
    findShortestPath(x, y-1, a, b, (depth+1), min_dist_to_player, array,dp);
    //check down
    if(CanMove(x,y+1,dp) || dp->getCell(y+1, x) == '@')
    findShortestPath(x,y+1,a,b,(depth+1), min_dist_to_player ,array,dp);
    //check to the right
    if(CanMove(x+1,y,dp) || dp->getCell(y, x+1) == '@')
    findShortestPath(x+1, y, a, b, (depth+1), min_dist_to_player, array,dp);
    //check to the left
    if(CanMove(x-1,y,dp) || dp->getCell(y, x-1) == '@')
    findShortestPath(x-1, y, a, b, (depth+1), min_dist_to_player, array,dp);
    
    // else
        //return -1;
}
char Goblin::findOptimumdirection(int array[18][70], int x, int y, int a, int b, int& min_dist_to_player, int depth, Dungeon*dp){
    int r1 = 40, r2 = 40, r3 = 40, r4 = 40;
    if(CanMove(x,y-1,dp) || dp->getCell(y-1, x) == '@'){
        findShortestPath(x, y-1, a, b, 0, min_dist_to_player, array,dp); //moving up
        r1 = min_dist_to_player;
    }
    min_dist_to_player = 40;
    if(CanMove(x,y+1,dp) || dp->getCell(y+1, x) == '@'){
        findShortestPath(x,y+1,a,b,0, min_dist_to_player ,array,dp); //moving down
        r2 = min_dist_to_player;
    
    }
    min_dist_to_player = 40;
    if(CanMove(x+1,y,dp) || dp->getCell(y, x+1) == '@'){
    findShortestPath(x+1, y, a, b,0, min_dist_to_player, array,dp); //moving right
        r3 = min_dist_to_player;
    }
    min_dist_to_player = 40;
    if(CanMove(x-1,y,dp) || dp->getCell(y, x-1) == '@'){
    findShortestPath(x-1, y, a, b,0, min_dist_to_player, array,dp); //moving left
        r4 = min_dist_to_player;
    }
    min_dist_to_player = 40;
    vector<int> collection;
    collection.push_back(r1);
    collection.push_back(r2);
    collection.push_back(r3);
    collection.push_back(r4);
    sort(collection.begin(),collection.end());
    if(r1 == collection[0] && collection[0]<15){
        return 'U';
    }
    if(r2 == collection[0] && collection[0]<15){
        return 'D';
    }
    if(r3 == collection[0] && collection[0]<15){
        return 'R';
    }
    if(r4 == collection[0] && collection[0]<15){
        return 'L';
    }
    return 'O';
    
    
}
