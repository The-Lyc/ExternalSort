#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <condition_variable>
#include <atomic>
#include <future>

#include "sort_option.hpp"

#define SPLIT_TASK 1
#define MERGE_TASK 2
#define COMMAND_TASK 3

typedef struct Task
{
    int8_t taskType;
    std::string fileSrc;
    std::size_t start;
    std::size_t offset;

    std::string fileAnother;
    std::string fileDest;

    std::string command;
} Task;