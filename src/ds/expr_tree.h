// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#pragma once

#include <string>
#include <iosfwd>

/*
 * expression tree
 */
 
enum eTokenType
{
    TOKEN_OPERAND,
    TOKEN_OPERATOR,
};

struct BinaryTreeNode
{
    eTokenType      type;
    std::string     token;
    BinaryTreeNode* left = nullptr;     // left child
    BinaryTreeNode* right = nullptr;    // right child

    BinaryTreeNode(eTokenType t, const std::string& s)
        : type(t), token(s)
    {}
};

// 中序遍历: left subtree -> root -> right subtree
void InfixTraversal(const BinaryTreeNode* tree, std::ostream& out);

// 后序遍历: left subtree -> right subtree -> root
void PostfixTraversal(const BinaryTreeNode* tree, std::ostream& out);

// 前序遍历: root -> left subtree -> right subtree
void PrefixTraversal(const BinaryTreeNode* tree, std::ostream& out);

void FreeTree(BinaryTreeNode*& tree);

BinaryTreeNode* ParsePostfixExpr(const std::string& expr);
