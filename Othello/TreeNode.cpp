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

	// Initialize set of adjacent cells
	for (unsigned int i = 0; i < m_BoardState.size(); ++i)
	{
		for (unsigned int j = 0; j < m_BoardState[i].size(); ++j)
		{
			if (m_BoardState[i][j] != EMPTY)
			{
				AddAdjacentCells(i, j);
			}
		}
	}
}


// Construct a child for an existng node
TreeNode::TreeNode(TreeNode* parent)
{
	m_Parent = parent;
	parent->m_Children.push_back(this);
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


// Add cells adjacent to the given one to the set of adjacent cells for this node
void TreeNode::AddAdjacentCells(int x, int y)
{
	for (std::pair<int, int> dir : DIRECTIONS)
	{
		std::pair<int, int> cell = std::make_pair(dir.first + x, dir.second + y);

		if (cell.first >= 0 && cell.first < m_BoardState.size()
			&& cell.second >= 0 && cell.second < m_BoardState[cell.first].size()
			&& m_BoardState[cell.first][cell.second] == EMPTY)
		{
			m_AdjacentCells.insert(cell);
		}
	}
}


// See if the given move is valid, and if so make a child node for the new board state
void TreeNode::MakeChild(int x, int y, int turn)
{
}