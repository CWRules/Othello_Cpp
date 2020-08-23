/*
This class implements the tree of board states used by the AI to evaluate moves.
*/

#include "TreeNode.h"


TreeNode::TreeNode(Board boardState, TreeNode* parent)
{
	m_Board = boardState;
	m_Parent = parent;
}


TreeNode::~TreeNode()
{
	for (TreeNode* child : m_Children)
	{
		delete child;
	}
}
