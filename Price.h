#ifndef LIMITORDERBOOK_PRICE_H
#define LIMITORDERBOOK_PRICE_H
#include <cassert>
#include <cstdint>

class Price {
public:
   Price() = delete;

   explicit Price(const int64_t price) : price_(price) {
      assert(price >= 0 && "Price < 0");
   };

   auto operator<=>(const Price &) const = default;

   [[nodiscard]]
   int64_t price() const {
      return price_;
   }

private:
   // Stored as a 64 bit int, to protect against issues with `double`
   int64_t price_;
};

struct PriceHasher {
   int64_t operator()(const Price& price) const {
      return price.price();
   }
};

#endif //LIMITORDERBOOK_PRICE_H
