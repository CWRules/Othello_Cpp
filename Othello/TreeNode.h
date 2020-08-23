#pragma once

#define EMPTY 0
#define BLACK -1
#define WHITE 1

#include <vector>
#include <string>

class TreeNode
{
public:
	TreeNode(TreeNode* parent);
	TreeNode(std::string fileName);
	~TreeNode();

	void ReadBoardState(std::string fileName);
	std::string PrintBoardState();

private:
	TreeNode* m_Parent;
	std::vector<TreeNode*> m_Children;
	std::vector<std::vector<int>> m_BoardState;
};
