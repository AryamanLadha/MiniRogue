//
//  Objects.cpp
//  Project3
//
//  Created by Siddharth Ladha on 13/05/20.
//  Copyright © 2020 CS 32. All rights reserved.
//

#include "Objects.h"
ladderOrIdol::ladderOrIdol()
:xCoord(0), yCoord(0)
{}
int ladderOrIdol::getxCoord() const{
    return xCoord;
}
int ladderOrIdol::getyCoord() const{
    return yCoord;
}
void ladderOrIdol::setPosition(int a, int b){
    xCoord = a;
    yCoord = b;
}

Object::Object(string m, int a, int b, int x, int y){
    xCoord = a;
    yCoord = b;
    name = m;
    damage = x;
    dexterity = y;
}
Object::~Object()
{
  // cerr<<"Object deleted" << endl;
}

int Object::getX() const{
    return xCoord;
}
int Object::getY() const{
    return yCoord;
}
void Object::setX(int a){
    xCoord = a;
}
void Object::setY(int b){
    yCoord = b;
}
char Object::value() const{
    return 'O';
}
string Object::getName() const{
    return name;
}
int Object::getDamage() const{
    return damage;
}

int Object::getdexterity() const{
    return dexterity;
}


Weapon::Weapon(string s, int a, int b, int x, int y): Object(s,x,y,a,b)
{
    
}

string Weapon::effect() const{
if(getName() == "mace")
    return "swings mace at";
    else if(getName() == "short sword")
        return "slashes short sword at";
    else if(getName() == "long sword")
        return "swings long sword at";
    else if(getName()== "magic fangs of sleep")
        return "strikes magic fangs at";
    else
        return "chops magic axe at";
}
//string Weapon::getName() const{
  //  return name;
//}
char Weapon::value() const{
    return 'W';
}

Scroll::Scroll(string a, int x, int y, int damage, int dexterity): Object(a,x,y,damage,dexterity)
{}

//string Scroll::getName() const{
   // return name;
//}
string Scroll::effect() const{
    string x = "";
    if(getName() == "scroll of teleportation"){
        x = "You feel your body wrenched in space and time.";
    }
    else if(getName() == "scroll of strength"){
        x = "Your muscles bulge.";
    }
    else if(getName() == "scroll of enhance armor"){
        x = "Your armor glows blue.";
    }
    else if(getName() == "scroll of enhance health"){
        x = "You feel your heart beating stronger.";
    }
    else if(getName() == "scroll of enhance dexterity"){
        x = "You feel like less of a klutz.";
    }
    return x;
}

char Scroll::value() const{
    return 'S';
}
