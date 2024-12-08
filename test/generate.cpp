#include "sort_option.hpp"

int main()
{
    std::string filePath = "input.txt";
    std::ofstream file(filePath, std::ios::app);

    SortOption sortOption;
    std::srand(std::time(nullptr));
    int next_checkpoint = 100000;
    for (size_t i = 0; i < 1e7; i++)
    {
        if (i == next_checkpoint) {
            printf("%d%%\n", next_checkpoint/100000);
            next_checkpoint += 100000; // 每次更新下一个检查点
        }
        int64_t randomNum = sortOption.GenerateRandomInt64();
        file.write(reinterpret_cast<const char*>(&randomNum), sizeof(randomNum));
    }
    
    return 0;
}