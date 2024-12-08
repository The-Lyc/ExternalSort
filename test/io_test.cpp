#include <iostream>
#include <string>
#include <fstream>

int main()
{
    std::string file = "../build/bin/output.txt";
    std::ifstream infile(file, std::ios::binary);

    infile.seekg(0, std::ios::end);
    size_t size = infile.tellg();
    infile.seekg(0, std::ios::beg);

    while (infile.tellg() < size)
    {
        int64_t value;
        infile.read(reinterpret_cast<char*>(&value), sizeof(value));
        std::cout << value << std::endl;
    }
    return 0;

}
