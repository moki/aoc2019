#include "emulator.hpp"
#include <iostream>

Emulator::Emulator(const std::vector<int64_t> &mem) {
        memload(mem);
        init();
}

Emulator::Emulator() { init(); }

void Emulator::init() {
        pc = 0;
        base = 0;
        paused = false;

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
        paused = false;
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
        memory[a] = v;
}

bool Emulator::halted() { return pc == memory.size(); }

void Emulator::run(const bool &block_on_io) {
        std::vector<std::tuple<int64_t, uint8_t>> parsed_instruction(4);
        std::tuple eb = std::make_tuple(0, 0);
        _block_on_io = block_on_io;
        parsed_instruction.reserve(4);
        parsed_instruction.assign(4, eb);

        for (; pc < memory.size();) {
                if (paused)
                        return;
                std::fill(parsed_instruction.begin(), parsed_instruction.end(),
                          eb);
                parse_instruction(&parsed_instruction);
                execute_instruction(parsed_instruction);
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
        const std::vector<std::tuple<int64_t, uint8_t>> &parsed_instruction) {
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
                if (_block_on_io && !input.rdbuf()->in_avail()) {
                        paused = true;
                        block_reason = blocked_on_input;
                        break;
                }

                auto [rd, rm] = parsed_instruction[1];
                int64_t _input;
                input >> _input;
                memset(rd, rm, _input);
                pc = pc + l;
                input.str(std::string());
                input.clear();
                break;
        }
        case syscall_write: {
                auto [v0, m0] = parsed_instruction[1];
                v0 = memget(v0, m0);
                output.str(std::string());
                output.clear();
                output << v0;
                pc = pc + l;
                if (_block_on_io) {
                        paused = true;
                        block_reason = blocked_after_output;
                }
                break;
        }
        }
}
