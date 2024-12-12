#include "thread_pool.hpp"

void ThreadPool::enqueue(Task task)
{
    blockingQueue.push(task);
}

void ThreadPool::enqueue_cas(Task task)
{
    casTaskQueue.enqueue(task);
}

size_t ThreadPool::checkFrontTwoFile(std::string &fileFirst, std::string &fileSecond)
{
    if (!casMode)
    {
        {
            std::lock_guard<std::mutex> lock(fileQueueMutex);
            if (fileQueue.size() < 2)
            {
                // std::cout << "check front two file : less than 2" << std::endl;
                return 0;
            }
        }
        fileFirst = fileQueue.front();
        fileQueue.pop();
        fileSecond = fileQueue.front();
        fileQueue.pop();
    }
    else
    {
        if (casFileQueue.checkSize(2))
        {
            if (!casFileQueue.dequeue(fileFirst))
            {
                printf("impossible, cant dequeue from queue(size > 2)");
                return 0;
            }
            if (!casFileQueue.dequeue(fileSecond))
            {
                casFileQueue.enqueue(fileFirst);
                printf("impossible, cant dequeue from queue(size > 2)");
                return 0;
            }
            std::cout << "check front two file from cas : " << fileFirst << "-----" << fileSecond << std::endl;
        }
        else
        {
            return 0;
        }
    }

    std::cout << "check front two file : " << fileFirst << "-----" << fileSecond << std::endl;

    std::ifstream infileFirst(fileFirst);
    std::ifstream infileSecond(fileSecond);

    infileFirst.seekg(0, std::ios::end);
    infileSecond.seekg(0, std::ios::end);

    return infileFirst.tellg() + infileSecond.tellg();
}

bool ThreadPool::getFrontTwoFile(std::string &fileFirst, std::string &fileSecond)
{
    std::lock_guard<std::mutex> lock(fileQueueMutex);
    if (fileQueue.size() < 2)
    {
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

void ThreadPool::stopPool()
{
    if (!stop.exchange(true))
    {
        size_t size = executors.size();
        Task stopTask;
        stopTask.taskType = COMMAND_TASK;
        stopTask.command = "STOP";
        for (size_t i = 0; i < size; i++)
        {
            if (!casMode)
                blockingQueue.push(stopTask);
            else
                casTaskQueue.enqueue(stopTask);
        }
        // 等待所有线程退出
        for (auto &executor : executors)
        {
            if (executor.joinable())
            {
                executor.join();
            }
        }
    }
}

void ThreadPool::joinPool()
{
    Task task;
    // if (!casMode)
    // {
    //     while (!blockingQueue.empty())
    //         ;
    // }
    // else
    // {
    //     while (!casTaskQueue.dequeue(task))
    //         ;
    // }

    if (!stop.exchange(true))
    {
        size_t size = executors.size();
        Task stopTask;
        stopTask.taskType = COMMAND_TASK;
        stopTask.command = "STOP";
        for (size_t i = 0; i < size; i++)
        {
            if (!casMode)
                blockingQueue.push(stopTask);
            else
                casTaskQueue.enqueue(stopTask);
        }
        // 等待所有线程退出
        for (auto &executor : executors)
        {
            if (executor.joinable())
            {
                executor.join();
            }
        }
    }
}

ThreadPool::ThreadPool(int num, bool casMode)
{
    this->casMode = casMode;
    if (casMode)
    {
        while (num--)
        {
            executors.push_back(std::thread(&ThreadPool::spin_cas, this));
        }
    }
    else
    {
        while (num--)
        {
            executors.push_back(std::thread(&ThreadPool::spin, this));
        }
    }
}
