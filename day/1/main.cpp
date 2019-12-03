#include <iostream>
#include <math.h>
#include <string>

int64_t calculateFuel(int64_t mass) { return floor(mass / 3) - 2; }

int main(int argc, char **argv) {
        std::string line;
        int64_t sum = 0;
        for (; std::getline(std::cin, line);) {
                int64_t intermidiate;
                intermidiate = calculateFuel(atoi(line.c_str()));
                do {
                        sum += intermidiate;
                        intermidiate = calculateFuel(intermidiate);
                } while (intermidiate > 0);
        }
        std::cout << "elfs need " << sum << " amounts of fuel \n";
        return 0;
}
