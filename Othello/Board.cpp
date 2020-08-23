/*
This class is used to store the state of an Othello board.
*/

#include "Board.h"
#include <fstream>


Board::Board(std::string fileName)
{
	if (fileName != "")
	{
		ReadBoardState(fileName);
	}
}


Board::~Board()
{
}


// Read a board state from a text file
void Board::ReadBoardState(std::string fileName)
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
std::string Board::PrintBoardState()
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
