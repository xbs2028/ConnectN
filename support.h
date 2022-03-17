#ifndef SUPPORT_INCLUDED
#define SUPPORT_INCLUDED

#include "provided.h"
#include <iostream>
#include <cstdlib>
#include <stack>
#include <vector>
using namespace std;

int randNum(int min, int max);

int scaffoldRate(const Scaffold& s, int N, int computer_color);

// Functions to determine consecutive checkers
int verticalAdjacentCount(const Scaffold& s, int column, int row);
int horizontalAdjacentCount(const Scaffold& s, int column, int row);
int diagonalUpAdjacentCount(const Scaffold& s, int column, int row);
int diagonalDownAdjacentCount(const Scaffold& s, int column, int row);
int adjacentCount(const Scaffold& s, int column, int row);

struct scoreAndColumn
{
    int score;
    int column;
};

// Smartplayer recursive function
scoreAndColumn minimax(const Scaffold& s, int N, int color, int depth, AlarmClock& ac);

class coordinate
{
    public:
        coordinate(int col, int lev);
        int column();
        int level();
    private:
        int col_coord;
        int lev_coord;
};


#endif // SUPPORT_INCLUDED
