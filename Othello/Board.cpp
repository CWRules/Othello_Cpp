#include "Board.h"
#include <fstream>
#include <sstream>


Board::Board(std::string fileName)
{
	ReadBoardState(fileName);
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
				if (c == 'W')
				{
					row.push_back(WHITE);
				}
				else if (c == 'B')
				{
					row.push_back(BLACK);
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
	std::string output;
	for (std::vector<int> row : m_BoardState)
	{
		for (int i : row)
		{
			if (i == WHITE)
			{
				output.append("W");
			}
			else if (i == BLACK)
			{
				output.append("B");
			}
			else
			{
				output.append("-");
			}
		}
		output.append("\n");
	}
	return output;
}
