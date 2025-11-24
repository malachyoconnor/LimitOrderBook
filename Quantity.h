#pragma once

#include <cassert>
#include <compare>
#include <cstdint>
#include "typedefs.h"

class Quantity {

public:
    explicit Quantity(int64_t quantity)
    : quantity_(quantity) {}

    auto operator<=>(const Quantity& other) const = default;

    Quantity& fill(Quantity& other) {
        assert(quantity_ >= other.quantity_);

        quantity_ -= other.quantity_;
        return *this;
    }

private:
    int64_t quantity_;

};


inline const Quantity ZERO = Quantity(0);
