//
//  Actor.cpp
//  Project3
//
//  Created by Siddharth Ladha on 13/05/20.
//  Copyright © 2020 CS 32. All rights reserved.
//
#include "Dungeon.h"
#include "utilities.h"
#include "Actor.h"
#include "Monster.h"
int Player::getXCoord() const{ //column value
    return xCoord;
}
int Player::getYCoord() const{ //row value
    return yCoord;
}
void Player::setPosition(int a, int b)
{
    xCoord = a;
    yCoord = b;
}

Player::Player(Dungeon*dp)
{
    int x =0;
    int y = 0;
    for(;;){
        y = randInt(1,68); //change this to make sure you're placing on an empty spot in the board+account for monsters, etc.
        x = randInt(1,16);
        if(dp->getCell(x, y)== ' ' || dp->getCell(x, y)=='>' || dp->getCell(x, y)=='&' || dp->getCell(x, y)== '?' || dp->getCell(x, y)== ')')
            break;
    }
        xCoord = y;
        yCoord = x;
        

mDungeon = dp;
level = 0;
maxHealth = 20;
HP = 20;
myWeapon =  new Weapon("short sword",2,0,xCoord,yCoord); //change to typename Weapon later
Armour = 2;
Strength = 2;
Dexterity = 2;
Sleeptime = 0;
Inventory.push_back(myWeapon); //change to typename "non-actor later" //make it so that a player has to be created on a blank spot
}

string Player::move(char Direction){
    if(isAbouttoMoveonMonster(Direction)){ //if you're about to move on a monster, the attack it, and return the result of that attack
        string answer = attack(Direction);
        return answer;
    }
    else{ //otherwise, move to the desired position, provided there's no walls or monsters there
    switch(Direction){
        case ARROW_LEFT:
            if((mDungeon->getCell(yCoord, xCoord-1)) == ' ' || (mDungeon->getCell(yCoord, xCoord-1) == '>' ) || (mDungeon->getCell(yCoord, xCoord-1) == '&') || (mDungeon->getCell(yCoord, xCoord-1)) == ')' || (mDungeon->getCell(yCoord, xCoord-1)) == '?'){
                mDungeon->setCell(yCoord, xCoord, ' '); //experimental
                xCoord--;
                mDungeon->setCell(yCoord, xCoord, '@');
            }
            break;
        case ARROW_RIGHT:
            if((mDungeon->getCell(yCoord, xCoord+1)) == ' ' || (mDungeon->getCell(yCoord, xCoord+1) == '>' ) || (mDungeon->getCell(yCoord, xCoord+1) == '&') || (mDungeon->getCell(yCoord, xCoord+1) == ')') || (mDungeon->getCell(yCoord, xCoord+1) == '?') ){
                mDungeon->setCell(yCoord, xCoord, ' '); //experimental
                xCoord++;
                mDungeon->setCell(yCoord, xCoord, '@');
            }
            break;
        case ARROW_UP:
            if((mDungeon->getCell(yCoord-1, xCoord)) == ' ' || (mDungeon->getCell(yCoord-1, xCoord) == '>' ) || (mDungeon->getCell(yCoord-1, xCoord) == '&') || (mDungeon->getCell(yCoord-1, xCoord) == ')') || (mDungeon->getCell(yCoord-1, xCoord) == '?')){
                mDungeon->setCell(yCoord, xCoord, ' '); //experimental
                yCoord--;
                mDungeon->setCell(yCoord, xCoord, '@');
            }
            break;
        case ARROW_DOWN:
            if((mDungeon->getCell(yCoord+1, xCoord)) == ' ' || (mDungeon->getCell(yCoord+1, xCoord) == '>' ) || (mDungeon->getCell(yCoord+1, xCoord) == '&') || (mDungeon->getCell(yCoord+1, xCoord) == ')') || (mDungeon->getCell(yCoord+1, xCoord) == '?')){
                mDungeon->setCell(yCoord, xCoord, ' ');//experimental
                yCoord++;
                mDungeon->setCell(yCoord, xCoord, '@');
            }
            break;
    }
        return "";
    }
}

Player::~Player()
{
    //what happens to the Weapon pointer?figure out solution
    list<Object*>::iterator it = Inventory.begin();
    while(it!=Inventory.end()){
        if((*it)!=myWeapon) //to avoid double deletion, since this pointer and myWeapon point to the same object in the heap
        delete (*it);
        it = Inventory.erase(it);
    }
    delete mDungeon;
    delete myWeapon; //delete the myWeapon
    //
   //cerr<<"Player deleted" << endl;
}

