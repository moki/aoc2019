#include <iostream>
#include <math.h>
#include <string>
#include <unordered_map>
#include <vector>

typedef std::pair<long, long> xy_t;

struct xy_hash : public std::unary_function<xy_t, std::size_t> {
        std::size_t operator()(const xy_t &k) const {
                return std::get<0>(k) & std::get<1>(k);
        }
};

struct xy_eq : public std::binary_function<xy_t, xy_t, bool> {
        bool operator()(const xy_t &v0, const xy_t &v1) const {
                return (std::get<0>(v0) == std::get<0>(v1) &&
                        std::get<1>(v0) == std::get<1>(v1));
        }
};

const std::string SEPARATOR = ",";

int main() {
        std::string line;
        std::vector<std::string> lines;
        std::vector<std::vector<std::string>> tokens;
        std::unordered_map<std::pair<long, long>, size_t, xy_hash, xy_eq>
                points;
        std::vector<std::pair<long, long>> intersections;

        for (; std::getline(std::cin, line);)
                lines.push_back(line);

        tokens.reserve(lines.size());
        size_t i;
        for (i = 0; i < lines.size(); i++) {
                size_t start = 0, end = 0;
                for (; end != std::string::npos;) {
                        end = lines[i].find(SEPARATOR, start);
                        auto token = lines[i].substr(start,
                                                     (end == std::string::npos)
                                                             ? std::string::npos
                                                             : end - start);
                        tokens[i].push_back(token);
                        start = ((end > (std::string::npos - SEPARATOR.size()))
                                         ? std::string::npos
                                         : end + SEPARATOR.size());
                }
        }
        for (i = 0; i < lines.size(); i++) {
                long x = 0, y = 0;
                for (auto ts : tokens[i]) {
                        long _x = x, _y = y, k, direction;
                        long val = std::atoi(ts.substr(1).c_str());
                        bool vertical = false;

                        switch (ts[0]) {
                        case 'U': {
                                _y = _y + val;
                                direction = 1;
                                vertical = true;
                                break;
                        }
                        case 'R': {
                                _x = _x + val;
                                direction = 1;
                                break;
                        }
                        case 'D': {
                                _y = _y - val;
                                direction = -1;
                                vertical = true;
                                break;
                        }
                        case 'L': {
                                _x = _x - val;
                                direction = -1;
                                break;
                        }
                        default:
                                return 1;
                        }
                        for (k = x; !vertical && k != _x + direction;
                             k = k + direction) {
                                auto lookup = points.find(std::make_pair(k, y));
                                if (lookup != points.end() &&
                                    lookup->second != i &&
                                    !(k == 0 || y == 0)) {
                                        intersections.push_back(
                                                std::make_pair(k, y));
                                } else {
                                        points.insert(std::make_pair(
                                                std::make_pair(k, y), i));
                                }
                        }
                        for (k = y; vertical && k != _y + direction;
                             k = k + direction) {
                                auto lookup = points.find(std::make_pair(x, k));
                                if (lookup != points.end() &&
                                    lookup->second != i &&
                                    !(x == 0 || k == 0)) {
                                        intersections.push_back(
                                                std::make_pair(x, k));
                                } else {
                                        points.insert(std::make_pair(
                                                std::make_pair(x, k), i));
                                }
                        }
                        x = _x;
                        y = _y;
                }
        }

        long min;
        size_t j = 0;
        for (auto ix : intersections) {
                auto [x, y] = ix;
                long distance = abs(x) + abs(y);
                if (!j)
                        min = distance;
                if (distance < min)
                        min = distance;
                j++;
        }
        std::cout << min << "\n";
        return 0;
}
