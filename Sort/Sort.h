/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Copyright © 2021 chibayuki@foxmail.com

Sort.Sort
Version 21.3.20.0000

This file is part of Dependency
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#pragma once

#include <type_traits>

using namespace std;

// 比较两个对象，使之符合指定的排序判据，如果需要，那么交换这两个对象
template<typename T, typename Predicate> bool _CompareSwap(T& left, T& right, Predicate predicate)
{
	if (predicate(right, left))
	{
		swap(left, right);

		return true;
	}
	else
	{
		return false;
	}
}

#pragma region 内省相关的原位排序

// 堆排序（完全二叉排序）
template<typename Iterator, typename Predicate> void HeapSort(const Iterator begin, const Iterator end, Predicate predicate)
{
	// 原理：
	// 将序列中的元素构造为一个堆（完全二叉树），例如将 a[0..N-1] 构造为
	//           a[0]
	//    a[1]        a[2]
	// a[3] a[4]  a[5] a[6]
	//          ......
	// 显然，a[i] 的子节点（如果有）为 a[2i+1], a[2i+2]，
	// 实际上并不需要真的构造树结构，只需按此下标关系处理序列即可。
	//
	// 首先进行第一轮排序，
	// 比较最后一个非叶节点（a[N/2-1]）及其子节点，将三者（或二者）的最大值置换到该非叶节点处（如果必要），
	// 然后对倒数第二个，倒数第三个...非叶节点重复此操作，直到根节点，
	// 这样一轮排序的结果是，整个序列的最大值被置换到了根节点，
	// 接下来，把根节点与最后一个叶节点置换，即最大值被放在序列尾部，
	//
	// 然后进行第二轮排序，从倒数第二个非叶节点开始，重复以上操作，结果是整个序列的第二大值被放在序列倒数第二位置，
	//
	// 进行第三轮...排序，最后只剩序列头部的两个元素还未排序，直接比较排序即可。

	auto size = end - begin;

	if (size > 2)
	{
		for (auto s = size; s > 2; s--)
		{
			for (auto i = s / 2 - 1; i >= 0; i--)
			{
				auto& mid = *(begin + i);
				auto& left = *(begin + 2 * i + 1);

				if (2 * i + 2 < s)
				{
					auto& right = *(begin + 2 * i + 2);

					bool lessThanL = predicate(mid, left);
					bool lessThanR = predicate(mid, right);

					if (lessThanL && lessThanR)
					{
						if (predicate(left, right))
						{
							swap(mid, right);
						}
						else
						{
							swap(mid, left);
						}
					}
					else if (lessThanL)
					{
						swap(mid, left);
					}
					else if (lessThanR)
					{
						swap(mid, right);
					}
				}
				else
				{
					if (predicate(mid, left))
					{
						swap(mid, left);
					}
				}
			}

			swap(*begin, *(begin + s - 1));
		}
	}

	if (size > 1)
	{
		_CompareSwap(*begin, *(begin + 1), predicate);
	}
}

// 堆排序（完全二叉排序）（默认升序排序）
template<typename Iterator> inline void HeapSort(const Iterator begin, const Iterator end)
{
	HeapSort(begin, end, less<>());
}

// 快速排序
template<typename Iterator, typename Predicate> void QuickSort(const Iterator begin, const Iterator end, Predicate predicate)
{
	// 原理：
	// 快速排序每一轮排序按照某种规则从序列中选取一个基准值，通过交换元素使得所有比基准值小/大的元素分别位于基准值的左侧/右侧，
	// 第一轮排序针对整个序列，然后对基准值两侧的部分序列分别重复以上操作，直到只需要比较两个元素为止。

	auto size = end - begin;

	if (size > 2)
	{
		using Value = remove_reference_t<decltype(*begin)>;

		// 这里使用三点取中值，即选取 a[0]、a[-1]、a[N/2] 的中值作为基准值，这样可以尽可能避免选到最大值/最小值，导致这轮排序无作用
		Value& left = *begin, & right = *(end - 1), & mid = *(begin + size / 2);
		Value baseVal;

		if (predicate(left, right))
		{
			if (predicate(mid, left))
			{
				baseVal = left;
			}
			else if (predicate(right, mid))
			{
				baseVal = right;
			}
			else
			{
				baseVal = mid;
			}
		}
		else
		{
			if (predicate(mid, right))
			{
				baseVal = right;
			}
			else if (predicate(left, mid))
			{
				baseVal = left;
			}
			else
			{
				baseVal = mid;
			}
		}

		// 从序列两端分别向中间搜索，通过下面的循环使得所有比基准值小/大的元素分别在基准值的左侧/右侧
		Iterator low = begin, high = end - 1;

		while (low < high)
		{
			// 一旦在右侧找到比基准值小的元素
			while (low < high && predicate(baseVal, *high))
			{
				--high;
			}

			// 把上述元素交换至左侧，并右移左游标
			if (low < high)
			{
				swap(*low, *high);
				++low;
			}

			// 一旦在左侧找到比基准值大的元素
			while (low < high && predicate(*low, baseVal))
			{
				++low;
			}

			// 把上述元素交换至右侧，并左移右游标
			if (low < high)
			{
				swap(*low, *high);
				--high;
			}
		} // 最后，左、右游标重合，位于基准值处

		// 对于基准值两侧的元素，递归执行以上操作
		QuickSort(begin, low, predicate);
		QuickSort(low + 1, end, predicate);
	}
	else if (size > 1)
	{
		_CompareSwap(*begin, *(begin + 1), predicate);
	}
}

