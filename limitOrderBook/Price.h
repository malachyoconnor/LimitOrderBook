#pragma once

#include <compare>
#include "typedefs.h"

class Price {
public:
   explicit Price(int64_t price) : price_(price) {
   }

   int64_t GetPrice() const { return price_; }

   explicit operator int64_t() const { return price_; }

   auto operator<=>(const Price &other) const = default;

private:
   int64_t price_;
};
