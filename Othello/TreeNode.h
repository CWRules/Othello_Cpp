#pragma once

#include <vector>
#include <string>
#include <set>
#include <chrono>
#include <random>

#define EMPTY 0
#define BLACK -1
#define WHITE 1

#define TIMEPOINT std::chrono::steady_clock::time_point
#define CURRENT_TIME std::chrono::steady_clock::now()
#define DURATION(a, b) (int)std::chrono::duration_cast<std::chrono::seconds>(b - a).count();

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
	std::pair<int, int> DiscCount();

	static int MakeTree(TreeNode* rootNode, int searchTime, int maxDepth);
	static void EvaluateNode(TreeNode* node);
	static void EvaluateBoardState(TreeNode* node);
	static TreeNode* SelectMove(TreeNode* rootNode, std::pair<int, int> move);
	static void ChangeRoot(TreeNode* rootNode, TreeNode* childNode);

	TreeNode* m_BestChildNode;
	bool m_GameOver;

private:
	TreeNode* m_Parent;
	std::vector<TreeNode*> m_Children;
	std::vector<std::vector<int>> m_BoardState;
	std::set<std::pair<int, int>> m_AdjacentCells;
	int m_Turn;
	std::pair<int, int> m_LastMove;
	double m_Value;

	void UpdateAdjacentCells(int x, int y);
	void MakeChildren();

	static std::random_device s_RandomDevice;
	static std::mt19937 s_RandomGenerator;
	static std::uniform_real_distribution<> s_RandomDistribution;
};
