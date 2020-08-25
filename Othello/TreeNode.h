#pragma once

#define EMPTY 0
#define BLACK -1
#define WHITE 1

#include <vector>
#include <string>
#include <set>

const std::vector<std::pair<int, int>> DIRECTIONS =
	{ std::make_pair(-1, -1),
	  std::make_pair(0, -1),
	  std::make_pair(1, -1),
	  std::make_pair(1, 0),
	  std::make_pair(1, 1),
	  std::make_pair(0, 1),
	  std::make_pair(-1, 1),
	  std::make_pair(-1, 0) };

class TreeNode
{
public:
	TreeNode(std::string fileName);
	TreeNode(TreeNode* parent);
	~TreeNode();

	void ReadBoardState(std::string fileName);
	std::string PrintBoardState();

private:
	TreeNode* m_Parent;
	std::vector<TreeNode*> m_Children;
	std::vector<std::vector<int>> m_BoardState;
	std::set<std::pair<int, int>> m_AdjacentCells;

	void AddAdjacentCells(int x, int y);
	void MakeChild(int x, int y, int turn);
};
