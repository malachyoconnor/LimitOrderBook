#pragma once

#include <cassert>
#include <compare>
#include <cstdint>
#include "typedefs.h"

class Quantity {
public:
   constexpr explicit Quantity(int64_t quantity)
      : quantity_(quantity) {
   }

   auto operator<=>(const Quantity &other) const = default;

   Quantity &fill(const Quantity &other) {
      assert(quantity_ >= other.quantity_);

      quantity_ -= other.quantity_;
      return *this;
   }

   int64_t GetQuantity() const { return quantity_; }

   explicit operator int64_t() const { return quantity_; }

private:
   int64_t quantity_;
};


inline constexpr Quantity ZERO = Quantity(0);
