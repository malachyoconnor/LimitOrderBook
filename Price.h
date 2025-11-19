#ifndef LIMITORDERBOOK_PRICE_H
#define LIMITORDERBOOK_PRICE_H

#include <cstdint>
#include <string>
#include <compare>
#include <format>

class Price {
public:
   explicit Price(int64_t price) : price_(price) {
   }

   [[nodiscard]] int64_t price() const { return price_; }

   std::string string() const {
      return std::format("{}.{}", price() / 100, price() % 100);
   }

   auto operator<=>(const Price &) const = default;

private:
   int64_t price_;
};

#endif //LIMITORDERBOOK_PRICE_H
