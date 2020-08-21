#pragma once

#define EMPTY 0
#define WHITE 1
#define BLACK -1

#include <vector>

class Board
{
public:
	Board(std::string fileName = "");
	~Board();

	void ReadBoardState(std::string fileName);
	std::string PrintBoardState();

private:
	std::vector<std::vector<int>> m_BoardState;
};
