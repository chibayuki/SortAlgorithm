/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Copyright © 2021 chibayuki@foxmail.com

Sort.Demo
Version 21.3.20.0000

This file is part of Dependency
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <iostream>
#include <vector>

#include "Sort.h"

using namespace std;

template<typename T, size_t N> constexpr size_t ArraySize(T(&)[N])
{
    return N;
}

int main()
{
    int sequenceA[20];
    for (auto& a : sequenceA)
    {
        a = rand() % 100;
    }

    IntroSort(sequenceA, sequenceA + ArraySize(sequenceA));

    cout << "sequenceA: ";
    for (auto a : sequenceA)
    {
        cout << a << " ";
    }
    cout << endl;

    vector<int> sequenceB(20);
    for (auto& b : sequenceB)
    {
        b = rand() % 100;
    }

    IntroSort(sequenceB.begin(), sequenceB.end());

    cout << "sequenceB: ";
    for (auto b : sequenceB)
    {
        cout << b << " ";
    }
    cout << endl;

    system("pause");
}