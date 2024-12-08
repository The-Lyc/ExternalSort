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
public:
    ThreadPoolManager(int num, std::string fileSrc, std::string fileDest) : threadPool(num), fileSrc(fileSrc), fileDest(fileDest) {blockSize=TotalMemory/num;}
    ~ThreadPoolManager(){};

    void split();
    void merge();
    void stop();
};
