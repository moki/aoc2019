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
constexpr int64_t op_incb = 9;
constexpr int64_t syscall_exit = 99;

constexpr size_t MAX_OPCODE = 99;

constexpr uint8_t relmode = 2;
constexpr uint8_t immmode = 1;
constexpr uint8_t posmode = 0;

Emulator::Emulator(const std::vector<int64_t> &mem) {
        memload(mem);
        init();
}

Emulator::Emulator() { init(); }

void Emulator::init() {
        pc = 0;
        base = 0;

        il = std::vector<uint8_t>(MAX_OPCODE);
        il.reserve(MAX_OPCODE);
        il.assign(MAX_OPCODE, 0);
        il[op_add - 1] = 4;
        il[op_mul - 1] = 4;
        il[op_jifft - 1] = 3;
        il[op_jifff - 1] = 3;
        il[op_slt - 1] = 4;
        il[op_seq - 1] = 4;
        il[op_incb - 1] = 2;
        il[syscall_read - 1] = 2;
        il[syscall_write - 1] = 2;
        il[syscall_exit - 1] = 1;

        reset_buffers();
}

void Emulator::memload(const std::vector<int64_t> &mem) {
        size_t i, l;
        for (l = mem.size(), i = 0; i < l; ++i)
                memory[i] = mem[i];
}

void Emulator::restart() {
        memory.clear();
        reset_buffers();
        pc = 0;
        base = 0;
}

void Emulator::reset_buffers() {
        output.str(std::string());
        input.str(std::string());
        output.clear();
        input.clear();
}

std::stringstream &Emulator::get_stdout() { return output; }

std::stringstream &Emulator::get_stdin() { return input; }

int64_t Emulator::memget(const int64_t a, const uint8_t m) {
        if (m == posmode)
                return memory[a];
        else if (m == immmode)
                return a;
        return memory[a + base];
}

void Emulator::memset(const int64_t a, const uint8_t m, int64_t v) {
        if (m == relmode)
                memory[a + base] = v;
        else
                memory[a] = v;
}

void Emulator::run(bool pause_on_output) {
        std::vector<std::tuple<int64_t, uint8_t>> parsed_instruction(4);
        std::tuple eb = std::make_tuple(0, 0);
        _pause_on_output = pause_on_output;
        bool pause = false;
        parsed_instruction.reserve(4);
        parsed_instruction.assign(4, eb);

        for (; pc < memory.size();) {
                std::fill(parsed_instruction.begin(), parsed_instruction.end(),
                          eb);
                parse_instruction(&parsed_instruction);
                execute_instruction(parsed_instruction, &pause);
                if (pause)
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
        bool *pause) {
        auto [opcode, l] = parsed_instruction[0];

        switch (opcode) {
        case op_add: {
                auto [v0, m0] = parsed_instruction[1];
                auto [v1, m1] = parsed_instruction[2];
                auto [v2, m2] = parsed_instruction[3];
                v0 = memget(v0, m0);
                v1 = memget(v1, m1);
                memset(v2, m2, v0 + v1);
                pc = pc + l;
                break;
        }
        case op_mul: {
                auto [v0, m0] = parsed_instruction[1];
                auto [v1, m1] = parsed_instruction[2];
                auto [v2, m2] = parsed_instruction[3];
                v0 = memget(v0, m0);
                v1 = memget(v1, m1);
                memset(v2, m2, v0 * v1);
                pc = pc + l;
                break;
        }
        case op_jifft: {
                auto [v0, m0] = parsed_instruction[1];
                auto [v1, m1] = parsed_instruction[2];
                v0 = memget(v0, m0);
                v1 = memget(v1, m1);
                if (v0)
                        pc = v1;
                else
                        pc = pc + l;
                break;
        }
        case op_jifff: {
                auto [v0, m0] = parsed_instruction[1];
                auto [v1, m1] = parsed_instruction[2];
                v0 = memget(v0, m0);
                v1 = memget(v1, m1);
                if (!v0)
                        pc = v1;
                else
                        pc = pc + l;
                break;
        }
        case op_slt: {
                auto [v0, m0] = parsed_instruction[1];
                auto [v1, m1] = parsed_instruction[2];
                auto [v2, m2] = parsed_instruction[3];
                v0 = memget(v0, m0);
                v1 = memget(v1, m1);
                memset(v2, m2, v0 < v1 ? 1 : 0);
                pc = pc + l;
                break;
        }
        case op_seq: {
                auto [v0, m0] = parsed_instruction[1];
                auto [v1, m1] = parsed_instruction[2];
                auto [v2, m2] = parsed_instruction[3];
                v0 = memget(v0, m0);
                v1 = memget(v1, m1);
                memset(v2, m2, v0 == v1 ? 1 : 0);
                pc = pc + l;
                break;
        }
        case op_incb: {
                auto [v0, m0] = parsed_instruction[1];
                v0 = memget(v0, m0);
                base += v0;
                pc = pc + l;
                break;
        }
        case syscall_exit: {
                pc = memory.size();
                break;
        }
        case syscall_read: {
                auto [rd, rm] = parsed_instruction[1];
                int64_t _input;
                input >> _input;
                memset(rd, rm, _input);
                pc = pc + l;
                break;
        }
        case syscall_write: {
                auto [v0, m0] = parsed_instruction[1];
                v0 = memget(v0, m0);
                output << v0;
                pc = pc + l;
                if (_pause_on_output)
                        (*pause) = true;
                break;
        }
        }
}
