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
}Task;


class BlockingQueue {
public:
    void push(Task task) {
        std::unique_lock<std::mutex> lock(taskQueueMutex);
        taskQueue.push(task);
        condVar.notify_one();
    }

    Task pop() {
        std::unique_lock<std::mutex> lock(taskQueueMutex);
        condVar.wait(lock, [this]() {return !taskQueue.empty();});
        Task task = taskQueue.front();
        taskQueue.pop();

        return task;
    }

    bool empty() {
        std::lock_guard<std::mutex> lock(taskQueueMutex);
        return taskQueue.empty();
    }

private:
    std::queue<Task> taskQueue;
    std::mutex taskQueueMutex;
    std::condition_variable condVar;
};


class ThreadPool
{
private:
    BlockingQueue blockingQueue;
    std::vector<std::thread> executors;
    std::atomic<bool> stop = false;
    std::mutex coutMutex;
    SortOption sortOption;

    std::queue<std::string> fileQueue;
    std::mutex fileQueueMutex;

    void spin() {
        while (true)
        {
            Task task;
            try {
                task = blockingQueue.pop();
            } catch (const std::exception& e) {
                        std::cerr << "Queue error: " << e.what() << std::endl;
                        break;
            }
            if (task.taskType == COMMAND_TASK || task.command == "STOP") {
                 // 特殊任务信号
                std::lock_guard<std::mutex> lock(coutMutex);
                std::cout << "------------------------------------" << std::this_thread::get_id() << std::endl;
                break; // 线程退出
            } else if (task.taskType == SPLIT_TASK)
            {
                std::string blockFile = sortOption.ReadFileBlock(task.fileSrc, task.start, task.offset);
                std::lock_guard<std::mutex> lock(fileQueueMutex);
                fileQueue.emplace(blockFile);
            } else if (task.taskType == MERGE_TASK)
            {
                std::string mergeFile = sortOption.TwoWayMerge(task.fileSrc, task.fileAnother , task.fileDest);
                std::lock_guard<std::mutex> lock(fileQueueMutex);
                fileQueue.emplace(task.fileDest);
            }   
        }
    }

    
public:
    ThreadPool(int num);
    ~ThreadPool(){};

    void enqueue(Task task);

    // 检查fileQueue里是不是还剩2个文件，是的话就返回文件的总大小给manager
    size_t checkFrontTwoFile(std::string &, std::string &);

    bool getFrontTwoFile(std::string &, std::string &);

    void stopPool();

    void joinPool();
};