void Player::setDungeon(Dungeon*dp){
mDungeon = dp;
}

bool Player::isonStaircaseorIdol() const{
   // cout<<mDungeon->getCell(yCoord, xCoord) << endl;; //this is the error right here!
    return((xCoord == mDungeon->getLadder().getxCoord()) && (yCoord == mDungeon->getLadder().getyCoord()));
}

int Player::getHP() const{
    return HP;
}
int Player::getArmour() const{
    return Armour;
}
int Player::getStrength() const{
    return Strength;
}
int Player::getDexterity() const{
    return Dexterity;
}

bool Player::isonObject() const{ //how does returning a list work, in terms of memory allocation? I'm not modifying anything here, so it shouldn't cause any problems.
    //maybe this should be inside
    //special case for when you're on a idol
if(xCoord == mDungeon->getLadder().getxCoord() && yCoord == mDungeon->getLadder().getyCoord() && mDungeon->getLevel()==4)
    return true;
list<Object*>::iterator it = mDungeon->getDungeonInventory().begin();
    while(it!=mDungeon->getDungeonInventory().end()){
    if((*it)->getX()==xCoord && (*it)->getY()==yCoord)
        return true;
        it++;
    }
    return false;
}
string Player::PickUp(){
    //if you're on the last level, and you're standing on the idol, then calling Pickup means you won
    if(xCoord == mDungeon->getLadder().getxCoord() && yCoord == mDungeon->getLadder().getyCoord() && mDungeon->getLevel()==4){
        return "Congratulations, you won!";
    }
    if(Inventory.size()>25) //can't pick up anymore items
        return "Your knapsack is full; you can't pick that up.";
    
    list<Object*>::iterator it = mDungeon->getDungeonInventory().begin(); //identify the object in the Dungeon you're standing on
    while(it!=mDungeon->getDungeonInventory().end()){
    if((*it)->getX()==xCoord && (*it)->getY()==yCoord)
        break;
        
        it++;
    }
    Object*p = (*it);
    Inventory.push_back(p); //addd that object to your inventory and remove it from the dungeon
    mDungeon->getDungeonInventory().erase(it);
    string result;
    if(p->value()=='W')
        result = "You pick up ";
    else
        result = "You pick up a scroll called ";
    result+=p->getName();
    return result;//return the appropriate message to the game
}

void Player::seeInventory(){
    clearScreen();
    list<Object*>::iterator it = Inventory.begin();
    char x = 'a';
    while(it!=Inventory.end()){
        string y = "";
        if((*it)->value()=='S') //if the object is a scroll, the message is slightly diffferent.
            y = "A scroll called ";
        
        cout << x << ". " << y << (*it)->getName() <<endl;
        it++;
        x++;
    }
}
string Player::wieldWeapon(){
    seeInventory();
    char x = getCharacter(); //convert the character recieved from the user into an integer
    int y = x-96;
    list<Object*>::iterator it = Inventory.begin();
    int i = 1;
    while(it!=Inventory.end()){ //if that integer matches the position in the Inventory of some item,
        if(y==i)
            break; //get the iterator pointing to that item
        i++;
        it++;
    }
    if(it==Inventory.end()) //if no such position exists, ie. invalid input, return an empty string. nothing happened
        return "";
    else if((*it)->value()=='S'){ //you can't wield a scroll!
        string answer = "You can't wield a ";
        answer+=(*it)->getName();
        return answer;
    }
    else{ //point the myWeapon pointer at the desired Weapon.
        Object*p = *it;
        myWeapon = p;
        string answer = "You are wielding ";
        answer+=myWeapon->getName();
        return answer;
    }
    
}

