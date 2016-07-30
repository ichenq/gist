// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

template <typename Iter>
void bubble_sort(Iter p, int size)
{
	for (int n = size; n >= 0; n--)
	{
		for (int i = 1; i < n; i++)
		{
			if (p[i - 1] > p[i])
			{
				std::swap(p[i-1], p[i]);
			}
		}
	}
}

template <typename Iter>
void insertion_sort(Iter p, int size)
{
	for (int i = 1; i < size; i++)
	{
		for (int j = i; j > 0 && p[j - 1] > p[j]; j--)
		{
			std::swap(p[j], p[j - 1]);
		}
	}
}

template <typename Iter>
void shell_sort(Iter p, int size)
{
	for (int gap = size >> 1; gap > 0; gap >>= 1)
	{
		for (int i = gap; i < size; i++)
		{
			auto tmp = p[i];
			int j = i - gap;
			for (; j >= 0 && p[j] > tmp; j -= gap)
			{
				p[j + gap] = p[j];
			}
			p[j + gap] = tmp;
		}
	}
}