#include "emulator.hpp"
#include <algorithm>
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
                code.push_back(atoi(line.c_str()));

        /* part 1 */
        Emulator emu;
        std::array<int, 5> settings{0, 1, 2, 3, 4};
        int64_t thrusters_max = INT_MIN;
        do {
                int64_t r = 0;
                for (auto s : settings) {
                        emu.restart();
                        emu.loadMemory(code);
                        std::stringstream &in = emu.get_stdin();
                        std::stringstream &ou = emu.get_stdout();
                        in << s << " " << r;
                        emu.run();
                        ou >> r;
                }
                if (r > thrusters_max)
                        thrusters_max = r;
        } while (std::next_permutation(settings.begin(), settings.end()));

        std::cout << thrusters_max << "\n";
        return 0;
}
