#include "emulator.hpp"
#include <iostream>

const long OPCODE_ADD = 1;
const long OPCODE_MUL = 2;
const long OPCODE_EXIT = 99;

Emulator::Emulator(const std::vector<long> mem) {
        for (auto m : mem)
                memory.push_back(m);
        pc = 0;
}

Emulator::Emulator() {
        pc = 0;
}

void Emulator::setMemory(size_t location, long value) {
        memory[location] = value;
}

long Emulator::getMemory(size_t location) { return memory[location]; }

void Emulator::loadMemory(const std::vector<long> mem) {
        for (auto m : mem)
                memory.push_back(m);
}

void Emulator::restart() {
        memory.clear();
        cash.clear();
        pc = 0;
}

void Emulator::execute() {
        long opcode, rs1, rs2, rd, vrs1, vrs2, result;

        for (; pc < memory.size();) {
                opcode = memory[pc];

                switch (opcode) {
                case OPCODE_ADD: {
                        rs1 = memory[1 + pc];
                        rs2 = memory[2 + pc];
                        rd = memory[3 + pc];
                        vrs1 = memory[rs1];
                        vrs2 = memory[rs2];
                        auto r = cash.find(std::make_tuple(opcode, vrs1, vrs2));
                        if (r != cash.end()) {
                                result = r->second;
                        } else {
                                result = vrs1 + vrs2;
                                cash.insert_or_assign(
                                        std::make_tuple(opcode, vrs1, vrs2),
                                        result);
                        }
                        memory[rd] = result;
                        pc = pc + 4;
                        break;
                }
                case OPCODE_MUL: {
                        rs1 = memory[1 + pc];
                        rs2 = memory[2 + pc];
                        rd = memory[3 + pc];
                        vrs1 = memory[rs1];
                        vrs2 = memory[rs2];
                        auto r = cash.find(std::make_tuple(opcode, vrs1, vrs2));
                        if (r != cash.end()) {
                                result = r->second;
                        } else {
                                result = vrs1 * vrs2;
                                cash.insert_or_assign(
                                        std::make_tuple(opcode, vrs1, vrs2),
                                        result);
                        }
                        memory[rd] = result;
                        pc = pc + 4;
                        break;
                }
                case OPCODE_EXIT: {
                        pc = pc + 1;
                        return;
                        break;
                }
                }
        }
}

std::vector<long> &Emulator::getRawMemory() { return memory; }
