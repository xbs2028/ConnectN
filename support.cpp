#include "support.h"

int randNum(int min, int max)
{
    // Change the seed with srand() for different outcomes. When testing, I set it to the computer's clock <ctime> but I'm not sure if this is allowed so I'll leave it out.
    return ( rand() % max + min );
}

int scaffoldRate(const Scaffold& s, int N, int color)
{
    // Scan the grid to see who wins (if there is a winner)
    // If input color wins, return 1. If input color loses, return -1.
    int winnerValue = -999; // If the value stays at -999, the game isn't over
    
    for (int row = 1; row <= s.levels(); row++)
    {
        for (int column = 1; column <= s.cols(); column++)
        {
            if (adjacentCount(s, column, row) >= N)
            {
                if (s.checkerAt(column, row) == color)
                {
                    winnerValue = 1;
                    break;
                }
                
                else if (s.checkerAt(column, row) != color)
                {
                    winnerValue = -1;
                    break;
                }
            }
        }
        
    }

    // If tie, winnerValue is 0
    if (s.numberEmpty() == 0 && winnerValue == -999)
        winnerValue = 0;
    
    return winnerValue;
}

int verticalAdjacentCount(const Scaffold& s, int column, int row)
{
    // Count the amount of like colors above the checker
    int color = s.checkerAt(column, row);
    int likeCount = 1;
    
    for (int i = 1; i <= s.levels()-row; i++)
    {
        if ( s.checkerAt(column, row+i) == color )
            likeCount++;
        else
            break;
    }
    
    if (color != VACANT)
        return likeCount;
    
    return 0;
}

int horizontalAdjacentCount(const Scaffold& s, int column, int row)
{
    // Count the amount of like colors to the right of the checker
    int color = s.checkerAt(column, row);
    int likeCount = 1;
    
    for (int i = 1; i <= s.cols()-column; i++)
    {
        if ( s.checkerAt(column+i, row) == color )
            likeCount++;
        else
            break;
    }
    
    if (color != VACANT)
        return likeCount;
    
    return 0;
}

int diagonalUpAdjacentCount(const Scaffold& s, int column, int row)
{
    // Count the amount of like colors to the upper diagonal of the checker
    int color = s.checkerAt(column, row);
    int likeCount = 1;
    
    for (int i = 1; i <= s.cols()-column; i++)
    {
        if ( row + i <= s.levels() && s.checkerAt(column+i, row+i) == color )
            likeCount++;
        else
            break;
    }
    
    if (color != VACANT)
        return likeCount;
    
    return 0;
}

int diagonalDownAdjacentCount(const Scaffold& s, int column, int row)
{
    // Count the amount of like colors to the lower diagonal of the checker
    int color = s.checkerAt(column, row);
    int likeCount = 1;
    
    for (int i = 1; i <= s.cols()-column; i++)
    {
        if ( row - i > 0 && s.checkerAt(column+i, row-i) == color )
            likeCount++;
        else
            break;
    }
    
    if (color != VACANT)
        return likeCount;
    
    return 0;
}

int adjacentCount(const Scaffold& s, int column, int row)
{
    // Try all adjacent functions, return one with max value
    int likeCount = 1;
    
    if (verticalAdjacentCount(s, column, row) > likeCount)
        likeCount = verticalAdjacentCount(s, column, row);
    if (horizontalAdjacentCount(s, column, row) > likeCount)
        likeCount = horizontalAdjacentCount(s, column, row);
    if (diagonalUpAdjacentCount(s, column, row) > likeCount)
        likeCount = diagonalUpAdjacentCount(s, column, row);
    if (diagonalDownAdjacentCount(s, column, row) > likeCount)
        likeCount = diagonalDownAdjacentCount(s, column, row);
    
    return likeCount;
}


scoreAndColumn minimax(const Scaffold& s, int N, int color, int initialDepth, AlarmClock& ac)
{
    Scaffold tempScaffold = Scaffold(s);
    
    // We store each evalutation of the rating/recursive function in this vector
    vector<scoreAndColumn> childBranch;
    
    // Code to find whose turn it is, and what the opponent's color is
    bool myTurn = true;
    if (initialDepth % 2 == 0)
        myTurn = true;
    else if (initialDepth % 2 != 0)
        myTurn = false;
    
    int oppositeColor = VACANT;
    if (color == RED)
        oppositeColor = BLACK;
    if (color == BLACK)
        oppositeColor = RED;
    
    // Maximize computer's moves
    if (myTurn)
    {
        for (int i = 0; i < s.cols(); i++)
        {
            if ( (s.checkerAt(i+1, s.levels())) == VACANT )
            {
                scoreAndColumn result;
                tempScaffold.makeMove(i+1, color);
                int new_score = scaffoldRate(tempScaffold, N, color);
                
                // If a win (or a timeout)
                if (new_score >= 0 || ac.timedOut())
                {
                    result.score = 10000 - initialDepth;
                }
                
                // Otherwise, recurse
                else
                {
                    result = minimax(tempScaffold, N, oppositeColor, initialDepth+1, ac);
                }
                
                // Store the values in the childBranch vector
                result.column = i + 1;
                childBranch.push_back(result);
                tempScaffold.undoMove();
            }
        }
    }
    
    
    // Minimize human's moves
    if (!myTurn)
    {
        for (int i = 0; i < s.cols(); i++)
        {
            if ( (s.checkerAt(i+1, s.levels())) == VACANT )
            {
                scoreAndColumn result;
                tempScaffold.makeMove(i+1, color);
                int new_score = scaffoldRate(tempScaffold, N, color);
                
                // If a win (or a timeout)
                if (new_score >= 0 || ac.timedOut())
                {
                    result.score = -(10000 - initialDepth);
                }
                
                // Otherwise, recurse
                else
                {
                    result = minimax(tempScaffold, N, oppositeColor, initialDepth+1, ac);
                }
                
                // Store the values in the childBranch vector
                result.column = i + 1;
                childBranch.push_back(result);
                tempScaffold.undoMove();
            }
        }
    }
    
    scoreAndColumn bestResult;
    bestResult = childBranch[0];
    
    for (int i = 1; i < childBranch.size(); i++)
    {
        // Return best move for this player
        if (myTurn)
        {
            if (childBranch[i].score > bestResult.score)
            {
                bestResult = childBranch[i];
            }
        }
        
        // Return the best (in negative) move for the other player
        if (!myTurn)
        {
            if (childBranch[i].score < bestResult.score)
            {
                bestResult = childBranch[i];
            }
        }
    }

    return bestResult;
}

coordinate::coordinate(int col, int lev)
{
    col_coord = col;
    lev_coord = lev;
}

int coordinate::column()
{
    return col_coord;
}

int coordinate::level()
{
    return lev_coord;
}
