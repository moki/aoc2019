#include "emulator.hpp"
#include "hash.hpp"
#include <algorithm>
#include <array>
#include <chrono>
#include <climits>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

const char SEPARATOR = ',';

[[maybe_unused]] constexpr unsigned empty = 0;
[[maybe_unused]] constexpr unsigned wall = 1;
[[maybe_unused]] constexpr unsigned block = 2;
[[maybe_unused]] constexpr unsigned paddle = 3;
[[maybe_unused]] constexpr unsigned ball = 4;
[[maybe_unused]] constexpr char obs[] = {'+', '|', '#', '=', 'o'};

void world_insert(std::unordered_map<std::tuple<int64_t, int64_t>, uint8_t,
                                     tuple_op_hash, tuple_op_eq> &world,
                  int64_t x, int64_t y, int64_t object) {
        world.insert_or_assign(std::make_tuple(x, y), object);
}

int64_t
provide_next_move(std::unordered_map<std::tuple<int64_t, int64_t>, uint8_t,
                                     tuple_op_hash, tuple_op_eq> &world) {
        int64_t paddle_x;
        int64_t ball_x;
        for (auto &p : world) {
                auto o = p.second;
                if (o == paddle) {
                        auto [x, _] = p.first;
                        paddle_x = x;
                } else if (o == ball) {
                        auto [x, _] = p.first;
                        ball_x = x;
                }
        }
        if (paddle_x == ball_x)
                return 0;
        else if (paddle_x < ball_x)
                return 1;
        else
                return -1;
        exit(1);
}

void game_loop(Emulator &emu,
               std::unordered_map<std::tuple<int64_t, int64_t>, uint8_t,
                                  tuple_op_hash, tuple_op_eq> &world) {
        bool block_on_io = true;
        uint8_t emu_ou_state = 0;
        std::stringstream &in = emu.get_stdin();
        std::stringstream &ou = emu.get_stdout();
        std::array<int64_t, 3> emuou;
        uint64_t score = 0;

        std::vector<std::vector<char>> canvas(24, std::vector<char>(44, '.'));
        bool playing = false;

        for (; !emu.halted();) {
                emu.run(block_on_io);
                if (emu.paused && emu.block_reason == blocked_after_output) {
                        ou >> emuou[emu_ou_state];
                        emu_ou_state = (emu_ou_state + 1) % 3;
                        if (!(emu_ou_state % 3)) {
                                if (emuou[0] == -1 && emuou[1] == 0) {
                                        score = emuou[2];
                                } else {
                                        world_insert(world, emuou[0], emuou[1],
                                                     emuou[2]);
                                }
                                std::fill(emuou.begin(), emuou.end(), 0);
                        }
                        emu.paused = false;
                } else if (emu.paused && emu.block_reason == blocked_on_input) {
                        playing = true;
                        int64_t joystick_cmd = provide_next_move(world);
                        in << joystick_cmd;
                        emu.paused = false;
                }

                if (playing) {
                        std::chrono::milliseconds timespan(16);
                        std::this_thread::sleep_for(timespan);

                        for (auto &p : world) {
                                auto [x, y] = p.first;
                                canvas[y][x] = obs[p.second];
                        }
                        for (auto xs : canvas) {
                                for (auto x : xs) {
                                        std::cout << x;
                                }
                                std::cout << std::endl;
                        }
                }
        }

        std::cout << score << std::endl;
}

int main() {
        std::string line;
        std::vector<int64_t> code;
        std::ifstream input("input");

        for (; std::getline(input, line, SEPARATOR);)
                code.push_back(std::stoll(line.c_str()));

        Emulator emu = Emulator(code);
        /* quarters hack */
        emu.memset(0, 1, 2);

        std::vector<std::vector<char>> canvas(50, std::vector<char>(100, '.'));
        std::unordered_map<std::tuple<int64_t, int64_t>, uint8_t, tuple_op_hash,
                           tuple_op_eq>
                world{};

        game_loop(emu, world);

        /* part 1 */
        /*
        size_t bs = 0;
        for (auto &p : world) {
                if (p.second == block)
                        bs++;
                auto [x, y] = p.first;
                canvas[y][x] = obs[p.second];
        }

        for (auto xs : canvas) {
                for (auto x : xs)
                        std::cout << x;
                std::cout << std::endl;
        }

        std::cout << bs << std::endl;
        */
        return 0;
}
