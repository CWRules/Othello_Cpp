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
	TreeNode(std::vector<std::vector<int>> boardState, int turn);
	~TreeNode();

	void ReadBoardState(std::string fileName);
	std::string PrintBoardState();
	static void MakeTree(TreeNode* rootNode, int searchTime);

private:
	TreeNode* m_Parent;
	std::vector<TreeNode*> m_Children;
	std::vector<std::vector<int>> m_BoardState;
	std::set<std::pair<int, int>> m_AdjacentCells;
	int m_Turn;
	bool m_Passed;
	bool m_GameOver;

	void UpdateAdjacentCells(int x, int y);
	void MakeChildren();
};
