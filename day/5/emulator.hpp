#include <unordered_map>
#include <cstdint>
#include <vector>
#include <tuple>

typedef std::tuple<int64_t, int64_t, int64_t> op_t;

struct op_hash : public std::unary_function<op_t, std::size_t> {
        std::size_t operator()(const op_t &k) const {
                return std::get<0>(k) & std::get<1>(k) & std::get<2>(k);
        }
};

struct op_eq : public std::binary_function<op_t, op_t, bool> {
        bool operator()(const op_t& v0, const op_t& v1) const {
                return (
                        std::get<0>(v0) == std::get<0>(v1) &&
                        std::get<1>(v0) == std::get<1>(v1) &&
                        std::get<2>(v0) == std::get<2>(v1)
                );
        }
};

class Emulator {
        public:
                Emulator(const std::vector<int64_t> &mem);
                Emulator();

                void setMemory(size_t location, int64_t value);
                int64_t getMemory(size_t location);
                void loadMemory(const std::vector<int64_t> &mem);
                void run();
                void restart();
        private:
                size_t pc;
                std::vector<int64_t> memory;
                std::vector<uint8_t> il;
                std::unordered_map<std::tuple<int64_t, int64_t, int64_t>, int64_t, op_hash, op_eq> cash;
                void parse_instruction(
                        std::vector<std::tuple<int64_t, uint8_t>> *parsed_instruction);
                void execute_instruction(
                        const std::vector<std::tuple<int64_t, uint8_t>> &parsed_instruction, bool *terminate);
                void init();
};
