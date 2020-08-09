#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

void print_array(const vector<int>& a)
{
    for (size_t i = 0; i < a.size(); i++)
    {
        cout << a[i] << " ";
    }
    cout << endl;
}

// https://leetcode.com/problems/how-many-numbers-are-smaller-than-the-current-number/
vector<int> smallerNumbersThanCurrent(vector<int>& nums) 
{
    vector<int> result(nums.size(), 0);
    for (size_t i = 0; i < nums.size(); i++)
    {
        int pivot = nums[i];
        for (size_t j = 0; j < nums.size(); j++)
        {
            if (j != i && nums[j] < pivot)
            {
                result[i]++;
            }
        }
    }
    return result;
}

void test_array()
{
    {
        vector<int> arr1{ 8, 1, 2, 2, 3 };
        auto result = smallerNumbersThanCurrent(arr1);
        print_array(result);
    }
}