#include "emulator.hpp"
#include <iostream>

constexpr int64_t op_add = 1;
constexpr int64_t op_mul = 2;
constexpr int64_t syscall_read = 3;
constexpr int64_t syscall_write = 4;
constexpr int64_t op_jifft = 5;
constexpr int64_t op_jifff = 6;
constexpr int64_t op_slt = 7;
constexpr int64_t op_seq = 8;
constexpr int64_t syscall_exit = 99;
constexpr size_t MAX_OPCODE = 99;

Emulator::Emulator(const std::vector<int64_t> &mem) {
        loadMemory(mem);
        init();
}

Emulator::Emulator() { init(); }

void Emulator::init() {
        pc = 0;

        il = std::vector<uint8_t>(MAX_OPCODE);
        il.reserve(MAX_OPCODE);
        il.assign(MAX_OPCODE, 0);
        il[op_add - 1] = 4;
        il[op_mul - 1] = 4;
        il[op_jifft - 1] = 3;
        il[op_jifff - 1] = 3;
        il[op_slt - 1] = 4;
        il[op_seq - 1] = 4;
        il[syscall_read - 1] = 2;
        il[syscall_write - 1] = 2;
        il[syscall_exit - 1] = 1;
}

void Emulator::run() {
        std::vector<std::tuple<int64_t, uint8_t>> parsed_instruction(4);
        std::tuple di = std::make_tuple(0, 0);
        bool terminate = false;
        parsed_instruction.reserve(4);
        parsed_instruction.assign(4, di);

        for (; pc < memory.size();) {
                std::fill(parsed_instruction.begin(), parsed_instruction.end(),
                          di);
                parse_instruction(&parsed_instruction);
                execute_instruction(parsed_instruction, &terminate);
                if (terminate)
                        return;
        }
}

void Emulator::parse_instruction(
        std::vector<std::tuple<int64_t, uint8_t>> *parsed_instruction) {
        int64_t word = memory[pc];
        uint8_t opcode = word % 100;
        uint8_t l = il[opcode - 1];
        size_t i = 0;
        word = word / 100;

        (*parsed_instruction)[i++] = std::make_tuple(opcode, l);

        for (; i < l; ++i, word /= 10)
                (*parsed_instruction)[i] =
                        std::make_tuple(memory[pc + i], word % 10);
}

void Emulator::execute_instruction(
        const std::vector<std::tuple<int64_t, uint8_t>> &parsed_instruction,
        bool *terminate) {
        auto [opcode, l] = parsed_instruction[0];

        switch (opcode) {
        case op_add: {
                auto [v0, m0] = parsed_instruction[1];
                if (!m0)
                        v0 = memory[v0];
                auto [v1, m1] = parsed_instruction[2];
                if (!m1)
                        v1 = memory[v1];
                auto [v2, _] = parsed_instruction[3];
                memory[v2] = v0 + v1;
                pc = pc + l;
                break;
        }
        case op_mul: {
                auto [v0, m0] = parsed_instruction[1];
                if (!m0)
                        v0 = memory[v0];
                auto [v1, m1] = parsed_instruction[2];
                if (!m1)
                        v1 = memory[v1];
                auto [v2, _] = parsed_instruction[3];
                memory[v2] = v0 * v1;
                pc = pc + l;
                break;
        }
        case op_jifft: {
                auto [v0, m0] = parsed_instruction[1];
                if (!m0)
                        v0 = memory[v0];
                auto [v1, m1] = parsed_instruction[2];
                if (!m1)
                        v1 = memory[v1];
                if (v0)
                        pc = v1;
                else
                        pc = pc + l;
                break;
        }
        case op_jifff: {
                auto [v0, m0] = parsed_instruction[1];
                if (!m0)
                        v0 = memory[v0];
                auto [v1, m1] = parsed_instruction[2];
                if (!m1)
                        v1 = memory[v1];
                if (!v0)
                        pc = v1;
                else
                        pc = pc + l;
                break;
        }
        case op_slt: {
                auto [v0, m0] = parsed_instruction[1];
                if (!m0)
                        v0 = memory[v0];
                auto [v1, m1] = parsed_instruction[2];
                if (!m1)
                        v1 = memory[v1];
                auto [v2, _] = parsed_instruction[3];
                memory[v2] = v0 < v1 ? 1 : 0;
                pc = pc + l;
                break;
        }
        case op_seq: {
                auto [v0, m0] = parsed_instruction[1];
                if (!m0)
                        v0 = memory[v0];
                auto [v1, m1] = parsed_instruction[2];
                if (!m1)
                        v1 = memory[v1];
                auto [v2, _] = parsed_instruction[3];
                memory[v2] = v0 == v1 ? 1 : 0;
                pc = pc + l;
                break;
        }
        case syscall_exit: {
                (*terminate) = true;
                pc = pc + l;
                break;
        }
        case syscall_read: {
                auto [rd, _] = parsed_instruction[1];
                std::string input;
                getline(std::cin, input);
                memory[rd] = atoi(input.c_str());
                pc = pc + l;
                break;
        }
        case syscall_write: {
                auto [v0, m0] = parsed_instruction[1];
                if (!m0)
                        v0 = memory[v0];
                std::cout << v0;
                pc = pc + l;
                break;
        }
        }
}

void Emulator::setMemory(size_t location, int64_t value) {
        memory[location] = value;
}

int64_t Emulator::getMemory(size_t location) { return memory[location]; }

void Emulator::loadMemory(const std::vector<int64_t> &mem) {
        for (auto m : mem)
                memory.push_back(m);
}

void Emulator::restart() {
        memory.clear();
        cash.clear();
        pc = 0;
}
