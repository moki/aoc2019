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
        bool pause_on_output = true;
        do {
                int64_t r = 0;
                for (auto s : settings) {
                        emu.restart();
                        emu.loadMemory(code);
                        std::stringstream &in = emu.get_stdin();
                        std::stringstream &ou = emu.get_stdout();
                        in << s << " " << r;
                        emu.run(pause_on_output);
                        ou >> r;
                }
                if (r > thrusters_max)
                        thrusters_max = r;
        } while (std::next_permutation(settings.begin(), settings.end()));

        std::cout << "part 1: " << thrusters_max << "\n";

        /* part 2 */
        std::array<int, 5> _settings{5, 6, 7, 8, 9};
        thrusters_max = INT_MIN;

        do {
                std::array<Emulator, 5> amps{};
                size_t i = 0;
                int64_t r = 0;

                for (auto s : _settings) {
                        amps[i] = Emulator(code);
                        std::stringstream &in = amps[i].get_stdin();
                        std::stringstream &ou = amps[i].get_stdout();
                        in << s << " " << r;
                        amps[i].run(pause_on_output);
                        ou >> r;
                        amps[i].reset_buffers();
                        i++;
                }

                for (i = 0; true; i = (i + 1) % 5) {
                        std::stringstream &in = amps[i].get_stdin();
                        std::stringstream &ou = amps[i].get_stdout();
                        in << r << " ";
                        amps[i].run(pause_on_output);
                        if (!ou.tellp()) {
                                break;
                        }
                        ou >> r;
                        amps[i].reset_buffers();
                }

                if (r > thrusters_max)
                        thrusters_max = r;
        } while (std::next_permutation(_settings.begin(), _settings.end()));

        std::cout << "part 2: " << thrusters_max << "\n";

        return 0;
}
