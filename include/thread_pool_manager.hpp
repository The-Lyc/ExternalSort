#include "thread_pool.hpp"
#include <cstdio>

class ThreadPoolManager
{
private:
    ThreadPool threadPool;
    std::string fileSrc;
    std::string fileDest;
    size_t blockSize;
    size_t totalSize;
    size_t fileIndex = 0;

    bool casMode;

public:
    ThreadPoolManager(int num, bool casMode, std::string fileSrc, std::string fileDest) : threadPool(num, casMode), fileSrc(fileSrc), fileDest(fileDest)
    {
        blockSize = TotalMemory / num;
        casMode = casMode;
    }
    ~ThreadPoolManager() {};

    void split();
    void merge();
    void stop();
};
