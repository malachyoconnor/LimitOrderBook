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
      case Side::BID: return "BID";
      case Side::ASK: return "ASK";
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
   static Order NewMarketOrder(Uuid orderId, Quantity quantity, Side side) {
      return {orderId, Price(0), quantity, side, OrderType::Market};
   }

   static Order NewOrder(Uuid orderId, Price price, Quantity quantity, Side side, OrderType orderType) {
      assert(orderType != OrderType::Market);
      return {orderId, price, quantity, side, orderType};
   }

   [[nodiscard]]
   Uuid GetOrderId() const { return orderId_; }

   [[nodiscard]]
   Price GetPrice() const { return price_; }

   [[nodiscard]]
   Quantity GetQuantity() const { return quantity_; }

   [[nodiscard]]
   Side GetSide() const { return side_; }

   [[nodiscard]]
   OrderType GetOrderType() const { return orderType_; }

   [[nodiscard]]
   bool IsFilled() const { return GetQuantity() == ZERO; }

   Order ToFillAndKill(Price price) {
      assert(orderType_ == OrderType::Market);

      orderType_ = OrderType::FillAndKill;
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
   Order(Uuid orderId, Price price, Quantity quantity, Side side, OrderType orderType) : orderId_(orderId),
      price_(price),
      quantity_(quantity),
      side_(side),
      orderType_(orderType) {
   }

   Order(Uuid orderId, Quantity quantity, Side side, OrderType orderType)
      : orderId_(orderId),
        price_(0),
        quantity_(quantity),
        side_(side),
        orderType_(orderType) {
      assert(orderType == OrderType::Market);
   }

   Uuid orderId_;
   Price price_;
   Quantity quantity_;
   Side side_;
   OrderType orderType_;
};
