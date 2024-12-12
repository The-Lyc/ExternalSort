#include "thread_pool_manager.hpp"

void ThreadPoolManager::split()
{
    std::ifstream infile(fileSrc, std::ios::binary);

    if (!infile.is_open())
    {
        std::cerr << "infile is not exist" << std::endl;

        return;
    }

    size_t readPointer = 0;
    infile.seekg(0, std::ios::end);
    size_t fileSize = infile.tellg();
    totalSize = fileSize;
    infile.seekg(0, std::ios::beg);

    while (readPointer < fileSize)
    {
        size_t currentBlockSize = std::min(blockSize, fileSize - readPointer);
        Task task;
        task.taskType = SPLIT_TASK;
        task.fileSrc = fileSrc;
        task.start = readPointer;
        task.offset = currentBlockSize;
        if (casMode)
            threadPool.enqueue_cas(task);
        else
            threadPool.enqueue(task);

        readPointer += blockSize;
    }
}

void ThreadPoolManager::merge()
{
    while (true)
    {
        // 根据情况决定push哪种类型的Merge Task
        std::string fileFirst;
        std::string fileSecond;
        size_t checkValue = 0;
        if (casMode)
            checkValue = threadPool.checkFrontTwoFile(fileFirst, fileSecond);
        else
            checkValue = threadPool.checkFrontTwoFile(fileFirst, fileSecond);
        if (checkValue == 0)
            continue;
        else
        {
            Task task;
            task.taskType = MERGE_TASK;
            task.fileSrc = fileFirst;
            task.fileAnother = fileSecond;
            std::cout << "push merge task : " << task.fileSrc << "|" << task.fileAnother << "|" << checkValue << std::endl;
            if (checkValue < totalSize)
            {
                task.fileDest = "merge" + std::to_string(fileIndex++) + ".txt";
                if (casMode)
                    threadPool.enqueue_cas(task);
                else
                    threadPool.enqueue(task);
            }
            else
            {
                task.fileDest = fileDest;
                if (casMode)
                    threadPool.enqueue_cas(task);
                else
                    threadPool.enqueue(task);
                threadPool.joinPool();
                break;
            }
        }
    }
}

void ThreadPoolManager::stop()
{
    threadPool.stopPool();
}