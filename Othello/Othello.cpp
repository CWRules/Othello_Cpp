/*
Othello-playing AI

Author: Connor Wilson
Date: 2020-08-20
*/

#include "TreeNode.h"
#include <chrono>
#include <iostream>

int main()
{
	TreeNode* currentRootNode = new TreeNode("..\\starting_board_small.txt");
	int currentTurn = BLACK;
	int playerColor = BLACK;

	while (true)
	{
		std::cout << currentRootNode->PrintBoardState();

		// Update game tree
		std::cout << "Building game tree...\n";
		std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
		int depth = TreeNode::MakeTree(currentRootNode, 10);
		std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
		int makeTreeTime = (int)std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
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
