#ifndef LIMITORDERBOOK_ORDER_H
#define LIMITORDERBOOK_ORDER_H

#include "Price.h"
#include "Quantity.h"
#include "typedefs.h"

enum OrderType {
   BID,
   ASK
};

template<OrderType OrderType_>
class Order;

using BidPointer = std::shared_ptr<Order<BID>>;
using AskPointer = std::shared_ptr<Order<ASK>>;
using BidWeakPointer = std::weak_ptr<Order<BID>>;
using AskWeakPointer = std::weak_ptr<Order<ASK>>;


template<OrderType OrderType_>
class Order {
public:
   Order() = delete;

   Order(Uuid uuid, Timestamp created_ts, Quantity quantity, Price price) : order_id_(uuid), created_ts_(created_ts),
                                                                            quantity_(quantity), price_(price) {
   }

   Order(Order &&other) noexcept : order_id_(other.order_id_), created_ts_(other.created_ts_),
                                   quantity_(other.quantity_), price_(other.price_) {
   }

   Quantity fillOrder(Quantity amountToFill) {
      if (amountToFill > quantity()) {
         throw std::invalid_argument(
            std::format("Quantity exceeds the maximum quantity in order {}", orderId()));
      }

      quantity_ -= amountToFill;

      return quantity_;
   }

   [[nodiscard]] bool isFilled() const {
      return quantity() == 0;
   }

   [[nodiscard]] Uuid orderId() const {
      return order_id_;
   }

   [[nodiscard]] Timestamp createdTs() const {
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
                         order_id_, created_ts_, quantity_.quantity(), price_.to_string());
   }

   bool operator<(const Order &other) const {
      // Bids: Higher prices at the top
      if constexpr (order_type_ == BID) {
         return this->price() < other.price() ||
                this->createdTs() < other.createdTs() ||
                this->quantity() < other.quantity() ||
                this->orderId() < other.orderId();
      } else {
         // Asks: Lower prices at the top
         return this->price() > other.price() ||
                this->createdTs() < other.createdTs() ||
                this->quantity() < other.quantity() ||
                this->orderId() < other.orderId();
      }
   }

   bool operator==(const Order &other) const {
      return this->orderId() == other.orderId() &&
             this->createdTs() == other.createdTs() &&
             this->quantity() == other.quantity() &&
             this->price() == other.price();
   }

   bool operator>(const Order &other) const {
      return !(*this == other || *this < other);
   }

private:
   Uuid order_id_;
   Timestamp created_ts_;
   Quantity quantity_;
   Price price_;

   const OrderType order_type_ = OrderType_;
};

#endif //LIMITORDERBOOK_ORDER_H
