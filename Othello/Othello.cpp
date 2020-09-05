/*
Othello-playing AI

Author: Connor Wilson
Date: 2020-08-20
*/

#include "TreeNode.h"
#include <iostream>

int main()
{
	TreeNode* currentRootNode = new TreeNode("..\\starting_board_tiny.txt");
	int currentTurn = BLACK;
	int playerColor = BLACK;

	while (true)
	{
		std::cout << currentRootNode->PrintBoardState();

		// Update game tree
		std::cout << "Building game tree...\n";
		TIMEPOINT startTime = CURRENT_TIME;
		int depth = TreeNode::MakeTree(currentRootNode, 10);
		TIMEPOINT currentTime = CURRENT_TIME;
		int makeTreeTime = DURATION(startTime, currentTime);
		std::cout << "Stopped at depth " << depth << " after " << makeTreeTime << " seconds.\n";

		TreeNode* newRootNode = NULL;
		if (currentTurn == playerColor)
		{
			// Player turn

			//// TODO: Get input and call SelectMove()
		}
		else
		{
			// AI turn
			TreeNode::EvaluateNode(currentRootNode);
			newRootNode = currentRootNode->m_BestChildNode;
		}

		TreeNode::ChangeRoot(currentRootNode, newRootNode);
		if (currentRootNode->m_GameOver)
		{
			break;
		}

		currentTurn = (currentTurn == BLACK ? WHITE : BLACK);
	}
}
