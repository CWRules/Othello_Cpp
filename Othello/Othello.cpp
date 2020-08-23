/*
Othello-playing AI

Author: Connor Wilson
Date: 2020-08-20
*/

#include "TreeNode.h"
#include <iostream>

int main()
{
	Board startingBoard("..\\starting_board.txt");
	TreeNode startingNode(startingBoard);
	std::cout << startingNode.m_Board.PrintBoardState();
}
