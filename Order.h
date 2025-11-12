#ifndef LIMITORDERBOOK_ORDER_H
#define LIMITORDERBOOK_ORDER_H
#include <uuid.h>

#include "Price.h"
#include "Quantity.h"
#include "typedefs.h"
#include "uuid_utils.h"

enum OrderType {
   BID,
   ASK
};

template<OrderType OrderType>
class Order {
public:
   Order() = delete;

   Order(Uuid uuid, Timestamp created_ts, Quantity quantity, Price price) : uuid_(uuid), created_ts_(created_ts),
                                                                            quantity_(quantity), price_(price) {
   }

   bool operator<(const Order &other) const {
      // Bids: Higher prices at the top
      if constexpr (OrderType == BID) {
         return this->price() < other.price() ||
                this->created_ts() < other.created_ts() ||
                this->quantity() < other.quantity() ||
                this->uuid() < other.uuid();
      } else {
         // Asks: Lower prices at the top
         return this->price() > other.price() ||
                this->created_ts() < other.created_ts() ||
                this->quantity() < other.quantity() ||
                this->uuid() < other.uuid();
      }
   }

   bool operator==(const Order &other) const {
      return this->uuid() == other.uuid() &&
             this->created_ts() == other.created_ts() &&
             this->quantity() == other.quantity() &&
             this->price() == other.price();
   }

   bool operator>(const Order &other) const {
      return !(*this == other || *this < other);
   }


   [[nodiscard]] Uuid uuid() const {
      return uuid_;
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

   [[nodiscard]] std::string to_string() const {

      return std::format("{} {:%d:%m:%Y} #{} ${:.2f}",
                         uuid_util::to_string(uuid_), created_ts_, quantity_.get_quantity(), price_.get_price());
   }

private:
   Uuid uuid_;
   Timestamp created_ts_;
   Quantity quantity_;
   Price price_;
};

#endif //LIMITORDERBOOK_ORDER_H
