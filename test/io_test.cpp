#include <iostream>
#include <string>
#include <fstream>

int main()
{
    std::string file = "/home/shy/cpp_ws/ExternalSort/build/bin/output.txt";
    std::ifstream infile(file, std::ios::binary);

    infile.seekg(0, std::ios::end);
    size_t size = infile.tellg();
    infile.seekg(0, std::ios::beg);

    int64_t lastValue = -9223372036854775807LL;

    while (infile.tellg() < size)
    {
        int64_t value;
        infile.read(reinterpret_cast<char *>(&value), sizeof(value));
        std::cout << value << std::endl;
        if (value < lastValue)
        {
            std::cout << "failure" << std::endl;
            break;
        }
        lastValue = value;
    }

    std::cout << "verify success" << std::endl;

    return 0;
}
