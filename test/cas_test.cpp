#include "cas_queue.hpp"

std::vector<std::thread> threadVector;

int main()
{
    CasQueue<char> casQueue;
    for (int i = 0; i < 16; i++)
    {
        threadVector.push_back(std::thread([i, &casQueue]
                                           {for(int j=0;j<10000;j++) casQueue.enqueue(i+'a'); }));
    }

    for (size_t i = 0; i < threadVector.size(); i++)
    {
        threadVector[i].join();
    }

    for (size_t i = 0; i < 16 * 10000 + 10; i++)
    {
        char ans = '!';
        bool success = casQueue.dequeue(ans);
        std::cout << (success ? "success" : "false");
        std::cout << ans << std::endl;
    }
}