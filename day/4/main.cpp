#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool vp(uint64_t guess) {
        uint8_t c, p;
        p = guess % 10;
        std::vector<uint8_t> ds(10, 0);
        ds[p]++;
        for (; guess /= 10; ds[c]++) {
                c = guess % 10;
                if (c > p)
                        return false;
                p = c;
        }

        return any_of(ds.begin(), ds.end(), [](uint8_t d) { return d == 2; });
}

int main() {
        std::ifstream input("input");
        std::string line;
        uint64_t min, max, vg = 0;

        std::getline(input, line, '-');
        min = std::stoll(line);
        std::getline(input, line);
        max = std::stoll(line);

        std::cout << min << "-" << max << std::endl;
        for (; min <= max;)
                if (vp(min++))
                        vg++;

        std::cout << vg << std::endl;

        return 0;
}
