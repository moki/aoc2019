#include "emulator.hpp"
#include <iostream>
#include <string>

const char SEPARATOR = ',';

int main() {
        Emulator emu = Emulator();
        std::string line;
        std::vector<long> code;

        for (; std::getline(std::cin, line, SEPARATOR);)
                code.push_back(atoi(line.c_str()));

        /* cash test */
        /*
        emu.loadMemory(code);
        emu.execute();
        exit(0);
        */

        /* part 1 */
        emu.loadMemory(code);
        /* restore 1202 program alarm state */
        emu.setMemory(1, 12);
        emu.setMemory(2, 2);
        emu.execute();
        std::cout << emu.getMemory(0) << "\n";

        /* part 2 */
        size_t i, j;
        std::vector<size_t> pair;
        for (i = 0; i < 100; i++) {
                for (j = 0; j < 100; j++) {
                        emu.restart();
                        emu.loadMemory(code);
                        emu.setMemory(1, i);
                        emu.setMemory(2, j);
                        emu.execute();
                        if (emu.getMemory(0) == 19690720) {
                                pair.push_back(emu.getMemory(1));
                                pair.push_back(emu.getMemory(2));
                                goto found;
                        }
                }
        }

        return 1;
found:
        std::cout << 100 * pair[0] + pair[1] << "\n";
        return 0;
}
