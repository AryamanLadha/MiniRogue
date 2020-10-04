// Game.cpp

#include "Game.h"
#include "utilities.h"
#include <iostream>
#include "Monster.h"
using namespace std;

// Implement these and other Game member functions you may have added.

Game::Game(int goblinSmellDistance)
:pitch(new Dungeon(0,goblinSmellDistance)) //what's going on here? //can I make the dungeon pointer a member variable?
{
    human = new Player(pitch);
    GoblinSmell = goblinSmellDistance;
}
Game::~Game(){
    //delete pitch;
    delete human;
    //cerr << "game deleted" << endl;
}
void Game::takePlayerTurn(const char x, string& whatHappened){
    //Player*temp = human;
    whatHappened = "";
    if(human->getSleeptime()!=0){ //won't respond to commands if asleep
        human->setSleeptime(human->getSleeptime()-1);
    }
    else if(x == '>' && human->isonStaircaseorIdol() && pitch->getLevel()!=4) //if you want to descend a ladder, and you're on a ladder,
        playerDescend();
    
    else if(x==ARROW_UP || x == ARROW_DOWN || x == ARROW_LEFT || x== ARROW_RIGHT)//move to a position
        whatHappened = human->move(x);
    
    else if(x=='g' && human->isonObject())
        whatHappened = human->PickUp();
    else if(x=='i'){
        human->seeInventory();
        getCharacter();
    }
    else if(x=='w'){
       whatHappened =  human->wieldWeapon();
    }
    else if(x=='r'){
        whatHappened = human->readScroll();
    }
    else if(x=='c'){
        whatHappened = human->Cheat();
    }
}

void Game::play()
{
    
    //while(pitch.getCell(human.getYCoord(), human.getXCoord())!= '&'){
   // }
    Player*temp = human;
    pitch->display(temp);
   // cout << "Press q to exit game." << endl;
    char x = getCharacter();
    string output = "";
    while (x != 'q'){
        while((output!= "Congratulations, you won!" && human->isPlayerDead()==false)){ //&&playerisAlive //make this a nested while loop
            human->mightincreaseHP(); //check position later. experimental                         x!=q
            takePlayerTurn(x,output);//                            player hasn't won and player hasn't lost
            TakeMonstersTurn();
            pitch->display(human); //display the dungeon
            cout << output << endl; //then display the player's output
            list<string>::iterator it = MonsterMessages.begin();
            while(it!=MonsterMessages.end()){
                if(*it!="")
                cout << *it << endl; //display all the monster messages
                it++;
            }
            //reset the message list
            MonsterMessages.clear();
            if(human->isPlayerDead()==false && output!= "Congratulations, you won!")//experimental
            x = getCharacter();  //maybe put it here?
            else
            cout << "Press q to exit game." << endl; //experimental
            if(x=='q' && human->getSleeptime()==0) //quit game-in progress, but only when awake
                break;
        }
        if(x!='q'){//if the player doesn't want to quit yet even though the game is over, ask for an input
        x = getCharacter();
        }
    }
    
}
void Game::playerDescend(){
int y = pitch->getLevel()+1;
Dungeon*ptr = new Dungeon(y,GoblinSmell);
delete pitch;
pitch = ptr;
human->setDungeon(pitch);
       for(;;)
       {
           int y = randInt(1,68);
           int x = randInt(1,16);
           if(pitch->getCell(x, y) == ' ' || pitch->getCell(x, y) == '>' || pitch->getCell(x, y) == '&'|| pitch->getCell(x, y) == ')' || pitch->getCell(x, y) == '?'){
               human->setPosition(y, x);
               break;
           }
               
       }
}
// You will presumably add to this project other .h/.cpp files for the
// various classes of objects you need to play the game:  player, monsters,
// weapons, etc.  You might have a separate .h/.cpp pair for each class
// (e.g., Player.h, Boegeyman.h, etc.), or you might put the class
// declarations for all actors in Actor.h, all game objects in GameObject.h,
// etc.

void Game::TakeMonstersTurn(){
    
//    for (Monster* aptr : pitch->getListofMonsters())
//    {
//        string q = (aptr)->Move(pitch,human);
//        MonsterMessages.push_back(q);
//    }
    list<Monster*>::iterator it = pitch->getListofMonsters().begin();
    while(it!= pitch->getListofMonsters().end()){
        string q = (*it)->Move(pitch,human);
        MonsterMessages.push_back(q);
        it++;
    }
    
}
