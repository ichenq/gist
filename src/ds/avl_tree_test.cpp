// Copyright (C) 2015 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#include "avl_tree.h"
#include <stdlib.h>

void simple_test()
{
    AVLTree<int, int> tree;
    for (int i = 0; i < 10; i++) {
        bool ok = tree.Insert(i, rand() % 1000);
        assert(ok);
    }

    for (int i = 0; i < 10; i++) {
        tree.Find(i);
    }
    for (int i = 0; i < 10; i++) {
        tree.Remove(i);
    }
}