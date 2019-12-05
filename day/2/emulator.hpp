#include <vector>
#include <unordered_map>
#include <tuple>

typedef std::tuple<long, long, long> op_t;

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
                Emulator(const std::vector<long> mem);
                Emulator();

                void setMemory(size_t location, long value);
                long getMemory(size_t location);
                void loadMemory(const std::vector<long> mem);
                void execute();
                void restart();
                std::vector<long> &getRawMemory();
        private:
                size_t pc;
                std::vector<long> memory;
                std::unordered_map<std::tuple<long, long, long>, long, op_hash, op_eq> cash;
};
