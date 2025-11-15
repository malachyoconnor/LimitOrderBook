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

template<OrderType OrderType_>
class Order {
public:
   Order() = delete;

   Order(Uuid uuid, Timestamp created_ts, Quantity quantity, Price price) : uuid_(uuid), created_ts_(created_ts),
                                                                            quantity_(quantity), price_(price) {
   }

   Order(Order &&other) noexcept : uuid_(std::move(other.uuid_)), created_ts_(std::move(other.created_ts_)),
                                   quantity_(std::move(other.quantity_)), price_(std::move(other.price_)) {}

   bool operator<(const Order &other) const {
      // Bids: Higher prices at the top
      if constexpr (order_type_ == BID) {
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

   [[nodiscard]] OrderType orderType() const {
      return order_type_;
   }

   [[nodiscard]] std::string to_string() const {

      return std::format("{} {:%d:%m:%Y} #{} ${}",
                         uuid_util::to_string(uuid_), created_ts_, quantity_.get_quantity(), price_.to_string());
   }

private:
   Uuid uuid_;
   Timestamp created_ts_;
   Quantity quantity_;
   Price price_;

   const OrderType order_type_ = OrderType_;
};

#endif //LIMITORDERBOOK_ORDER_H
