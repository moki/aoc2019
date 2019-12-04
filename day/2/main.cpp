#include "emulator.hpp"
#include <iostream>
#include <string>

const char SEPARATOR = ',';

int main(int argc, char **argv) {
        Emulator emu;
        std::string line;
        std::vector<long> code;

        for (; std::getline(std::cin, line, SEPARATOR);)
                code.push_back(atoi(line.c_str()));
        for (auto c : code)
                emu.add(c);

        /* part 1 */
        /* restore 1202 program alarm state */
        emu.setCode(1, 12);
        emu.setCode(2, 2);
        emu.execute();
        std::cout << emu.getCode(0) << "\n";

        emu.restart();

        /* part 2 */
        size_t i, j;
        std::vector<size_t> pair;
        for (i = 0; i < 100; i++) {
                for (j = 0; j < 100; j++) {
                        emu.restart();
                        for (auto c : code)
                                emu.add(c);
                        emu.setCode(1, i);
                        emu.setCode(2, j);
                        emu.execute();

                        if (emu.getCode(0) == 19690720) {
                                pair.push_back(emu.getCode(1));
                                pair.push_back(emu.getCode(2));
                                goto found;
                        }
                }
        }

        return 1;
found:
        std::cout << 100 * pair[0] + pair[1] << "\n";
        return 0;
}
