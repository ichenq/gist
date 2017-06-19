#include "expr_tree.h"
#include <assert.h>
#include <stack>
#include <sstream>
#include <iostream>

void FreeTree(BinaryTreeNode*& node)
{
    if (node != nullptr)
    {
        if (node->left != nullptr)
        {
            FreeTree(node->left);
        }
        if (node->right != nullptr)
        {
            FreeTree(node->right);
        }
        delete node;
        node = nullptr;
    }
}

void InfixTraversal(const BinaryTreeNode* tree, std::ostream& out)
{
    if (tree != nullptr)
    {
        if (tree->type == TOKEN_OPERATOR)
        {
            out << "( ";
        }
        InfixTraversal(tree->left, out);
        out << " " << tree->token << " ";
        InfixTraversal(tree->right, out);
        if (tree->type == TOKEN_OPERATOR)
        {
            out << " )";
        }
    }
}

void PostfixTraversal(const BinaryTreeNode* tree, std::ostream& out)
{
    if (tree != nullptr)
    {
        PostfixTraversal(tree->left, out);
        PostfixTraversal(tree->right, out);
        out << " " << tree->token << " ";
    }
}

void PrefixTraversal(const BinaryTreeNode* tree, std::ostream& out)
{
    if (tree != nullptr)
    {
        out << " " << tree->token << " ";
        PrefixTraversal(tree->left, out);
        PrefixTraversal(tree->right, out);
    }
}

BinaryTreeNode* ParsePostfixExpr(const std::string& expr)
{
    std::stack<BinaryTreeNode*> stk;
    std::istringstream strm(expr);
    std::string token;
    while (strm >> token)
    {
        double number;
        if (std::istringstream(token) >> number) // is operand?
        {
            auto node = new BinaryTreeNode(TOKEN_OPERAND, token);
            stk.push(node);
            continue;
        }
        auto node = new BinaryTreeNode(TOKEN_OPERATOR, token);
        assert(stk.size() >= 2);
        auto rhs = stk.top();
        stk.pop();
        auto lhs = stk.top();
        stk.pop();
        node->left = lhs;
        node->right = rhs;
        stk.push(node);
    }
    assert(stk.size() == 1);
    return stk.top();
}

void unit_test()
{
    const std::string expr = "5 1 2 + 4 ¡Á + 3 -";
    auto tree = ParsePostfixExpr(expr);
    InfixTraversal(tree, std::cout);
    std::cout << std::endl;
    PostfixTraversal(tree, std::cout);
    std::cout << std::endl;
    InfixTraversal(tree, std::cout);
    std::cout << std::endl;
    FreeTree(tree);
    
}
