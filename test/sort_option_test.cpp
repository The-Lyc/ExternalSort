// n个线程，先计算64MB在单个文件排序时分给每个线程的内存 m = 64/n MB
// 再计算文件能被分成多少个小块 u = S / m，上取整。
// 计算 mu = u/n，那么每个线程的读取范围就可以计算。
#include "sort_option.hpp"

int main()
{
    SortOption sortOption;
    std::string outPath = "test.txt";

    std::ofstream outFile(outPath,std::ios::app);

    for (int64_t i = 0; i < 10; i++)
    {
        int cnt = 100;
        while(cnt--) {
            // 按字节写入int64_t的数据
            std::srand(std::time(nullptr));
            int64_t randomNum = sortOption.GenerateRandomInt64();
            outFile.write(reinterpret_cast<const char*>(&randomNum), sizeof(randomNum));
        }
    }

    outFile << std::flush;

    for (int i = 0; i < 10; i++)
    {
        sortOption.ReadFileBlock(outPath,i*800,800);
    }

    std::string fileA = "0.txt";
    std::string fileB = "800.txt";
    std::string fileC = "output.txt";
    sortOption.TwoWayMerge(fileA, fileB, fileC);

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
