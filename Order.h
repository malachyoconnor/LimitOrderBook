#ifndef LIMITORDERBOOK_ORDER_H
#define LIMITORDERBOOK_ORDER_H

#include "Price.h"
#include "Quantity.h"
#include "typedefs.h"

enum Side {
   BID,
   ASK
};

class Order {
public:
   Order() = delete;

   Order(Uuid orderId, Timestamp created_ts, Quantity quantity, Price price, Side side) : orderId_(orderId),
      created_ts_(created_ts),
      quantity_(quantity),
      price_(price),
      side_(side) {
   }

   [[nodiscard]] std::string string() const {
      return std::format("{:x} #{} £{}.{}", orderId_, quantity_.quantity(), price_.price() / 100, price_.price() % 100);
   }

   [[nodiscard]] Uuid order_id() const {
      return orderId_;
   }

   [[nodiscard]] Timestamp created_ts() const {
      return created_ts_;
   }

   [[nodiscard]] Quantity quantity() const {
      return quantity_;
   }

   [[nodiscard]] Price price() const {
      return price_;
   }

   [[nodiscard]] Side side() const {
      return side_;
   }

private:
   friend class OrderBook;

   Uuid orderId_;
   Timestamp created_ts_;
   Quantity quantity_;
   Price price_;
   Side side_;
};

#endif //LIMITORDERBOOK_ORDER_H
