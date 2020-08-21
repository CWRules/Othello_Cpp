// Othello.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Board.h"
#include <iostream>
#include <string>

int main()
{
	Board gameBoard("..\\starting_board.txt");
	std::cout << gameBoard.PrintBoardState();
}
