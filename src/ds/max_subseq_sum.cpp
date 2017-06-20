// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.


/**
 * Max sub-sequence sum with different complexity
 */

// O(N^3) complexity
int MaxSubSequenceSumV1(const int array[], int n) {
    int maxSum = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            int thisSum = 0;
            for (int k = i; k <= j; k++) {
                thisSum += array[k];
            }
            // range of [i, j]
            if (thisSum > maxSum) {
                maxSum = thisSum;
            }
        }
    }
    return maxSum;
}

// O(N^2) complexity
int MaxSubSequenceSumV2(const int array[], int n) {
    int maxSum = 0;
    for (int i = 0; i < n; i++) {
        int thisSum = 0;
        for (int j = i; j < n; j++) {
            thisSum += array[j];
            if (thisSum > maxSum) { // range of [i, j]
                maxSum = thisSum;
            }
        }
    }
    return maxSum;
}

// O(N*logN) complexity
static int maxSubSum(const int array[], int left, int right) {
    if (left == right) { // base case
        if (array[left] > 0) {
            return array[left];
        }
        else {
            return 0;
        }
    }
    int center = (left + right) / 2;
    int maxLeftSum = maxSubSum(array, left, center);
    int maxRightSum = maxSubSum(array, center, right);

    int leftBorderSum = 0, maxLeftBorderSum = 0;
    for (int i = center; i >= left; i--) {
        leftBorderSum += array[i];
        if (leftBorderSum > maxLeftBorderSum) {
            maxLeftBorderSum = leftBorderSum;
        }
    }
    int rightBorderSum = 0, maxRightBorderSum = 0;
    for (int i = center + 1; i <= right; i++) {
        rightBorderSum += array[i];
        if (rightBorderSum > maxRightBorderSum) {
            maxRightBorderSum = rightBorderSum;
        }
    }
    int maxSum = maxLeftBorderSum + maxRightBorderSum;
    if (maxLeftSum > maxSum) {
        maxSum = maxLeftSum;
    }
    if (maxRightSum > maxSum) {
        maxSum = maxRightSum;
    }
    return maxSum;
}


int MaxSubSequenceSumV3(const int array[], int n) {
    return maxSubSum(array, 0, n - 1);
}

// O(N) complexity
int MaxSumSequenceSumV4(const int array[], int n) {
    int thisSum = 0, maxSum = 0;
    for (int i = 0; i < n; i++) {
        thisSum += array[i];
        if (thisSum > maxSum) {
            maxSum = thisSum;
        } else if (thisSum < 0) {
            thisSum = 0;
        }
    }
    return maxSum;
}
