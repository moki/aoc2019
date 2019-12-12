#include "emulator.hpp"
#include "hash.hpp"
#include "robot.hpp"
#include <algorithm>
#include <chrono>
#include <climits>
#include <fstream>
#include <iostream>
#include <string>

const char SEPARATOR = ',';

void send_painter(Emulator &emu, Robot &painter) {
        bool block_on_io = true;
        std::stringstream &in = emu.get_stdin();
        std::stringstream &ou = emu.get_stdout();

        uint8_t emu_ou_state = 0;

        for (; !emu.halted();) {
                emu.run(block_on_io);
                if (emu.paused && emu.block_reason == blocked_on_input) {
                        in << (unsigned)painter.read_surface_color();
                        emu.paused = false;
                } else if (emu.paused &&
                           emu.block_reason == blocked_after_output) {
                        unsigned emuou;
                        ou >> emuou;
                        if (!emu_ou_state)
                                painter.write_surface_color(emuou);
                        else {
                                painter.turn(emuou);
                                painter.move_forward();
                        }
                        emu_ou_state = (emu_ou_state + 1) % 2;
                        emu.paused = false;
                }
        }

        auto number_of_plates = painter.painted();

        std::cout << "robot painted: " << number_of_plates << std::endl;
}

int main() {
        std::string line;
        std::vector<int64_t> code;
        std::ifstream input("input");

        for (; std::getline(input, line, SEPARATOR);)
                code.push_back(std::stoll(line.c_str()));

        std::unordered_map<std::tuple<int64_t, int64_t>, uint8_t, tuple_op_hash,
                           tuple_op_eq>
                surface{};

        Emulator emu = Emulator(code);
        Robot painter_start_black = Robot(surface, 0, 0, direction_up, false);

        send_painter(emu, painter_start_black);

        surface.clear();
        emu = Emulator(code);
        Robot painter_start_white(surface);

        send_painter(emu, painter_start_white);

        std::vector<std::vector<char>> canvas(100, std::vector<char>(100, '.'));

        for (auto &panel : surface) {
                auto [x, y] = panel.first;
                auto value = (unsigned)panel.second & 0b0001;
                canvas[x + 50][y + 50] = value ? '#' : '.';
        }

        for (auto xs : canvas) {
                for (auto x : xs)
                        std::cout << x;
                std::cout << std::endl;
        }

        return 0;
}
