#pragma once

#include "hash.hpp"
#include <tuple>
#include <unordered_set>
#include <unordered_map>
#include <cstdint>

#include <iostream>

[[maybe_unused]] constexpr uint8_t direction_up    = 0b000;
[[maybe_unused]] constexpr uint8_t direction_right = 0b001;
[[maybe_unused]] constexpr uint8_t direction_down  = 0b010;
[[maybe_unused]] constexpr uint8_t direction_left  = 0b011;
[[maybe_unused]] constexpr uint8_t directions      = 0b0100;

[[maybe_unused]] constexpr uint8_t plate_color_white = 0b0001;
[[maybe_unused]] constexpr uint8_t plate_color_black = 0b0000;

[[maybe_unused]] constexpr uint8_t plate_state_prestine = 0b0000;
[[maybe_unused]] constexpr uint8_t plate_state_dirty = 0b0010;

[[maybe_unused]] auto fu = [](uint8_t v) { return (unsigned)v; };

class Robot {
        public:
                Robot(std::unordered_map<std::tuple<int64_t, int64_t>, uint8_t,
                                         tuple_op_hash, tuple_op_eq> &surface,
                      int64_t x, int64_t y, uint8_t direction, bool start_on_white);
                Robot(std::unordered_map<std::tuple<int64_t, int64_t>, uint8_t,
                                         tuple_op_hash, tuple_op_eq> &surface);
                void set_location(int64_t x, int64_t y);
                void set_direction(uint8_t direction);
                void set_surface(const std::unordered_map<std::tuple<int64_t, int64_t>,
                                                          uint8_t, tuple_op_hash, tuple_op_eq>& surface);
                uint8_t read_surface_color();
                void turn(uint8_t direction);
                void write_surface_color(uint8_t color);
                void move_forward();
                uint64_t painted();
        private:
                std::unordered_map<std::tuple<int64_t, int64_t>, uint8_t,
                                   tuple_op_hash, tuple_op_eq> &_surface;
                std::unordered_set<std::tuple<int64_t, int64_t>, tuple_op_hash> painted_plates;
                int64_t _x;
                int64_t _y;
                uint8_t _direction;
                bool _start_on_white;

};