string Player::readScroll(){
  seeInventory();
    char x = getCharacter(); //convert the character recieved from the user into an integer
    int y = x-96;
    list<Object*>::iterator it = Inventory.begin();
    int i = 1;
    while(it!=Inventory.end()){ //if that integer matches the position in the Inventory of some item,
        if(y==i)
            break; //get the iterator pointing to that item
        i++;
        it++;
    }
    if(it==Inventory.end()) //if no such position exists, ie. invalid input, return an empty string. nothing happened
        return "";
    else if((*it)->value()=='W'){ //you can't read a Weapon!
        string answer = "You can't read a ";
        answer+=(*it)->getName();
        return answer;
    }
    else{
        string scrollName = (*it)->getName();
        if(scrollName == "scroll of teleportation"){
        int x =0;//send the player to a new location!
        int y = 0;
        for(;;){
            y = randInt(1,68); //change this to make sure you're placing on an empty spot in the board+account for monsters, etc.
            x = randInt(1,16);
            if(mDungeon->getCell(x, y)== ' ' || mDungeon->getCell(x, y)=='>' || mDungeon->getCell(x, y)=='&' || mDungeon->getCell(x, y)== '?' || mDungeon->getCell(x, y)== ')')
                break;
        }
            //experimental;
            mDungeon->setCell(yCoord,xCoord,' '); //update the dungeon
            xCoord = y;
            yCoord = x;
            mDungeon->setCell(yCoord,xCoord,'@');
        }
        else if(scrollName == "scroll of strength"){
            int x = randInt(1,3); //increase player Strength
            Strength+=x;
        }
        else if(scrollName == "scroll of enhance armor" ){
        int x = randInt(1,3); //increase player Armour
                   Armour+=x;
        }
        else if(scrollName == "scroll of enhance dexterity" ){
            Dexterity+=1;
        }
        else if(scrollName =="scroll of enhance health"){
            maxHealth+=randInt(3,8);
        }
        string answer = "You read the scroll called ";
        answer+=scrollName;
        answer+=" ";
        answer+= (*it)->effect();
        Object*ptr = (*it);
        delete ptr;
        it = Inventory.erase(it);
        return answer;
        
    }
}

int Player::getMaxHP() const{
    return maxHealth;
}
void Player::mightincreaseHP(){
    bool increasemyHp = trueWithProbability(0.1);
    if(increasemyHp && (HP<maxHealth)){
        HP+=1;
        return;
    }
    return;
}
//verify if the direction the player is about to move in coincides with the position of a monster
bool Player::isAbouttoMoveonMonster(char Direction) const{
    int column = xCoord;
    int row = yCoord;
    if(Direction == ARROW_UP)
        row--;
    else if(Direction == ARROW_DOWN)
        row++;
    else if(Direction == ARROW_LEFT)
        column--;
    else if(Direction == ARROW_RIGHT)
        column++;
    //row and colomn now represent the coordinates of the point you want to move on
    list<Monster*>::iterator it = mDungeon->getListofMonsters().begin();
    while(it!=mDungeon->getListofMonsters().end()){
        Monster*mp = (*it);
    if(mp->getXCoord()==column && mp->getYCoord()== row)
        return true; //returns true if there's a monster on the position you want to move to
        it++;
    }
    return false;
}

