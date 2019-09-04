#pragma once
#include <vector>
#include <iostream>


struct ListManager {
    std::vector<unsigned int> list;
    const unsigned int maxElement = 100000;
    unsigned int listSize;
    ListManager(const unsigned int ls ) {
        listSize = ls;
        for (int i = 0; i < listSize; i++) {
            list.push_back(rand()%maxElement);
        }
    }
};

void printIntVector(std::vector<int> list) {
    std::cout << "[";
    for (int i = 0; i < list.size(); i++) {
        if (i + 1 != list.size()) {
            std::cout << list[i] << ", ";
        } else {
            std::cout << list[i];
        }
    }
    std::cout << "]" << std::endl;
};

bool sorted(const std::vector<unsigned int> &list) {
    for(int i = 0; i < list.size() - 1; i++) {
        if (list[i] > list[i + 1]) {
            return false;
        }
    }
    return true;
}

struct Sorter {
    int j;
    void sortStep(std::vector<unsigned int> &list)
    {
        for (int i = 0; i < list.size(); i++) {
            if (list[i + 1] < list[i]) {
                if (i != list.size() - 1) {
                    int t = list[i];
                    list[i] = list[i + 1];
                    list[i + 1] = t;
                }
            }
        }
        j++;
        if( j == list.size()) {
            j = 0;
        }
    }
};