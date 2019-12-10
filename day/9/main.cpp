#include "emulator.hpp"
#include <algorithm>
#include <chrono>
#include <climits>
#include <fstream>
#include <iostream>
#include <string>

const char SEPARATOR = ',';

int main() {
        std::string line;
        std::vector<int64_t> code;
        std::ifstream input("input");

        for (; std::getline(input, line, SEPARATOR);)
                code.push_back(std::stoll(line.c_str()));

        bool pause_on_output = false;
        int64_t res;

        Emulator emu = Emulator(code);
        std::stringstream &in = emu.get_stdin();
        std::stringstream &ou = emu.get_stdout();

        in << 1;

        auto start = std::chrono::system_clock::now();

        emu.run(pause_on_output);
        ou >> res;

        std::cout << res << std::endl;

        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        std::time_t start_time = std::chrono::system_clock::to_time_t(start);
        std::cout << "st: " << std::ctime(&start_time)
                  << "et: " << std::ctime(&end_time)
                  << "el: " << elapsed.count() << std::endl;

        emu = Emulator(code);
        in << 2;

        start = std::chrono::system_clock::now();

        emu.run(pause_on_output);
        ou >> res;

        std::cout << res << std::endl;

        end = std::chrono::system_clock::now();
        elapsed = end - start;
        end_time = std::chrono::system_clock::to_time_t(end);
        start_time = std::chrono::system_clock::to_time_t(start);
        std::cout << "st: " << std::ctime(&start_time)
                  << "et: " << std::ctime(&end_time)
                  << "el: " << elapsed.count() << std::endl;

        return 0;
}
