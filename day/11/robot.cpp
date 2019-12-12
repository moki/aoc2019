#include "robot.hpp"

Robot::Robot(std::unordered_map<std::tuple<int64_t, int64_t>, uint8_t,
                                tuple_op_hash, tuple_op_eq> &surface,
             int64_t x, int64_t y, uint8_t direction, bool start_on_white)
        : _surface(surface), painted_plates{}, _x(x), _y(y),
          _direction(direction), _start_on_white(start_on_white) {}

Robot::Robot(std::unordered_map<std::tuple<int64_t, int64_t>, uint8_t,
                                tuple_op_hash, tuple_op_eq> &surface)
        : Robot(surface, 0, 0, direction_up, true) {}

void Robot::set_direction(uint8_t direction) { _direction = direction; }

void Robot::set_surface(
        const std::unordered_map<std::tuple<int64_t, int64_t>, uint8_t,
                                 tuple_op_hash, tuple_op_eq> &surface) {
        _surface = surface;
}

void Robot::set_location(int64_t x, int64_t y) {
        _x = x;
        _y = y;
}

uint8_t Robot::read_surface_color() {
        auto [plate, ok] = _surface.insert(
                {std::make_tuple(_x, _y),
                 (_start_on_white ? plate_color_white : plate_color_black) |
                         plate_state_prestine});
        if (ok) {
                _start_on_white = false;
        }
        return plate->second & 0b0001;
}

void Robot::write_surface_color(uint8_t color) {
        auto [plate, inserted] = _surface.insert_or_assign(
                std::make_tuple(_x, _y), (color | plate_state_dirty));
        if (!inserted)
                painted_plates.insert(plate->first);
}

void Robot::turn(uint8_t direction) {
        _direction = (_direction + (direction ? 1 : -1)) & (directions - 1);
}

void Robot::move_forward() {
        if (!(_direction ^ direction_up) | !(_direction ^ direction_down))
                _y += !(_direction ^ direction_up) ? 1 : (-1);
        else
                _x += !(_direction ^ direction_right) ? 1 : (-1);
}

uint64_t Robot::painted() { return painted_plates.size(); }
