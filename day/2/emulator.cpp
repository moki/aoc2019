#include "emulator.hpp"

Emulator::Emulator() {
        pc = 0;
        il = 4;
}

void Emulator::add(long value) {
        code.push_back(value);
}
void Emulator::setCode(size_t location, long value) { code[location] = value; }
long Emulator::getCode(size_t location) { return code[location]; }
void Emulator::restart() {
        code.clear();
        pc = 0;
}
void Emulator::execute() {
        long opcode, rs1, rs2, rd;
        for (; pc < code.size();) {
                opcode = code[pc * il];
                rs1 = code[1 + pc * il];
                rs2 = code[2 + pc * il];
                rd = code[3 + pc * il];

                switch (opcode) {
                case 1:
                        /* add */
                        code[rd] = code[rs1] + code[rs2];
                        break;
                case 2:
                        /* mul */
                        code[rd] = code[rs1] * code[rs2];
                        break;
                case 99:
                        /* exit */
                        return;
                }

                pc++;
        }
}
std::vector<long> &Emulator::getCode() { return code; }
