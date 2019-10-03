#ifndef TYPES
#define TYPES

#include <array>
#include <vector>

using byte_t = uint8_t;
using block = std::vector<byte_t>;

template <size_t N>
using bytes = std::array<byte_t, N>;

// A bucket contains a number of Blocks
constexpr int Z = 4;

#endif
