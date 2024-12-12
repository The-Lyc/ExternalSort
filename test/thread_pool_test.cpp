#include "thread_pool.hpp"

int main()
{
    ThreadPool pool(12, true);
    for (size_t i = 0; i < 100000; i++)
    {
        if (i % 2 == 0)
        {
            Task splitTask;
            splitTask.taskType = SPLIT_TASK;
            splitTask.fileSrc = std::to_string(i) + ".txt";
            splitTask.start = i;
            splitTask.offset = i;
            pool.enqueue(splitTask);
        }
        else
        {
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
