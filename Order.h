#pragma once

#include "Price.h"
#include "Quantity.h"
#include "OrderType.h"

enum class Side {
   BID,
   ASK
};

inline std::string toString(const Side side) {
   switch (side) {
      case Side::BID:
         return "BID";
      case Side::ASK:
         return "ASK";
      default: assert(false);
   }
}

constexpr Side otherSide(Side side) {
   if (side == Side::BID) {
      return Side::ASK;
   }
   return Side::BID;
}

class Order {
public:
   Order(Uuid orderId, Price price, Quantity quantity, Side side, OrderType orderType) : orderId_(orderId),
      price_(price),
      quantity_(quantity),
      side_(side),
      orderType_(orderType) {
      assert(orderType != OrderType::Market);
   }

   Order(Uuid orderId, Quantity quantity, Side side, OrderType orderType)
      : orderId_(orderId),
        price_(0),
        quantity_(quantity),
        side_(side),
        orderType_(orderType) {
      assert(orderType == OrderType::Market);
   }


   Uuid GetOrderId() const { return orderId_; }
   Price GetPrice() const { return price_; }
   Quantity GetQuantity() const { return quantity_; }
   Side GetSide() const { return side_; }
   OrderType GetOrderType() const { return orderType_; }

   bool isFilled() const { return GetQuantity() == ZERO; }

   Order &ToGoodTillCancel(Price price) {
      assert(orderType_ == OrderType::Market);

      price_ = price;
      return *this;
   }

   Order &fillOrder(Quantity other_quantity) {
      assert(GetQuantity() >= other_quantity);

      quantity_ = GetQuantity().fill(other_quantity);
      return *this;
   }

   std::string string() const {
      return std::format("#{} £{}.{} {}x {} {}",
                         orderId_,
                         price_.GetPrice() / 100,
                         price_.GetPrice() % 100,
                         quantity_.GetQuantity(),
                         toString(side_),
                         toString(orderType_));
   }

private:
   Uuid orderId_;
   Price price_;
   Quantity quantity_;
   Side side_;
   OrderType orderType_;
};
