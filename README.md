# CONNECT-N
Connect-N is a version of connect-4 where you can specify
the grid dimensions and number in a row (N) to win. A win
is N pieces of the same color either horizontal, vertical,
or diagonal. This game also features a minimax AI algorithm
which picks the best possible moves. It acheives either
the quickest win or the longest loss.

Instructions to change game parameters:
1. Navigate into main.cpp and edit these lines:
```
    SmartPlayer bp("Computer");
    HumanPlayer hp("Human");

    Game g(4, 3, 3, &bp, &hp);
```
Here, you can use a SmartPlayer (which is the minimax algorithm),
a BadPlayer (which just picks random moves), or a HumanPlayer,
which receives moves via standard input. If a human is playing,
simply type in the desired column you'd like to place the piece.

The order of these two lines determines which player goes first.
Moreover, you can change the nickname of the player by
modifying the text in quotes, i.e. ("Computer") -> ("Sam")