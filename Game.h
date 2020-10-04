// Game.h

#ifndef GAME_INCLUDED
#define GAME_INCLUDED

// You may add data members and other member functions to this class.
#include "Actor.h"
#include "Dungeon.h"
class Game
{
public:
    Game(int goblinSmellDistance);
    ~Game();
    void play();
    void takePlayerTurn(const char x, string& whatHappened);
    void playerDescend();
    void TakeMonstersTurn();
private:
    Player* human;
    Dungeon* pitch;
    list<string>MonsterMessages;
    int GoblinSmell;
};

#endif // GAME_INCLUDED

