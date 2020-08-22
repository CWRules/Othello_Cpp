/*
Othello-playing AI

Author: Connor Wilson
Date: 2020-08-20
*/

#include "Board.h"
#include <iostream>

int main()
{
	Board gameBoard("..\\starting_board.txt");
	std::cout << gameBoard.PrintBoardState();
}