// 快速排序（默认升序排序）
template<typename Iterator> inline void QuickSort(const Iterator begin, const Iterator end)
{
	QuickSort(begin, end, less<>());
}

// 插入排序
template<typename Iterator, typename Predicate> void InsertSort(const Iterator begin, const Iterator end, Predicate predicate)
{
	// 原理：
	// 首先进行第一轮排序，观察序列的第二个元素 a[1]，如果 a[1] < a[0]，那么交换二者，
	// 然后进行第二轮排序，观察序列的第三个元素 a[2]，此时队列的前两个元素已经是有序的，将 a[2] 与它之前的元素依次比较（并交换），直到符合顺序，
	// 然后进行第三轮...排序，重复以上操作，直到序列的最后一个元素被移动至正确的位值。

	auto size = end - begin;

	if (size > 2)
	{
		for (size_t s = 1; s < size; s++)
		{
			for (size_t i = s; i > 0; i--)
			{
				if (!_CompareSwap(*(begin + i - 1), *(begin + i), predicate))
				{
					break;
				}
			}
		}
	}
	else if (size > 1)
	{
		_CompareSwap(*begin, *(begin + 1), predicate);
	}
}

// 插入排序（默认升序排序）
template<typename Iterator> inline void InsertSort(const Iterator begin, const Iterator end)
{
	InsertSort(begin, end, less<>());
}

// 希尔排序的插入排序部分
template<typename Iterator, typename Predicate> void _InsertSortForShellSort(const Iterator begin, const Iterator end, Predicate predicate, size_t step = 1)
{
	if (step > 1)
	{
		auto size = end - begin;

		for (size_t s = 0; s < step; s++)
		{
			for (size_t i = step + s; i < size; i += step)
			{
				for (size_t j = s; j >= step; j -= step)
				{
					if (!_CompareSwap(*(begin + j - step), *(begin + j), predicate))
					{
						break;
					}
				}
			}
		}
	}
	else
	{
		InsertSort(begin, end, predicate);
	}
}

// 希尔排序
template<typename Iterator, typename Predicate> void ShellSort(const Iterator begin, const Iterator end, Predicate predicate)
{
	// 原理：
	// 与直接插入排序不同，希尔排序按照某种步长对序列分组，对每一组分别进行插入排序，
	// 例如步长为 3 时，序列被分为 3 组：
	// a[0], a[3], a[6], ...
	// a[1], a[4], a[7], ...
	// a[2], a[5], a[8], ...
	// 所以分别对上述 a[3i], a[3i+1], a[3i+2] 进行插入排序。
	//
	// 希尔排序的每一轮排序都是上面这样的插入排序，
	// 第一轮排序步长为 N/2，此后每一轮步长减半，直到最后一轮步长为 1，等同于直接插入排序。

	auto size = end - begin;

	if (size > 3)
	{
		for (size_t step = size / 2; step > 0; step /= 2)
		{
			_InsertSortForShellSort(begin, end, predicate, step);
		}
	}
	else if (size > 2)
	{
		_InsertSortForShellSort(begin, end, predicate);
	}
	else if (size > 1)
	{
		_CompareSwap(*begin, *(begin + 1), predicate);
	}
}

// 希尔排序（默认升序排序）
template<typename Iterator> inline void ShellSort(const Iterator begin, const Iterator end)
{
	ShellSort(begin, end, less<>());
}

static const size_t _ShellSortThreshold = 16; // 快速排序区间小于此阈值时，切换到希尔排序。

