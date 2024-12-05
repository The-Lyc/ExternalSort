// n个线程，先计算64MB在单个文件排序时分给每个线程的内存 m = 64/n MB
// 再计算文件能被分成多少个小块 u = S / m，上取整。
// 计算 mu = u/n，那么每个线程的读取范围就可以计算。

#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <cstring>
#include <iomanip>

#define TotalMemory 65536

const int64_t zeroNumber = 3472328296227680304;

void SplitFileToBlock(std::string fileName, int numThreads)
{
    int memoryOfEachThread = TotalMemory / numThreads;
    //TODO:submit read task to task queue of thread pool
}

void ReadFileBlock(std::string& fileName, size_t start, size_t offset)
{
    std::ifstream inFile(fileName, std::ios::binary);

    inFile.seekg(start);

    std::vector<char> buffer(offset);

    inFile.read(buffer.data(),buffer.size());
    
    // 将读取到的字节按 64 位整数解释
    for (size_t i = 0; i < buffer.size(); i += 8) {
        int64_t value;
        // 将 8 个字节转换为 int64_t
        std::memcpy(&value, &buffer[i], 8);
        std::cout << value << std::flush;
    }

    std::cout<<std::endl;
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
            outFile.write(reinterpret_cast<const char*>(&i), sizeof(i));
        }
    }

    outFile << std::flush;

    for (int i = 0; i < 10; i++)
    {
        ReadFileBlock(outPath,i*800,800);
    }
    
    return 0;
}
