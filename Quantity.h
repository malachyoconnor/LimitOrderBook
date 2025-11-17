#ifndef LIMITORDERBOOK_QUANTITY_H
#define LIMITORDERBOOK_QUANTITY_H
#include <cassert>
#include <cstdint>
#include <stdexcept>

class Quantity {
public:
   Quantity() = delete;

   explicit Quantity(const int64_t quantity) : quantity_(quantity) {
      if (quantity < 0) {
         throw std::logic_error("Received negative quantity");
      }
   };

   auto operator<=>(const Quantity &) const = default;

   auto operator==(const uint64_t& other) const -> bool {
      return quantity() == other;
   }

   Quantity operator-(const Quantity &other) const {
      return Quantity(quantity_ - other.quantity_);
   }

   Quantity operator-=(const Quantity &other) {
      quantity_ -= other.quantity_;
      return *this;
   }

   [[nodiscard]]
   int64_t quantity() const {
      assert(quantity_ >= 0 && "Returning negative quantity");
      return quantity_;
   }

private:
   int64_t quantity_;
};

#endif //LIMITORDERBOOK_QUANTITY_H