// 内省排序的插入排序部分
template<typename Iterator, typename Predicate> void _QuickSortForIntroSort(const Iterator begin, const Iterator end, Predicate predicate, int depth)
{
	auto size = end - begin;

	if (size > _ShellSortThreshold)
	{
		if (depth > 0)
		{
			using Value = remove_reference_t<decltype(*begin)>;

			// 这里使用三点取中值，即选取 a[0]、a[-1]、a[N/2] 的中值作为基准值，这样可以尽可能避免选到最大值/最小值，导致这轮排序无作用
			Value& left = *begin, & right = *(end - 1), & mid = *(begin + size / 2);
			Value baseVal;

			if (predicate(left, right))
			{
				if (predicate(mid, left))
				{
					baseVal = left;
				}
				else if (predicate(right, mid))
				{
					baseVal = right;
				}
				else
				{
					baseVal = mid;
				}
			}
			else
			{
				if (predicate(mid, right))
				{
					baseVal = right;
				}
				else if (predicate(left, mid))
				{
					baseVal = left;
				}
				else
				{
					baseVal = mid;
				}
			}

			// 从序列两端分别向中间搜索，通过下面的循环使得所有比基准值小/大的元素分别在基准值的左侧/右侧
			Iterator low = begin, high = end - 1;

			while (low < high)
			{
				// 一旦在右侧找到比基准值小的元素
				while (low < high && predicate(baseVal, *high))
				{
					--high;
				}

				// 把上述元素交换至左侧，并右移左游标
				if (low < high)
				{
					swap(*low, *high);
					++low;
				}

				// 一旦在左侧找到比基准值大的元素
				while (low < high && predicate(*low, baseVal))
				{
					++low;
				}

				// 把上述元素交换至右侧，并左移右游标
				if (low < high)
				{
					swap(*low, *high);
					--high;
				}
			} // 最后，左、右游标重合，位于基准值处

			// 对于基准值两侧的元素，递归执行以上操作
			_QuickSortForIntroSort(begin, low, predicate, depth - 1);
			_QuickSortForIntroSort(low + 1, end, predicate, depth - 1);
		}
		else
		{
			// 递归深度超过阈值，切换到堆排序
			HeapSort(begin, end, predicate);
		}
	}
	else if (size > 2)
	{
		// 排序区间小于阈值，切换到希尔排序
		ShellSort(begin, end, predicate);
	}
	else if (size > 1)
	{
		_CompareSwap(*begin, *(begin + 1), predicate);
	}
}

// 内省排序
template<typename Iterator, typename Predicate> void IntroSort(const Iterator begin, const Iterator end, Predicate predicate)
{
	// 原理：
	// 理想情况下，如果快速排序每一轮选取的基准值都恰好是中位数，那么整个快速排序只需递归 Log2(N) 次，
	// 内省排序首先从快速排序开始，当递归深度超过 Log2(N) 时，对未排序完成的部分使用堆排序，
	// 另外，当快速排序的区间小于某个阈值时，对此区间使用希尔排序。

	auto size = end - begin;

	if (size > _ShellSortThreshold)
	{
		int log = 0;
		size_t s = size;

		while (s != 0)
		{
			s >>= 1;
			++log;
		}

		_QuickSortForIntroSort(begin, end, predicate, log);
	}
	else if (size > 2)
	{
		ShellSort(begin, end, predicate);
	}
	else if (size > 1)
	{
		_CompareSwap(*begin, *(begin + 1), predicate);
	}
}

// 内省排序（默认升序排序）
template<typename Iterator> inline void IntroSort(const Iterator begin, const Iterator end)
{
	IntroSort(begin, end, less<>());
}

#pragma endregion

#pragma region 其他原位排序

// 冒泡排序
template<typename Iterator, typename Predicate> void BubbleSort(const Iterator begin, const Iterator end, Predicate predicate)
{
	// 原理：
	// 首先进行第一轮排序，观察序列的第二个元素 a[1]，如果 a[1] < a[0]，那么交换二者，
	// 然后对 a[2]...a[N-1] 重复此操作，这样一轮排序的结果是，整个序列的最大值被置换到了序列尾部，
	//
	// 然后进行第二轮排序，仍然从 a[1] 开始，但直到 a[N-2] 为止，重复以上操作，结果是整个序列的第二大值被置换到序列倒数第二位置，
	//
	// 进行第三轮...排序，最后只剩序列头部的两个元素还未排序，直接比较排序即可。

	auto size = end - begin;

	if (size > 2)
	{
		for (size_t s = size; s > 0; s--)
		{
			for (size_t i = 1; i < s; i++)
			{
				_CompareSwap(*(begin + i - 1), *(begin + i), predicate);
			}
		}
	}
	else if (size > 1)
	{
		_CompareSwap(*begin, *(begin + 1), predicate);
	}
}

// 冒泡排序（默认升序排序）
template<typename Iterator> inline void BubbleSort(const Iterator begin, const Iterator end)
{
	BubbleSort(begin, end, less<>());
}

#pragma endregion