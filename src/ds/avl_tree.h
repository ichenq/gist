// Copyright (C) 2015 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#pragma once

#include <stddef.h>

enum 
{
    ROT_LEFT = -1,
    ROT_RIGHT = 1,
};

#define AVL_HEIGHT(node)    ((node) != NULL ? (node)->height : -1)

template <typename K, typename T>
struct AVLTreeNode
{
    int             height = 0;
    AVLTreeNode*    left = nullptr;
    AVLTreeNode*    right = nullptr;
    K               key;
    T               data;
};

template <typename K, typename T>
class AVLTree
{
public:
    size_t Size() const {
        return size_;
    }

    AVLTreeNode<K, T>* FindMinNode() {
        return findMinNode(&root_);
    }

    AVLTreeNode<K, T>* FindMaxNode() {
        return findMaxNode(&root_);
    }

    T& Find(const K& key);

    bool Insert(const K& key, const T& data);
    bool Remove(const K& key);

private:
    AVLTreeNode<K, T>*  createNode(const K& key, const T& data);
    void                freeNode(AVLTreeNode<K, T>* node);

    int getBalance(AVLTreeNode<K, T>* node);

    AVLTreeNode<K, T>* findMinNode(AVLTreeNode<K, T>* node);
    AVLTreeNode<K, T>* findMaxNode(AVLTreeNode<K, T>* node);
    AVLTreeNode<K, T>* findNode(AVLTreeNode<K, T>* node, const K& key);

    AVLTreeNode<K, T>* rotateLL(AVLTreeNode<K, T>* node);
    AVLTreeNode<K, T>* rotateRR(AVLTreeNode<K, T>* node);
    AVLTreeNode<K, T>* rotateLR(AVLTreeNode<K, T>* node);
    AVLTreeNode<K, T>* rotateRL(AVLTreeNode<K, T>* node);

    AVLTreeNode<K, T>* balanceNode(AVLTreeNode<K, T>* node, int factor, int rotate);
    AVLTreeNode<K, T>* insertNode(AVLTreeNode<K, T>* node, const K& key, const T& data);
    AVLTreeNode<K, T>* removeNode(AVLTreeNode<K, T>* node, const K& key);

private:
    AVLTreeNode<K, T>*  root_ = nullptr;
    size_t              size_ = 0;
};

#include "avl_tree-inl.h"
