/*
Othello-playing AI

Author: Connor Wilson
Date: 2020-08-20
*/

#include "TreeNode.h"
#include <fstream>
#include <iostream>

std::string startingBoardFile;
int firstPlayer;
int maxSearchTime;
int maxSearchDepth;


// Read in config file options
void ReadConfig(std::string fileName)
{
	std::ifstream inputFile(fileName);

	if (inputFile.is_open())
	{
		std::string line;

		while (std::getline(inputFile, line))
		{
			if (line == "" || line[0] == '#')
			{
				continue;
			}
			
			std::string option = line.substr(0, line.find(" "));
			if (option == "StartingBoard")
			{
				startingBoardFile = line.substr(line.find(" ") + 1);
			}
			else if (option == "FirstPlayer")
			{
				std::string color = line.substr(line.find(" ") + 1);
				if (color == "BLACK")
				{
					firstPlayer = BLACK;
				}
				else if (color == "WHITE")
				{
					firstPlayer = WHITE;
				}
			}
			else if (option == "MaxSearchTime")
			{
				maxSearchTime = stoi(line.substr(line.find(" ") + 1));
			}
			else if (option == "MaxSearchDepth")
			{
				maxSearchDepth = stoi(line.substr(line.find(" ") + 1));
			}
		}
	}
	inputFile.close();
}


// Have the player select their color
int SelectPlayerColor()
{
	std::string input;

	while (true)
	{
		std::cout << "Enter player color (b/w):\n";
		std::cin >> input;

		if (input == "b")
		{
			return BLACK;
		}
		else if (input == "w")
		{
			return WHITE;
		}
		else
		{
			std::cout << "Invalid input\n\n";
		}
	}
}


// Update the game tree
void UpdateGameTree(TreeNode* currentRootNode)
{
	std::cout << "Building game tree...\n";
	TIMEPOINT startTime = CURRENT_TIME;
	int depth = TreeNode::MakeTree(currentRootNode, maxSearchTime, maxSearchDepth);
	TIMEPOINT currentTime = CURRENT_TIME;
	int elapsedTime = DURATION(startTime, currentTime);
	std::cout << "Stopped at depth " << depth << " after " << elapsedTime << " seconds.\n\n";
}


// Main loop
int main()
{
	ReadConfig("..\\settings.txt");
	int playerColor = SelectPlayerColor();
	std::cout << "\nSelected " << (playerColor == BLACK ? "Black" : "White") << "\n\n";

	TreeNode* currentRootNode = new TreeNode(startingBoardFile);
	int currentTurn = firstPlayer;
	UpdateGameTree(currentRootNode);

	while (true)
	{
		std::cout << currentRootNode->PrintBoardState() << "\n" << (currentTurn == BLACK ? "Black" : "White") << " turn\n";

		TreeNode* newRootNode = NULL;
		if (currentTurn == playerColor)
		{
			// Player turn
			std::pair<int, int> move;
			std::string input;

			while (true)
			{
				std::cout << "\nEnter move (row,column or pass):\n";
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
			UpdateGameTree(currentRootNode);
			TreeNode::EvaluateNode(currentRootNode);
			newRootNode = currentRootNode->m_BestChildNode;
		}

		TreeNode::ChangeRoot(currentRootNode, newRootNode);
		currentRootNode = newRootNode;
		if (currentRootNode->m_GameOver)
		{
			break;
		}

		currentTurn = (currentTurn == BLACK ? WHITE : BLACK);
	}

	std::pair<int, int> discCount = currentRootNode->DiscCount();
	std::cout << "Game over\n" << discCount.first << "-" << discCount.second << "\n";
	if (discCount.first > discCount.second)
	{
		std::cout << "Black wins!\n";
	}
	else if (discCount.first < discCount.second)
	{
		std::cout << "White wins!\n";
	}
	else
	{
		std::cout << "It's a tie!\n";
	}
}
