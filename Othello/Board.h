#pragma once

#define EMPTY 0
#define BLACK -1
#define WHITE 1

#include <vector>
#include <string>

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
