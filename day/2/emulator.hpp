#include <vector>

class Emulator {
        public:
                Emulator();
                void add(long value);
                void setCode(size_t location, long value);
                long getCode(size_t location);
                void execute();
                void restart();
                std::vector<long> &getCode();
        private:
                size_t pc;
                short il;
                std::vector<long> code;
};
