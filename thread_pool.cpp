#include "thread_pool.hpp"

Threadenqueue(Task task) {
        blockingQueue.push(task);
    }

void stopPool() {
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

void joinPool() {
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

int main()
{
    ThreadPool pool(12);
    for (size_t i = 0; i < 100000; i++)
    {
        if(i%2 == 0) {
            Task splitTask;
            splitTask.taskType = SPLIT_TASK;
            splitTask.fileSrc = std::to_string(i) + ".txt";
            splitTask.start = i;
            splitTask.offset = i;
            pool.enqueue(splitTask);
        } else {
            Task splitTask;
            splitTask.taskType = MERGE_TASK;
            splitTask.fileSrc = std::to_string(i) + ".txt";
            splitTask.fileAnother = std::to_string(i) + ".txt";
            splitTask.fileDest = std::to_string(i) + ".txt";
            pool.enqueue(splitTask);
        }
    }
    pool.stopPool();
    // pool.joinPool();
}
