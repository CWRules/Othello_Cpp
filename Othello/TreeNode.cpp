/*
This class implements the tree of board states used by the AI to evaluate moves.
*/

#include "TreeNode.h"
#include <fstream>
#include <chrono>
#include <algorithm>
#include <iostream>


// Construct a root note by reading in a board state
TreeNode::TreeNode(std::string fileName)
{
	m_Parent = NULL;
	ReadBoardState(fileName);
	m_Turn = BLACK;
	m_LastMove = PASS_TURN;
	m_GameOver = false;
	m_Value = 0;
	m_BestChildNode = NULL;

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
TreeNode::TreeNode(std::vector<std::vector<int>> boardState, int turn, std::pair<int, int> move)
{
	m_BoardState = boardState;
	m_Turn = turn;
	m_LastMove = move;
	m_GameOver = false;
	m_Value = 0;
	m_BestChildNode = NULL;
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

		if (cell.first >= 0 && cell.first < (int)m_BoardState.size()
			&& cell.second >= 0 && cell.second < (int)m_BoardState[cell.first].size()
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

			while (cell.first >= 0 && cell.first < (int)m_BoardState.size()
				&& cell.second >= 0 && cell.second < (int)m_BoardState[cell.first].size())
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
			TreeNode* childNode = new TreeNode(m_BoardState, nextTurn, move);
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
		TreeNode* childNode = new TreeNode(m_BoardState, nextTurn, PASS_TURN);
		childNode->m_Parent = this;
		m_Children.push_back(childNode);
		childNode->m_AdjacentCells = m_AdjacentCells;

		if (m_LastMove == PASS_TURN)
		{
			childNode->m_GameOver = true;
		}
	}
}


// Build the game tree
int TreeNode::MakeTree(TreeNode* rootNode, int searchTime)
{
	int depth = 1;
	std::vector<TreeNode*> nodeList = { rootNode };
	std::vector<TreeNode*> childNodeList;
	std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

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
			return depth;
		}
		else
		{
			nodeList = childNodeList;
			childNodeList.clear();
			++depth;
		}
	}
}


// Recursively evaluate nodes in the tree
void TreeNode::EvaluateNode(TreeNode* node)
{
	if (node->m_Children.size() == 0)
	{
		EvaluateBoardState(node);
	}
	else
	{
		TreeNode* bestNode = NULL;
		for (TreeNode* childNode : node->m_Children)
		{
			EvaluateNode(childNode);
			if (bestNode == NULL ||
				(node->m_Turn == BLACK && childNode->m_Value < bestNode->m_Value) ||
				(node->m_Turn == WHITE && childNode->m_Value > bestNode->m_Value))
			{
				bestNode = childNode;
			}
		}
		node->m_Value = bestNode->m_Value;
		node->m_BestChildNode = bestNode;
	}
}


// Determine the value of the board state in the given node
// Black is the minimizing player, white is maximizing
void TreeNode::EvaluateBoardState(TreeNode* node)
{
	// If the game is over, just count the discs of each color
	if (node->m_GameOver)
	{
		for (std::vector<int> row : node->m_BoardState)
		{
			for (int cell : row)
			{
				node->m_Value += cell;
			}
		}
		node->m_Value *= 1000;
		return;
	}
	
	// Random adjustment
	node->m_Value = s_RandomDistribution(s_RandomGenerator);
}


// Find the child node that matches the given move
// If the given move is not vaid, return NULL.
TreeNode* TreeNode::SelectMove(TreeNode* rootNode, std::pair<int, int> move)
{
	for (TreeNode* childNode : rootNode->m_Children)
	{
		if (childNode->m_LastMove == move)
		{
			return childNode;
		}
	}
	return NULL;
}


// Prune the tree so that the given child node becomes the new root
void TreeNode::ChangeRoot(TreeNode* rootNode, TreeNode* childNode)
{
	rootNode->m_Children.erase(std::remove(rootNode->m_Children.begin(), rootNode->m_Children.end(), childNode), rootNode->m_Children.end());
	delete rootNode;
	childNode->m_Parent = NULL;
}


// Used to generate random numbers for the evaluation function
std::random_device TreeNode::s_RandomDevice;
std::mt19937 TreeNode::s_RandomGenerator(s_RandomDevice());
std::uniform_real_distribution<> TreeNode::s_RandomDistribution(-1.0, 1.0);