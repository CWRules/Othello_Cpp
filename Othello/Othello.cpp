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
		std::cout << "\nBuilding game tree...\n";
		TIMEPOINT startTime = CURRENT_TIME;
		int depth = TreeNode::MakeTree(currentRootNode, 10);
		TIMEPOINT currentTime = CURRENT_TIME;
		int elapsedTime = DURATION(startTime, currentTime);
		std::cout << "Stopped at depth " << depth << " after " << elapsedTime << " seconds.\n\n" << (currentTurn == BLACK ? "Black" : "White") << " turn\n";

		TreeNode* newRootNode = NULL;
		if (currentTurn == playerColor)
		{
			// Player turn
			std::pair<int, int> move;
			while (true)
			{
				std::cout << "\nEnter move (row,column or pass):\n";

				std::string input;
				std::cin >> input;

				// Check input is valid
				if (input == "pass")
				{
					move = PASS_TURN;
				}
				else
				{
					std::string rowStr = input.substr(0, input.find(','));
					std::string colStr = input.substr(input.find(',') + 1);
					
					char* p1;
					char* p2;
					move = std::make_pair(strtol(rowStr.c_str(), &p1, 10), strtol(colStr.c_str(), &p2, 10));

					if (*p1 != 0 || *p2 != 0)
					{
						std::cout << "Invalid input\n";
						continue;
					}
				}

				newRootNode = TreeNode::SelectMove(currentRootNode, move);
				if (newRootNode == NULL)
				{
					std::cout << "Not a legal move\n";
				}
				else
				{
					std::cout << "\n";
					break;
				}
			}
		}
		else
		{
			// AI turn
			std::cout << "\nEvaluating...\n";
			TIMEPOINT startTime = CURRENT_TIME;
			TreeNode::EvaluateNode(currentRootNode);
			newRootNode = currentRootNode->m_BestChildNode;
			TIMEPOINT currentTime = CURRENT_TIME;
			int elapsedTime = DURATION(startTime, currentTime);
			std::cout << "Finished after " << elapsedTime << " seconds\n\n";
		}

		TreeNode::ChangeRoot(currentRootNode, newRootNode);
		currentRootNode = newRootNode;
		if (currentRootNode->m_GameOver)
		{
			break;
		}

		currentTurn = (currentTurn == BLACK ? WHITE : BLACK);
	}

	std::cout << "Game over\n";
	int winner = currentRootNode->WinningPlayer();
	if (winner == BLACK)
	{
		std::cout << "Black wins!\n";
	}
	else if (winner == WHITE)
	{
		std::cout << "White wins!\n";
	}
	else
	{
		std::cout << "It's a tie!\n";
	}
}
