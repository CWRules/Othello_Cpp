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

const std::pair<int, int> PASS_TURN = std::make_pair(-1, -1);


class TreeNode
{
public:
	TreeNode(std::string fileName);
	TreeNode(std::vector<std::vector<int>> boardState, int turn, std::pair<int, int> move);
	~TreeNode();

	void ReadBoardState(std::string fileName);
	std::string PrintBoardState();
	static void MakeTree(TreeNode* rootNode, int searchTime);
	static void EvaluateNode(TreeNode* node);
	static TreeNode* SelectMove(TreeNode* rootNode, std::pair<int, int> move);

private:
	TreeNode* m_Parent;
	std::vector<TreeNode*> m_Children;
	std::vector<std::vector<int>> m_BoardState;
	std::set<std::pair<int, int>> m_AdjacentCells;
	int m_Turn;
	std::pair<int, int> m_LastMove;
	bool m_GameOver;
	double m_Value;
	TreeNode* m_BestChildNode;

	void UpdateAdjacentCells(int x, int y);
	void MakeChildren();
};
