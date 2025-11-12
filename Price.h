#ifndef LIMITORDERBOOK_PRICE_H
#define LIMITORDERBOOK_PRICE_H
#include <cassert>

class Price {
public:
   Price() = delete;

   explicit Price(const double price) : price_(price) {
      assert(price >= 0 && "Price < 0");
   };

   [[nodiscard]]
   double get_price() const {
      return price_;
   }

private:
   double price_;
};

#endif //LIMITORDERBOOK_PRICE_H
