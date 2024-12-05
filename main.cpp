// n个线程，先计算64MB在单个文件排序时分给每个线程的内存 m = 64/n MB
// 再计算文件能被分成多少个小块 u = S / m，上取整。
// 计算 mu = u/n，那么每个线程的读取范围就可以计算。

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

std::queue<std::string> fileQueue; 
std::mutex fileQueueMutex;

void SplitFileToBlock(std::string fileName, int numThreads)
{
    int memoryOfEachThread = TotalMemory / numThreads;
    //TODO:submit read task to task queue of thread pool
}

void AtomicPushToQueue(std::string path)
{
    std::lock_guard<std::mutex> lock(fileQueueMutex);
    fileQueue.push(path);
}

void AtomicPopFromQueue()
{
    std::lock_guard<std::mutex> lock(fileQueueMutex);
    fileQueue.pop();
}

std::string AtomicFrontFromQueue()
{
    std::lock_guard<std::mutex> lock(fileQueueMutex);
    return fileQueue.front();
}

int AtomicSizeFromQueue()
{
    std::lock_guard<std::mutex> lock(fileQueueMutex);
    return fileQueue.size();
}

void ReadFileBlock(std::string& fileName, size_t start, size_t offset)
{
    std::ifstream inFile(fileName, std::ios::binary);
    std::priority_queue<int64_t,std::vector<int64_t>> pq;

    inFile.seekg(start);

    std::vector<char> buffer(offset);

    inFile.read(buffer.data(),buffer.size());

    // 打开存储每个小block的文件
    std::string currentFileName = std::to_string(start) + ".txt";
    std::ofstream blockFile(currentFileName);

    // 将读取到的字节按 64 位整数解释
    for (size_t i = 0; i < buffer.size(); i += 8) {
        int64_t value;
        // 将 8 个字节转换为 int64_t
        std::memcpy(&value, &buffer[i], 8);
        // 先暂存在pq中排序，这一步计入使用内存
        pq.push(value);
    }

    // 有序写入block文件
    while (!pq.empty())
    {
        int64_t ans = pq.top();
        pq.pop();
        // 必须按字节写入，如果按文本写入，则无法控制写入文件最终的大小，不能满足内存的限制
        blockFile.write(reinterpret_cast<const char*>(&ans), sizeof(ans));
        blockFile.flush();
    }
    //把排好序的blockFile记录起来
    AtomicPushToQueue(currentFileName);
}

void TwoWayMerge(std::string fileA, std::string fileB, std::string fileOutput)
{
    std::ifstream infileA(fileA, std::ios::binary);
    std::ifstream infileB(fileB, std::ios::binary);
    std::ofstream outFile(fileOutput, std::ios::binary);

    if(!infileA.is_open() || !infileB.is_open()) {
        perror("ifstream, TwoWayMerge");
    }

    // 维护两个队列作为infileA和infileB的缓冲，这里需要计入内存使用
    std::queue<int64_t> queueA;
    std::queue<int64_t> queueB;
    // A B 各自的读指针偏移
    size_t offsetA = 0;
    size_t offsetB = 0;
    // 获取A B各自的文件字节数，即读指针各自的终点
    infileA.seekg(0, std::ios::end);
    infileB.seekg(0, std::ios::end);
    size_t FileSizeA = infileA.tellg();
    size_t FileSizeB = infileB.tellg();
    infileA.seekg(0, std::ios::beg);
    infileB.seekg(0, std::ios::beg);

    //TODO:把每个缓冲区大小调整为每个线程能分配到的最大内存(要是8的倍数，不然数字会被截断)
    size_t maxSize = 200;
    while(offsetA<FileSizeA || offsetB<FileSizeB) {
        // 如果A或B有一个读完，那直接把另一个(缓冲区和文件剩余的)全部读到out里面。这里要计入内存使用
        if(offsetA >= FileSizeA) {
            while(!queueB.empty()) {
                int64_t value = queueB.front();
                queueB.pop();
                outFile.write(reinterpret_cast<char*>(&value),sizeof(value));
                offsetB += 8;
            }
            while(offsetB < FileSizeB) {
                int64_t value;
                infileB.read(reinterpret_cast<char*>(&value),sizeof(value));
                outFile.write(reinterpret_cast<char*>(&value),sizeof(value));
                offsetB += 8;
            }
            return;
        }
        if(offsetB >= FileSizeB) {
            while(!queueA.empty()) {
                int64_t value = queueA.front();
                queueA.pop();
                outFile.write(reinterpret_cast<char*>(&value),sizeof(value));
                offsetA += 8;
            }
            while(offsetA < FileSizeA) {
                int64_t value;
                infileA.read(reinterpret_cast<char*>(&value),sizeof(value));
                outFile.write(reinterpret_cast<char*>(&value),sizeof(value));
                offsetA += 8;
            }
            return;
        }

        if(queueA.empty()) {
            size_t readSize = std::min(maxSize, FileSizeA-offsetA); 
            while(readSize) {
                int64_t value;
                infileA.read(reinterpret_cast<char*>(&value),sizeof(value));
                queueA.push(value);
                readSize-=8;
            }
        }
        if(queueB.empty()) {
            size_t readSize = std::min(maxSize, FileSizeB-offsetB); 
            while(readSize) {
                int64_t value;
                infileB.read(reinterpret_cast<char*>(&value),sizeof(value));
                queueB.push(value);
                readSize-=8;
            }
        }
        int64_t topOfQueueA = queueA.front();
        int64_t topOfQueueB = queueB.front();
        if(topOfQueueA <= topOfQueueB) {
            outFile.write(reinterpret_cast<char*>(&topOfQueueB), sizeof(topOfQueueB));
            queueB.pop();
            offsetB += 8;
        }
        else {
            outFile.write(reinterpret_cast<char*>(&topOfQueueA), sizeof(topOfQueueA));
            queueA.pop();
            offsetA += 8;
        }
    }
}

int64_t GenerateRandomInt64() {
    std::random_device rd;                    // 获取硬件随机数
    std::mt19937_64 gen(rd());                 // 使用 64 位的 Mersenne Twister 算法
    std::uniform_int_distribution<int64_t> dis; // 默认范围是 int64_t 的最小值到最大值
    
    return dis(gen);  // 生成一个随机的 int64_t 数字
}

int main()
{
    std::string outPath = "test.txt";

    std::ofstream outFile(outPath,std::ios::app);

    for (int64_t i = 0; i < 10; i++)
    {
        int cnt = 100;
        while(cnt--) {
            // 按字节写入int64_t的数据
            std::srand(std::time(nullptr));
            int64_t randomNum = GenerateRandomInt64();
            outFile.write(reinterpret_cast<const char*>(&randomNum), sizeof(randomNum));
        }
    }

    outFile << std::flush;

    for (int i = 0; i < 10; i++)
    {
        ReadFileBlock(outPath,i*800,800);
    }

    std::string fileA = "0.txt";
    std::string fileB = "800.txt";
    std::string fileC = "output.txt";
    TwoWayMerge(fileA, fileB, fileC);

    // std::ifstream outFile(fileC, std::ios::binary);
    // outFile.seekg(0, std::ios::end);
    // size_t sizeOfOutFile = outFile.tellg();
    // outFile.seekg(0, std::ios::beg);
    // while (outFile.tellg() < sizeOfOutFile)
    // {
    //     int64_t value;
    //     outFile.read(reinterpret_cast<char*>(&value), sizeof(value));
    //     std::cout << value << std::endl;
    // }
    
    return 0;
}
