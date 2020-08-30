/*
This class implements the tree of board states used by the AI to evaluate moves.
*/

#include "TreeNode.h"
#include <fstream>


// Construct a root note by reading in a board state
TreeNode::TreeNode(std::string fileName)
{
	m_Parent = NULL;
	ReadBoardState(fileName);
	m_Turn = BLACK;

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
void TreeNode::MakeChildren(int turn)
{
	int nextTurn = (turn == BLACK ? WHITE : BLACK);

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
				else if (m_BoardState[cell.first][cell.second] == turn)
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
			TreeNode* ChildNode = new TreeNode(m_BoardState, nextTurn);
			ChildNode->m_Parent = this;
			m_Children.push_back(ChildNode);
			ChildNode->m_AdjacentCells = m_AdjacentCells;
			ChildNode->UpdateAdjacentCells(move.first, move.second);
		}
	}

	// If no valid moves, make a single child which passes the turn
	if (m_Children.size() == 0)
	{
		TreeNode* ChildNode = new TreeNode(m_BoardState, nextTurn);
		ChildNode->m_Parent = this;
		m_Children.push_back(ChildNode);
		ChildNode->m_AdjacentCells = m_AdjacentCells;
	}
}


// Recursively build the game tree
void TreeNode::MakeTree()
{
	// Make a queue of nodes starting from root
	// Call MakeChildren() for each and add children to queue
	// Estimate time needed for next level and terminate search if too long
	// Evaluate nodes
}
