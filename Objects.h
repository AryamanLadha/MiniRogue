//
//  Objects.hpp
//  Project3
//
//  Created by Siddharth Ladha on 13/05/20.
//  Copyright © 2020 CS 32. All rights reserved.
//

#ifndef Objects_h
#define Objects_h
#include <string>
using namespace std;
#include <iostream>
class ladderOrIdol{
public:
    ladderOrIdol();
    int getxCoord() const;
    int getyCoord() const;
    void setPosition(int a, int b);
private:
    int xCoord;
    int yCoord;
};

class Object{
public:
    Object( string m, int a, int b, int x, int y);
    virtual~Object();
    virtual string effect() const = 0;
    string getName() const;
    int getX() const;
    int getY() const;
    void setX(int a);
    void setY(int b);
    virtual char value() const; //used to see if it's a Weapon or a Scroll
    int getDamage () const;
    int getdexterity() const;
private:
    int xCoord;
    int yCoord;
    string name;
    int damage;
    int dexterity;
};

class Weapon: public Object{
public:
    Weapon(string s, int a, int b, int x, int y);
   // int getDamage() const;
    string effect() const;
    //int getdexterity() const;
    // virtual string getName() const;
    virtual char value() const;
private:
  //  int damage;
  //  int dexterityBonus;
};
//all scrolls will have damage and dexterity -1;
class Scroll: public Object{
public:
    Scroll(string a, int x, int y, int damage, int dexterity);
    // virtual string getName() const;
    string effect() const;
    virtual char value() const;
private:
};


#endif /* Objects_h */
