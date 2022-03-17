#include "provided.h"
#include "support.h"
#include <stack>
#include <string>
#include <iostream>
using namespace std;

class HumanPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
};

class BadPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
};

class SmartPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
};

int HumanPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    int moveChoice;
    cout << "Choose your move: " << endl;
    cin >> moveChoice;
    cin.ignore();
    
    // If the column of choice is invalid/full, reprompt
    while ( moveChoice <= 0 || moveChoice > s.cols() || s.checkerAt(moveChoice, s.levels()) != VACANT)
    {
        cout << "Invalid column! Choose another move: " << endl;
        cin >> moveChoice;
        cin.ignore();
    }
    
    return moveChoice;
}

int BadPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    int moveChoice = 0;
    
    while (moveChoice == 0)
    {
        moveChoice = randNum(1, s.cols());
        if ( s.checkerAt(moveChoice, s.levels())!= VACANT )
            moveChoice = 0;
    }
    
    return moveChoice;
}

int SmartPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    AlarmClock ac(8900);
    
    int initialDepth = 0;
    
    scoreAndColumn move = minimax(s, N, color, initialDepth, ac);
    return move.column;
}

//******************** Player derived class functions *************************

//  These functions simply delegate to the Impl classes' functions.
//  You probably don't want to change any of this code.

HumanPlayer::HumanPlayer(string nm)
 : Player(nm)
{
    m_impl = new HumanPlayerImpl;
}
 
HumanPlayer::~HumanPlayer()
{
    delete m_impl;
}
 
int HumanPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

BadPlayer::BadPlayer(string nm)
 : Player(nm)
{
    m_impl = new BadPlayerImpl;
}
 
BadPlayer::~BadPlayer()
{
    delete m_impl;
}
 
int BadPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

SmartPlayer::SmartPlayer(string nm)
 : Player(nm)
{
    m_impl = new SmartPlayerImpl;
}
 
SmartPlayer::~SmartPlayer()
{
    delete m_impl;
}
 
int SmartPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}
