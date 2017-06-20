// Copyright (C) 2015 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#include <assert.h>
#include <stdexcept>
#include <algorithm> // std::max


template <typename K, typename T>
T& AVLTree<K, T>::Find(const K& key)
{
    auto node = findNode(root_, key);
    if (node != nullptr) {
        return node->data;
    }
    throw std::runtime_error("key not found");
}

template <typename K, typename T>
bool AVLTree<K, T>::Insert(const K& key, const T& data)
{
    root_ = insertNode(root_, key, data);
    if (root_ != nullptr) {
        size_++;
        return true;
    }
    return false;
}

template <typename K, typename T>
bool AVLTree<K, T>::Remove(const K& key)
{
    auto node = removeNode(root_, key);
    if (node != nullptr) {
        freeNode(node);
        size_--;
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////

template <typename K, typename T>
AVLTreeNode<K, T>* AVLTree<K, T>::createNode(const K& key, const T& data)
{
    auto node = new AVLTreeNode<K, T>();
    node->key = key;
    node->data = data;
    return node;
}

template <typename K, typename T>
void AVLTree<K, T>::freeNode(AVLTreeNode<K, T>* node)
{
    delete node;
}

template <typename K, typename T>
int AVLTree<K, T>::getBalance(AVLTreeNode<K, T>* node)
{
    if (node != nullptr) {
        return AVL_HEIGHT(node->left) - AVL_HEIGHT(node->right);
    }
    return 0;
}

template <typename K, typename T>
AVLTreeNode<K, T>* AVLTree<K, T>::findMinNode(AVLTreeNode<K, T>* node)
{
    if (node != nullptr) {
        while (node->left != nullptr) {
            node = node->left;
        }
    }
    return node;
}

template <typename K, typename T>
AVLTreeNode<K, T>* AVLTree<K, T>::findMaxNode(AVLTreeNode<K, T>* node)
{
    if (node != nullptr) {
        while (node->right != nullptr) {
            node = node->right;
        }
    }
    return node;
}

template <typename K, typename T>
AVLTreeNode<K, T>* AVLTree<K, T>::findNode(AVLTreeNode<K, T>* node, const K& key)
{
    if (node == nullptr) {
        return nullptr;
    }
    if (key < node->key) {
        return findNode(node->left, key);
    } else if (node->key < key) {
        return findNode(node->right, key);
    } else {
        return node;
    }
}


// left-left rotation
//
//        A               B
//       / \            /   \
//      B   C          D     A
//     / \     ==>    / \   / \
//    D   E          F   G E   C
//   / \
//  F   G
//
template <typename K, typename T>
AVLTreeNode<K, T>* AVLTree<K, T>::rotateLL(AVLTreeNode<K, T>* A)
{
    assert(A != nullptr && A->left != nullptr);
    auto B = A->left;
    A->left = B->right;
    B->right = A;

    // update height
    A->height = std::max(AVL_HEIGHT(A->left), AVL_HEIGHT(A->right)) + 1;
    B->height = std::max(AVL_HEIGHT(B->left), AVL_HEIGHT(B->right)) + 1;

    return B;
}


// right-right rotation
//
//    A                 C
//   / \              /   \
//  B   C            A     E
//     / \   ==>    / \   / \
//    D   E        B   D F   G
//       / \
//      F   G
//
template <typename K, typename T>
AVLTreeNode<K, T>* AVLTree<K, T>::rotateRR(AVLTreeNode<K, T>* A)
{
    assert(A != nullptr && A->right != nullptr);
    auto C = A->right;
    A->right = C->left;
    C->left = A;

    // update height
    A->height = std::max(AVL_HEIGHT(A->left), AVL_HEIGHT(A->right)) + 1;
    C->height = std::max(AVL_HEIGHT(C->left), AVL_HEIGHT(C->right)) + 1;

    return C;
}

//
//      A              A              E
//     / \            / \           /   \
//    B   C          E   C         B     A
//   / \     ==>    / \     ==>   / \   / \
//  D   E          B   G         D   F G   C
//     / \        / \
//    F   G      D   F
//
template <typename K, typename T>
AVLTreeNode<K, T>* AVLTree<K, T>::rotateLR(AVLTreeNode<K, T>* A)
{
    assert(A != nullptr && A->left != nullptr);
    A->left = rotateRR(A->left);
    return rotateLL(A);
}


// right-right rotation
//    A             A               D
//   / \           / \            /   \
//  B   C         B   D          A     C
//     / \   ==>     / \   ==>  / \   / \
//    D   E         F   C      B   F G   E
//   / \               / \
//  F   G             G   E
//
template <typename K, typename T>
AVLTreeNode<K, T>* AVLTree<K, T>::rotateRL(AVLTreeNode<K, T>* A)
{
    assert(A != nullptr && A->right != nullptr);
    A->right = rotateLL(A->right);
    return rotateRR(A);
}

// re-balance the tree after insertion/deletion
template <typename K, typename T>
AVLTreeNode<K, T>* AVLTree<K, T>::balanceNode(AVLTreeNode<K, T>* node, int factor, int rotate)
{
    if (node == nullptr) {
        return nullptr;
    }
    if (factor > 1) { // left subtree need re-balance
        switch (rotate) {
        case ROT_LEFT:
            return rotateLL(node);
        case ROT_RIGHT:
            return rotateLR(node);
        default:
            assert(!"invalid rotation");
        }
    }
    else if (factor < -1) { // right subtree need re-balance
        switch (rotate) {
        case ROT_LEFT:
            return rotateRL(node);
        case ROT_RIGHT:
            return rotateRR(node);
        default:
            assert(!"invlaid rotation");
        }
    }
    return node;
}

template <typename K, typename T>
AVLTreeNode<K, T>* AVLTree<K, T>::insertNode(AVLTreeNode<K, T>* node, const K& key, const T& data)
{
    int rotate = 0;
    if (node == nullptr) {
        return createNode(key, data);
    }
    if (key < node->key) {
        rotate = ROT_LEFT;
        node->left = insertNode(node->left, key, data);
    }
    else if (node->key < key) {
        rotate = ROT_RIGHT;
        node->right = insertNode(node->right, key, data);
    }
    else {
        assert(!"duplicate key");
    }
    // update height
    node->height = std::max(AVL_HEIGHT(node->left), AVL_HEIGHT(node->right)) + 1;
    return balanceNode(node, getBalance(node), rotate);
}

// delete an node from tree
template <typename K, typename T>
AVLTreeNode<K, T>* AVLTree<K, T>::removeNode(AVLTreeNode<K, T>* root, const K& key)
{
    if (root == nullptr) {
        return nullptr;
    }
    if (key < root->key) {
        root->left = removeNode(root->left, key);
    } else if (root->key < key) {
        root->right = removeNode(root->right, key);
    } else { 
        // found node to be deleted
        if (root->left == nullptr || root->right == nullptr) {
            auto node = (root->left == nullptr ? root->right : root->left);
            if (node != nullptr) { // has one child
                *root = *node;
            } else {
                node = root;
                root = nullptr;
            }
        } else { // node with 2 children
            auto node = findMinNode(root->right);
            root->key = node->key;
            root->data = node->data;
            root->right = removeNode(root->right, node->key);
        }
    }
    if (root != nullptr) {
        int rotate = 0;
        root->height = std::max(AVL_HEIGHT(root->left), AVL_HEIGHT(root->right)) + 1;
        int factor = getBalance(root);
        if (factor > 1) {
            int balance = getBalance(root->left);
            rotate = (balance >= 0 ? ROT_LEFT : ROT_RIGHT);
        } else if (factor < -1) {
            int balance = getBalance(root->right);
            rotate = (balance <= 0 ? ROT_RIGHT : ROT_LEFT); 
        }
        return balanceNode(root, factor, rotate);
    }
    return nullptr;
}
