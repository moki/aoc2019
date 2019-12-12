#pragma once
#include <tuple>

typedef std::tuple<int64_t, int64_t> tuple_op_t;

struct tuple_op_hash : public std::unary_function<tuple_op_t, std::size_t> {
        std::size_t operator()(const tuple_op_t &k) const {
                return std::get<0>(k) & std::get<1>(k);
        }
};

struct tuple_op_eq : public std::binary_function<tuple_op_t, tuple_op_t, bool> {
        bool operator()(const tuple_op_t &v0, const tuple_op_t &v1) const {
                return (std::get<0>(v0) == std::get<0>(v1) &&
                        std::get<1>(v0) == std::get<1>(v1));
        }
};
