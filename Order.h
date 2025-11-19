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
   explicit Order(Price price, Quantity quantity, Uuid order_id, Side side) : price_(price), quantity_(quantity),
                                                                              order_id_(order_id), side_(side) {
   }

   std::string string() const {
      return std::format("{} £{} #{} {:x}",
                         (side_ == ASK ? "ASK" : "BID"),
                         price_.string(),
                         quantity_.quantity(),
                         order_id_);
   }

   Quantity fill(Quantity &fill_by) {
      assert(fill_by <= quantity() && "Tried to overfill quantity");

      quantity_ = quantity() - fill_by;
      return quantity_;
   }

   [[nodiscard]] Price price() const {
      return price_;
   }

   [[nodiscard]] Quantity quantity() const {
      return quantity_;
   }

   [[nodiscard]] Uuid order_id() const {
      return order_id_;
   }

   [[nodiscard]] Side side() const {
      return side_;
   }

private:
   Price price_;
   Quantity quantity_;
   Uuid order_id_;
   Side side_;
};

#endif //LIMITORDERBOOK_ORDER_H
