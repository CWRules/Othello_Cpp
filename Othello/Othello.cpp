/*
Othello-playing AI

Author: Connor Wilson
Date: 2020-08-20
*/

#include "TreeNode.h"
#include <iostream>

int main()
{
	TreeNode startingNode("..\\starting_board.txt");
	std::cout << startingNode.PrintBoardState();

	TreeNode::MakeTree(&startingNode, 5);//// TESTING
}
