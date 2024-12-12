#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <cstring>
#include <iomanip>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <random>
#include <mutex>
#include <algorithm>

#define TotalMemory 65536

class SortOption
{
public:
    SortOption() {};
    ~SortOption() {};

    std::string ReadFileBlock(std::string &fileName, size_t start, size_t offset);
    std::string TwoWayMerge(std::string fileA, std::string fileB, std::string fileOutput);
    int64_t GenerateRandomInt64();
};
