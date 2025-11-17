#ifndef LIMITORDERBOOK_QUANTITY_H
#define LIMITORDERBOOK_QUANTITY_H
#include <algorithm>
#include <cassert>

class Quantity {
public:
   Quantity() = delete;

   explicit Quantity(const long quantity) : quantity_(quantity) {
      assert(quantity >= 0 && "Quantity < 0");
   };

   bool operator<=>(const Quantity &bid) const {
      return std::clamp(quantity() - bid.quantity(), -1L, 1L);
   }

   bool operator==(long other) const {
      return quantity() == other;
   }

   Quantity operator-(const Quantity &bid) const {
      return Quantity(quantity() - bid.quantity_);
   }

   Quantity operator-=(const Quantity &bid) {
      quantity_ -= bid.quantity();
      return *this;
   }

   [[nodiscard]]
   long quantity() const {
      assert(quantity_ > 0);
      return quantity_;
   }

private:
   long quantity_;
};

#endif //LIMITORDERBOOK_QUANTITY_H
