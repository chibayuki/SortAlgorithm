/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Copyright © 2021 chibayuki@foxmail.com

Sort.Demo
Version 21.3.27.0000

This file is part of Sort
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <iostream>
#include <vector>

#include "Sort.h"

using namespace std;

template<typename T, size_t N> constexpr size_t ArraySize(T(&)[N])
{
    return N;
}

// 对数组排序
void Demo1()
{
    int sequence[20];

    for (auto& val : sequence)
    {
        val = rand() % 100;
    }

    IntroSort(sequence, sequence + ArraySize(sequence));

    cout << "array: ";

    for (auto val : sequence)
    {
        cout << val << " ";
    }

    cout << endl;
}

// 对 STL 容器排序
// 一个容器支持排序的条件是：
// （1）迭代器通过重载 operator*() 实现解引用；
// （2）迭代器通过重载 operator++()、operator--() 等实现逐元素移动（前置或后置的自增、自减）；
// （3）迭代器通过重载 operator+()、operator-() 等实现距离计算与按距离移动；
// （4）容器的元素类型是可比较的（基本数据类型），或者可以被自定义排序依据比较，或者重载了 operator<() 等。
// 容器是否支持排序与它的内存布局无关。
void Demo2()
{
    vector<int> sequence(20);

    for (auto& val : sequence)
    {
        val = rand() % 100;
    }

    IntroSort(sequence.begin(), sequence.end());

    cout << "vector: ";

    for (auto val : sequence)
    {
        cout << val << " ";
    }

    cout << endl;
}

int main()
{
    Demo1();
    Demo2();

    system("pause");
}