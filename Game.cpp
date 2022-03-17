#include "provided.h"
#include "support.h"
#include <stack>
#include <iostream>
using namespace std;

class GameImpl
{
    public:
        GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black);
        bool completed(int& winner) const;
        bool takeTurn();
        void play();
        int checkerAt(int c, int r) const;
    private:
        Scaffold mScaffold;
        int mN;
        Player* mRed;
        Player* mBlack;
        stack<Player*> move_history;
};

GameImpl::GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black) : mScaffold(Scaffold(nColumns, nLevels)), mN(N), mRed(red), mBlack(black)
{
}

bool GameImpl::completed(int& winner) const
{
    // Use the scaffold rating function to determine the winner
    if ( scaffoldRate(mScaffold, mN, RED) == 1 )
    {
        winner = RED;
        return true;
    }
    if ( scaffoldRate(mScaffold, mN, RED) == -1 )
    {
        winner = BLACK;
        return true;
    }
    if ( scaffoldRate(mScaffold, mN, RED) == 0 )
    {
        winner = TIE_GAME;
        return true;
    }
    else
        return false;
}

bool GameImpl::takeTurn()
{
    // Only valid when scaffold rating is incomplete (-999)
    if ( scaffoldRate(mScaffold, mN, RED) == -999 )
    {
        if (move_history.empty() || move_history.top() == mBlack)
        {
            mScaffold.makeMove(mRed->chooseMove(mScaffold, mN, RED), RED);
            move_history.push(mRed);
            return true;
        }
        else if (move_history.top() == mRed)
        {
            mScaffold.makeMove(mBlack->chooseMove(mScaffold, mN, BLACK), BLACK);
            move_history.push(mBlack);
            return true;
        }
    }
    return false;
}

void GameImpl::play()
{
    int winner = 0;
    
    while ( !completed(winner) )
    {
        if (move_history.empty())
            cout << "##### " << mRed->name() << "'s Turn: RED #####" << endl;
        else if (move_history.top() == mRed)
            cout << "##### " << mBlack->name() << "'s Turn: BLACK #####" << endl;
        else if (move_history.top() == mBlack)
            cout << "##### " << mRed->name() << "'s Turn: RED #####" << endl;
        
        mScaffold.display();
        
        // If neither player is interactive, prompt for ENTER
        if (!mRed->isInteractive() && !mBlack->isInteractive())
        {
            cout << "Press ENTER to Continue" << endl;
            cin.ignore();
        }
        takeTurn();
    }
    
    cout << "##### Game Complete! #####" << endl;
    mScaffold.display();
    if (winner == 0)
        cout << mRed->name() << " (Red) got " << mN << " in a row!" << endl;
    
    if (winner == 1)
        cout << mBlack->name() << " (Black) got " << mN << " in a row!" << endl;
    
    if (winner == -1)
        cout << "Tie Game!" << endl;
    
}

int GameImpl::checkerAt(int c, int r) const
{
    return mScaffold.checkerAt(c, r);
}

//******************** Game functions *******************************

//  These functions simply delegate to GameImpl's functions.
//  You probably don't want to change any of this code.

Game::Game(int nColumns, int nLevels, int N, Player* red, Player* black)
{
    m_impl = new GameImpl(nColumns, nLevels, N, red, black);
}
 
Game::~Game()
{
    delete m_impl;
}
 
bool Game::completed(int& winner) const
{
    return m_impl->completed(winner);
}

bool Game::takeTurn()
{
    return m_impl->takeTurn();
}

void Game::play()
{
    m_impl->play();
}
 
int Game::checkerAt(int c, int r) const
{
    return m_impl->checkerAt(c, r);
}
