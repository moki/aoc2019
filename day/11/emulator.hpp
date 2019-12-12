#pragma once

#include <unordered_map>
#include <cstdint>
#include <vector>
#include <tuple>
#include <sstream>

[[maybe_unused]] constexpr int64_t op_add = 1;
[[maybe_unused]] constexpr int64_t op_mul = 2;
[[maybe_unused]] constexpr int64_t syscall_read = 3;
[[maybe_unused]] constexpr int64_t syscall_write = 4;
[[maybe_unused]] constexpr int64_t op_jifft = 5;
[[maybe_unused]] constexpr int64_t op_jifff = 6;
[[maybe_unused]] constexpr int64_t op_slt = 7;
[[maybe_unused]] constexpr int64_t op_seq = 8;
[[maybe_unused]] constexpr int64_t op_incb = 9;
[[maybe_unused]] constexpr int64_t syscall_exit = 99;

[[maybe_unused]] constexpr size_t MAX_OPCODE = 99;

[[maybe_unused]] constexpr uint8_t relmode = 2;
[[maybe_unused]] constexpr uint8_t immmode = 1;
[[maybe_unused]] constexpr uint8_t posmode = 0;

[[maybe_unused]] constexpr uint8_t blocked_on_input  = 0;
[[maybe_unused]] constexpr uint8_t blocked_after_output = 1 << 0;

class Emulator {
        public:
                bool paused;
                Emulator(const std::vector<int64_t> &mem);
                Emulator();
                int64_t memget(const int64_t a, const uint8_t m);
                void memset(const int64_t a, const uint8_t m, int64_t v);
                void memload(const std::vector<int64_t> &mem);
                void run(const bool& block_on_io);
                void reset_buffers();
                void restart();
                std::stringstream &get_stdin();
                std::stringstream &get_stdout();
                uint8_t block_reason;
                bool halted();
        private:
                bool _block_on_io;
                std::stringstream input;
                std::stringstream output;
                std::unordered_map<int64_t, int64_t> memory;
                std::vector<uint8_t> il;
                size_t pc;
                size_t base;
                void parse_instruction(
                        std::vector<std::tuple<int64_t, uint8_t>> *parsed_instruction);
                void execute_instruction(
                        const std::vector<std::tuple<int64_t, uint8_t>> &parsed_instruction);
                void init();
};