string Player::attack(char Direction){
    //identify the monster to be attacked
    int column = xCoord;
    int row = yCoord;
       if(Direction == ARROW_UP)
           row--;
       else if(Direction == ARROW_DOWN)
           row++;
       else if(Direction == ARROW_LEFT)
           column--;
       else if(Direction == ARROW_RIGHT)
           column++;
       //row and colomn now represent the coordinates of the point you want to move on
       list<Monster*>::iterator it = mDungeon->getListofMonsters().begin();
       while(it!=mDungeon->getListofMonsters().end()){
           Monster*mp = (*it);
       if(mp->getXCoord()==column && mp->getYCoord()== row)
           break;//*it now points to the the monster on the position you want to move to
           it++;
       }
    Monster*mp = *it;
    
    string q = myWeapon->getName();
   /* int WeaponDexterityBonus = 0; //set the WeaponDexterity and Damage based on what type of weapon you are crrently weielding
    int WeaponDamage = 2;
    if(q=="short sword"){
        WeaponDexterityBonus = 0;
        WeaponDamage = 2;
    }
    else if (q== "long sword"){
        WeaponDexterityBonus = 2;
        WeaponDamage = 4;
    }
    else if(q== "mace"){
        WeaponDexterityBonus = 0;
        WeaponDamage = 2;;
    }
    else if(q == "magic axe"){
        WeaponDexterityBonus = 5;
        WeaponDamage = 5;
    }
    else if(q == "magic fangs of sleep"){
    WeaponDexterityBonus = 3;
    WeaponDamage = 2;
    }
    */
    string response = "Player "; //build up the string response of the player
    response+=myWeapon->effect();
    response+= " the ";
    response+=mp->getName();
    int attackerPoints = Dexterity + myWeapon->getdexterity();     // WeaponDexterityBonus;
    int defenderPoints = mp->getDexterity() + mp->getArmour();
    int damagePoints = randInt(0, Strength + myWeapon->getDamage() - 1); //Weapon damage
    if (randInt(1, attackerPoints) >= randInt(1, defenderPoints)){ //the Player has struck the Monster
        mp->reduceHPby(damagePoints); //reduce the monster's HP;
        if(mp->getHP()<=0){ //if it's dead
            response+= " and hits, dealing a final blow."; //get rid of the monster then
            if(mp->whatMonster()=='B'){ //see if the monster will drop an object. If so, make the drop
                if((mDungeon->hasObject(mp->getXCoord(), mp->getYCoord())== false) &&  (trueWithProbability(0.1))){
                    Object*p = new Weapon("magic axe",5,5,mp->getXCoord(),mp->getYCoord());
                    mDungeon->getDungeonInventory().push_back(p);
                }
            }
            else if(mp->whatMonster()== 'G'){
                if((mDungeon->hasObject(mp->getXCoord(), mp->getYCoord())== false) &&  (trueWithProbability(1.0/3.0))){
                int j = randInt(1,2);
                Object*p;
                if(j==1)
                    p = new Weapon("magic axe",5,5,mp->getXCoord(),mp->getYCoord());
                else
                    p = new Weapon("magic fangs of sleep",2,3,mp->getXCoord(),mp->getYCoord());
                mDungeon->getDungeonInventory().push_back(p);
            }
            }
            else if(mp->whatMonster() == 'S'){
                if((mDungeon->hasObject(mp->getXCoord(), mp->getYCoord())== false) &&  (trueWithProbability(1.0/3.0))){
                Object*p = new Weapon("magic fangs of sleep",2,3,mp->getXCoord(),mp->getYCoord());
                mDungeon->getDungeonInventory().push_back(p);
            }
            }
            else if(mp->whatMonster() == 'D'){
                if((mDungeon->hasObject(mp->getXCoord(), mp->getYCoord())== false) &&  (trueWithProbability(1.0))){
                Object*p;
                int i = randInt(1,5);
                if(i==1)
                    p = new Scroll("scroll of enhance armor",mp->getXCoord(),mp->getYCoord(),-1,-1);
                else if(i==2)
                    p = new Scroll("scroll of strength",mp->getXCoord(),mp->getYCoord(),-1,-1);
                else if(i==3)
                    p = new Scroll("scroll of enhance health",mp->getXCoord(),mp->getYCoord(),-1,-1);
                else if(i==4)
                    p = new Scroll("scroll of enhance dexterity",mp->getXCoord(),mp->getYCoord(),-1,-1);
                else if(i==5)
                    p = new Scroll("scroll of teleportation",mp->getXCoord(),mp->getYCoord(),-1,-1);
                    
                
                mDungeon->getDungeonInventory().push_back(p);
            }
            }
            //experimental update the dungeon to make a spot without a monster on it
            mDungeon->setCell(mp->getYCoord(), mp->getXCoord(), ' ');
            delete mp; //delete the monster object in the heap
            it = mDungeon->getListofMonsters().erase(it); //remove it from the ListOfMonsters
            //accounted for the monster dropping an object
            
        }
        else{
            if(myWeapon->getName()=="magic fangs of sleep" && trueWithProbability(1.0/5.0)){
            //account for the monster being put to sleep
                int X = randInt(2,6);
                int Y = mp->getSleeptime();
                mp->setSleeptime(max(X,Y));
                response+=" and hits, putting ";
                response+=mp->getName();
                response+=" to sleep.";
            }
            else{
            response+=" and hits.";
            }
        }
    }//the Player has missed
    else{
        response += " and misses.";
    }
    
    return response;
}

int Player::max(int a, int b) const{
    if(a>b)
        return a;
    else
        return b;
}

int Player::getSleeptime() const{
    return Sleeptime;
}
void Player::setSleeptime(int x){
    Sleeptime = x;
}

bool Player::isPlayerDead() const{
if(HP<=0)
    return true;
    else
        return false;
}
void Player::reduceHpby(int x){
    HP-=x;
}

string Player::Cheat(){
    Strength = 9;
    HP = 50;
    return "";
}
