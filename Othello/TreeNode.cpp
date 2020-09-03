/*
This class implements the tree of board states used by the AI to evaluate moves.
*/

#include "TreeNode.h"
#include <fstream>
#include <chrono>
#include <iostream>


// Construct a root note by reading in a board state
TreeNode::TreeNode(std::string fileName)
{
	m_Parent = NULL;
	ReadBoardState(fileName);
	m_Turn = BLACK;
	m_Passed = false;
	m_GameOver = false;

	// Initialize set of adjacent cells
	for (unsigned int i = 0; i < m_BoardState.size(); ++i)
	{
		for (unsigned int j = 0; j < m_BoardState[i].size(); ++j)
		{
			if (m_BoardState[i][j] != EMPTY)
			{
				UpdateAdjacentCells(i, j);
			}
		}
	}
}


// Construct a child for an existng node
TreeNode::TreeNode(std::vector<std::vector<int>> boardState, int turn)
{
	m_BoardState = boardState;
	m_Turn = turn;
	m_Passed = false;
	m_GameOver = false;
}


TreeNode::~TreeNode()
{
	// Need to destroy all child nodes
	for (TreeNode* child : m_Children)
	{
		delete child;
	}
}


// Read a board state from a text file
void TreeNode::ReadBoardState(std::string fileName)
{
	std::ifstream inputFile(fileName);

	if (inputFile.is_open())
	{
		m_BoardState.clear();
		std::string line;

		while (std::getline(inputFile, line))
		{
			if (line == "" || line[0] == '#')
			{
				continue;
			}

			std::vector<int> row;
			for (char& c : line)
			{
				if (c == 'B')
				{
					row.push_back(BLACK);
				}
				else if (c == 'W')
				{
					row.push_back(WHITE);
				}
				else
				{
					row.push_back(EMPTY);
				}
			}
			m_BoardState.push_back(row);
		}
	}
	inputFile.close();
}


// Returns a string representation of the current board state
std::string TreeNode::PrintBoardState()
{
	std::string output = "  ";
	if (m_BoardState.size() > 0)
	{
		for (unsigned int i = 0; i < m_BoardState[0].size(); ++i)
		{
			output += std::to_string(i) + " ";
		}
		output += "\n";
	}

	for (unsigned int i = 0; i < m_BoardState.size(); ++i)
	{
		output += std::to_string(i) + "|";
		for (int cell : m_BoardState[i])
		{
			if (cell == WHITE)
			{
				output += "W|";
			}
			else if (cell == BLACK)
			{
				output += "B|";
			}
			else
			{
				output += " |";
			}
		}
		output += "\n";
	}
	return output;
}


// Remove the given cell from the set of adjacent cells and add the ones adjacent to it
void TreeNode::UpdateAdjacentCells(int x, int y)
{
	m_AdjacentCells.erase(std::make_pair(x, y));

	for (std::pair<int, int> dir : DIRECTIONS)
	{
		std::pair<int, int> cell = std::make_pair(x + dir.first, y + dir.second);

		if (cell.first >= 0 && cell.first < m_BoardState.size()
			&& cell.second >= 0 && cell.second < m_BoardState[cell.first].size()
			&& m_BoardState[cell.first][cell.second] == EMPTY)
		{
			m_AdjacentCells.insert(cell);
		}
	}
}


// Check adjacent cells to see which are valid moves and make child nodes as required
void TreeNode::MakeChildren()
{
	int nextTurn = (m_Turn == BLACK ? WHITE : BLACK);

	for (std::pair<int, int> move : m_AdjacentCells)
	{
		std::vector<std::pair<int, int>> cellsToFlip;

		for (std::pair<int, int> dir : DIRECTIONS)
		{
			std::vector<std::pair<int, int>> newCells;
			std::pair<int, int> cell = std::make_pair(move.first + dir.first, move.second + dir.second);

			while (cell.first >= 0 && cell.first < m_BoardState.size()
				&& cell.second >= 0 && cell.second < m_BoardState[cell.first].size())
			{
				if (m_BoardState[cell.first][cell.second] == EMPTY)
				{
					break;
				}
				else if (m_BoardState[cell.first][cell.second] == m_Turn)
				{
					cellsToFlip.insert(cellsToFlip.end(), newCells.begin(), newCells.end());
				}
				else
				{
					newCells.push_back(cell);
				}
				cell.first += dir.first;
				cell.second += dir.second;
			}
		}

		// If at least one cell is flipped, make a child node
		if (cellsToFlip.size() > 0)
		{
			TreeNode* childNode = new TreeNode(m_BoardState, nextTurn);
			childNode->m_Parent = this;
			m_Children.push_back(childNode);
			childNode->m_AdjacentCells = m_AdjacentCells;
			childNode->UpdateAdjacentCells(move.first, move.second);

			childNode->m_BoardState[move.first][move.second] = m_Turn;
			for (std::pair<int, int> cell : cellsToFlip)
			{
				childNode->m_BoardState[cell.first][cell.second] = m_Turn;
			}
		}
	}

	// If no valid moves, make a single child which passes the turn
	if (m_Children.size() == 0)
	{
		TreeNode* childNode = new TreeNode(m_BoardState, nextTurn);
		childNode->m_Parent = this;
		m_Children.push_back(childNode);
		childNode->m_AdjacentCells = m_AdjacentCells;
		childNode->m_Passed = true;

		if (m_Passed)
		{
			childNode->m_GameOver = true;
		}
	}
}


// Recursively build the game tree
void TreeNode::MakeTree(TreeNode* rootNode, int searchTime)
{
	int depth = 1;
	std::vector<TreeNode*> nodeList = { rootNode };
	std::vector<TreeNode*> childNodeList;
	std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

	std::cout << "Building tree...\n";
	while (true)
	{
		std::chrono::steady_clock::time_point layerStartTime = std::chrono::steady_clock::now();

		for (TreeNode* node : nodeList)
		{
			if (node->m_Children.size() == 0 && node->m_GameOver == false)
			{
				node->MakeChildren();
			}
			childNodeList.insert(childNodeList.end(), node->m_Children.begin(), node->m_Children.end());
		}

		// Estimate time for next layer
		std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
		int totalTime = (int)std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
		int layerTime = (int)std::chrono::duration_cast<std::chrono::seconds>(currentTime - layerStartTime).count();
		int nextLayerTimeEstimate = layerTime * (childNodeList.size() / nodeList.size());
		int totalTimeAfterNextLayer = totalTime + nextLayerTimeEstimate;

		if (childNodeList.size() == 0 || totalTimeAfterNextLayer > searchTime)
		{
			std::cout << "Stopped at depth " << depth  << " after " << totalTime << " seconds.\n";
			return;
		}
		else
		{
			nodeList = childNodeList;
			childNodeList.clear();
			++depth;
		}
	}
}
