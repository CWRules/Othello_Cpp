/*
Othello-playing AI

Author: Connor Wilson
Date: 2020-08-20
*/

#include "TreeNode.h"
#include <iostream>

int main()
{
	TreeNode* startingNode = new TreeNode("..\\starting_board.txt");

	//// TESTING
	TreeNode::MakeTree(startingNode, 10);
	TreeNode::EvaluateNode(startingNode);
	//TreeNode* newRoot = TreeNode::SelectMove(startingNode, std::make_pair(2, 3));

	std::cout << startingNode->PrintBoardState();
}
