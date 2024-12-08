#include "thread_pool.hpp"

void ThreadPool::enqueue(Task task) {
    blockingQueue.push(task);
}

size_t ThreadPool::checkFrontTwoFile(std::string & fileFirst, std::string & fileSecond) {
    {
        std::lock_guard<std::mutex> lock(fileQueueMutex);
        if(fileQueue.size() < 2)   {
            // std::cout << "check front two file : less than 2" << std::endl;
            return 0;
        }
    }

    fileFirst = fileQueue.front();
    fileQueue.pop();
    fileSecond = fileQueue.front();
    fileQueue.pop();

    std::cout << "check front two file : " << fileFirst << "-----" << fileSecond << std::endl;

    std::ifstream infileFirst(fileFirst);
    std::ifstream infileSecond(fileSecond);

    infileFirst.seekg(0, std::ios::end);
    infileSecond.seekg(0, std::ios::end);

    return infileFirst.tellg() + infileSecond.tellg();    
}

bool ThreadPool::getFrontTwoFile(std::string & fileFirst, std::string & fileSecond) {
    std::lock_guard<std::mutex> lock(fileQueueMutex);
    if(fileQueue.size()<2)  {
        std::cout << "get front two file : null" << std::endl;
        return false;
    }
    fileFirst = fileQueue.front();
    fileQueue.pop();
    fileSecond = fileQueue.front();
    fileQueue.pop();
    std::cout << "get front two file : " << fileFirst << "|" << fileSecond << std::endl;
    return true;
}

void ThreadPool::stopPool() {
    if(!stop.exchange(true)) {
        size_t size = executors.size();
        Task stopTask;
        stopTask.taskType = COMMAND_TASK;
        stopTask.command = "STOP";
        for(size_t i=0;i<size;i++) {
            blockingQueue.push(stopTask);
        }
        // 等待所有线程退出
        for (auto& executor : executors) {
            if (executor.joinable()) {
                executor.join();
            }
        }
    }
}

void ThreadPool::joinPool() {
    while(!blockingQueue.empty());
    if(!stop.exchange(true)) {
        size_t size = executors.size();
        Task stopTask;
        stopTask.taskType = COMMAND_TASK;
        stopTask.command = "STOP";
        for(size_t i=0;i<size;i++) {
            blockingQueue.push(stopTask);
        }
        // 等待所有线程退出
        for (auto& executor : executors) {
            if (executor.joinable()) {
                executor.join();
            }
        }
    }
}


ThreadPool::ThreadPool(int num)
{
    while (num--) {
        executors.push_back(std::thread(&ThreadPool::spin, this));
    }
}


