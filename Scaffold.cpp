#include "provided.h"
#include "support.h"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <stack>
#include <utility>
using namespace std;

class ScaffoldImpl
{
    public:
        ScaffoldImpl(int nColumns, int nLevels);
        int cols() const;
        int levels() const;
        int numberEmpty() const;
        int checkerAt(int column, int level) const;
        void display() const;
        bool makeMove(int column, int color);
        int undoMove();

    private:
        int mColumns;
        int mLevels;
        int mCheckerCount;
    
        vector<vector<int>> grid;
    
        stack<coordinate> history;
};

ScaffoldImpl::ScaffoldImpl(int nColumns, int nLevels)
{
    // Accept proper dimensions
    if (nColumns > 0)
        mColumns = nColumns;
    if (nColumns <= 0)
    {
        cerr << "Invalid Column Size";
        exit(1);
    }
    
    if (nLevels > 0)
        mLevels = nLevels;
    if (nLevels <= 0)
    {
        cerr << "Invalid Level Size";
        exit(1);
    }
    
    mCheckerCount = 0;
    
    grid.resize(nColumns);  // grid now has nLevels empty rows
    for (int i = 0; i < nColumns; i++)
            grid[i].resize(nLevels);  // row i now has nColumns columns
    
    // Fill grid with VACANT values
    for (int i = 0; i < nColumns; i++)
    {
        for (int j = 0; j < nLevels; j++)
        {
            grid[i][j] = -1;
        }
    }
}

int ScaffoldImpl::cols() const
{
    return mColumns;
}

int ScaffoldImpl::levels() const
{
    return mLevels;
}

int ScaffoldImpl::numberEmpty() const
{
    return ( mColumns * mLevels ) - mCheckerCount;
}

int ScaffoldImpl::checkerAt(int column, int level) const
{
    // Since it's 0 based indexing, we subtract 1 from each input
    
    return grid[column-1][level-1];
}

void ScaffoldImpl::display() const
{
    for (int i = mLevels - 1; i >= 0; i--)
    {
        for (int j = 0; j < mColumns; j++)
        {
            cout << "|";
            if (grid[j][i] == RED)
                cout << "R";
            else if (grid[j][i] == BLACK)
                cout << "B";
            else if (grid[j][i] == VACANT)
                cout << " ";
            
            if (j == mColumns - 1)
                cout << "|" << endl;
        }
        
        if (i == 0)
        {
            for (int j = 0; j < mColumns; j++)
            {
                cout << "+-";
                if (j == mColumns - 1)
                    cout << "+" << endl;
            }
        }
    }
}

bool ScaffoldImpl::makeMove(int column, int color)
{
    // If there's a vacancy, make the move at the first available spot in the column
    if (checkerAt(column, mLevels) == VACANT)
    {
        for (int level = 1; level<=mLevels; level++)
        {
            if (checkerAt(column, level) == VACANT)
            {
                grid[column-1][level-1] = color;
                history.push( coordinate(column, level) );
                mCheckerCount++;
                break;
            }
        }
        return true;
    }
    return false;
}

int ScaffoldImpl::undoMove()
{
    // Find the most recent move from the history, remove it from the grid and pop it from the history
    if (history.size() > 0)
    {
        grid[history.top().column()-1][history.top().level()-1] = -1;
        
        coordinate temp = history.top();
        history.pop();
        mCheckerCount--;
        
        return temp.column();
    }
    return 0;
}

//******************** Scaffold functions *******************************

//  These functions simply delegate to ScaffoldImpl's functions.
//  You probably don't want to change any of this code.

Scaffold::Scaffold(int nColumns, int nLevels)
{
    m_impl = new ScaffoldImpl(nColumns, nLevels);
}
 
Scaffold::~Scaffold()
{
    delete m_impl;
}
 
Scaffold::Scaffold(const Scaffold& other)
{
    m_impl = new ScaffoldImpl(*other.m_impl);
}
 
Scaffold& Scaffold::operator=(const Scaffold& rhs)
{
    if (this != &rhs)
    {
        Scaffold temp(rhs);
        swap(m_impl, temp.m_impl);
    }
    return *this;
}
 
int Scaffold::cols() const
{
    return m_impl->cols();
}

int Scaffold::levels() const
{
    return m_impl->levels();
}

int Scaffold::numberEmpty() const
{
    return m_impl->numberEmpty();
}

int Scaffold::checkerAt(int column, int level) const
{
    return m_impl->checkerAt(column, level);
}
 
void Scaffold::display() const
{
    m_impl->display();
}
 
bool Scaffold::makeMove(int column, int color)
{
    return m_impl->makeMove(column, color);
}
 
int Scaffold::undoMove()
{
    return m_impl->undoMove();
}
