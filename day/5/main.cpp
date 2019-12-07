#include "emulator.hpp"
#include <fstream>
#include <iostream>
#include <string>

const char SEPARATOR = ',';

int main() {
        Emulator emu;
        std::string line;
        std::vector<int64_t> code;
        std::ifstream input("input");

        for (; std::getline(input, line, SEPARATOR);)
                code.push_back(atoi(line.c_str()));

        emu.loadMemory(code);
        emu.run();

        return 0;
}
