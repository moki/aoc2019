#include <unordered_map>
#include <cstdint>
#include <vector>
#include <tuple>
#include <sstream>

class Emulator {
        public:
                Emulator(const std::vector<int64_t> &mem);
                Emulator();
                int64_t memget(const int64_t a, const uint8_t m);
                void memset(const int64_t a, const uint8_t m, int64_t v);
                void memload(const std::vector<int64_t> &mem);
                void run(bool pause_on_output);
                void reset_buffers();
                void restart();
                std::stringstream &get_stdin();
                std::stringstream &get_stdout();
        private:
                bool _pause_on_output;
                std::stringstream input;
                std::stringstream output;
                std::unordered_map<int64_t, int64_t> memory;
                std::vector<uint8_t> il;
                size_t pc;
                size_t base;
                void parse_instruction(
                        std::vector<std::tuple<int64_t, uint8_t>> *parsed_instruction);
                void execute_instruction(
                        const std::vector<std::tuple<int64_t, uint8_t>> &parsed_instruction, bool *pause);
                void init();
};
