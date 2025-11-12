#ifndef LIMITORDERBOOK_QUANTITY_H
#define LIMITORDERBOOK_QUANTITY_H
#include <cassert>

class Quantity {
public:
   Quantity() = delete;

   explicit Quantity(const int quantity) : quantity_(quantity) {
      assert(quantity > 0 && "Quantity <= 0");
   };

   [[nodiscard]]
   double get_quantity() const {
      return quantity_;
   }

private:
   int quantity_;
};

#endif //LIMITORDERBOOK_QUANTITY_H