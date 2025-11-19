#ifndef LIMITORDERBOOK_QUANTITY_H
#define LIMITORDERBOOK_QUANTITY_H

#include <cassert>
#include <compare>
#include <cstdint>

class Quantity {
public:
   explicit Quantity(int64_t quantity) : quantity_(quantity) {
      assert(quantity_ >= 0 && "Quantity should be >= 0");
   };

   [[nodiscard]] int64_t quantity() const { return quantity_; }

   bool operator==(const int64_t rhs) const {
      return quantity_ == rhs;
   }

   Quantity operator-(const Quantity other) const {
      return Quantity(quantity_ - other.quantity_);
   }

   auto operator<=>(const Quantity &) const = default;

private:
   int64_t quantity_;
};

#endif //LIMITORDERBOOK_QUANTITY_H
