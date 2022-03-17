#include "provided.h"
#include "support.h"
#include <string>
#include <iostream>
using namespace std;


int main()
{
    //===== SPECIFY GAME TYPE =====\\
    
    // Player types: SmartPlayer, BadPlayer, HumanPlayer
    
    // PLAYER 1
    SmartPlayer bp("Computer");
    // PLAYER 2
    HumanPlayer hp("Human");
    
    
    // Specify x rows, y columns, n in-a-row to win
    // Recommended: 4, 3, 3
    Game g(4, 3, 3, &bp, &hp);
    
    // Run the game
    g.play();
}
