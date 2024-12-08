#include "thread_pool_manager.hpp"

int main()
{
    ThreadPoolManager threadPoolManager(4, "input.txt", "output.txt");
    threadPoolManager.split();
    threadPoolManager.merge();

    return 0;
}