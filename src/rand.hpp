#pragma once

#include "byte_vector.hpp"

#include <cstddef>
#include <iterator>

namespace keepasspp {

enum class RandomStrength {
    weak,
    strong
};

class Rand {
public:
    explicit Rand(RandomStrength rs) : strength(rs) {};

    template <typename Container>
    void fill(Container& container) const noexcept;
    ByteVector get(std::size_t count) const noexcept;
private:
    const RandomStrength strength;

    void fillBuffer(void* buffer, std::size_t size) const noexcept;
};

template <typename Container>
void Rand::fill(Container& container) const noexcept {
    this->fillBuffer(std::data(container), std::size(container));
}

}
