#ifndef LIMITORDERBOOK_PRICE_H
#define LIMITORDERBOOK_PRICE_H
#include <algorithm>
#include <cassert>

class Price {
public:
   Price() = delete;

   explicit Price(const long long price) : price_(price) {
      assert(price >= 0 && "Price < 0");
   };

   [[nodiscard]]
   long long price() const {
      return price_;
   }

   [[nodiscard]]
   std::string to_string() const {
      return std::to_string(price_ / 100) + "." + std::to_string(price_ % 100);
   }

   auto operator<=>(const Price &other) const = default;

private:
   // Stored as the number of 'cents' to prevent double strangeness
   long long price_;
};

static_assert(sizeof(size_t) == sizeof(long long));

struct PriceHasher {
   std::size_t operator()(const Price &price_to_hash) const {
      static constexpr std::hash<long long> hasher{};
      return hasher(price_to_hash.price());
   }
};

#endif //LIMITORDERBOOK_PRICE_H
