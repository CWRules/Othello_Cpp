#pragma once

#include "Board.h"
#include <vector>

class TreeNode
{
public:
	TreeNode(Board boardState, TreeNode* parent = NULL);
	~TreeNode();

	Board m_Board;

private:
	TreeNode* m_Parent;
	std::vector<TreeNode*> m_Children;
};
