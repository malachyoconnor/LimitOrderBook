#pragma once

#include "Price.h"
#include "Quantity.h"
#include "OrderType.h"

enum class Side {
    BID,
    ASK
};

constexpr Side otherSide(Side side) {
   if (side == Side::BID) {
      return Side::ASK;
   } else {
      return Side::BID;
   }
}

class Order {
public:

    Order(Uuid orderId, Price price, Quantity quantity, Side side, OrderType OrderType) :
        orderId_(orderId),
        price_(price),
        quantity_(quantity),
        side_(side),
        orderType_(OrderType) {}


    Uuid GetOrderId() const {return orderId_;}
    Price GetPrice() const {return price_;}
    Quantity GetQuantity() const {return quantity_;}
    Side GetSide() const {return side_;}
    OrderType GetOrderType() const {return orderType_;}

    bool isFilled() const {return GetQuantity() == ZERO;}

    Order& fillOrder(Quantity other_quantity) {
        assert(GetQuantity() >= other_quantity);

        quantity_ = GetQuantity().fill(other_quantity);
        return *this;
    }

private:
    Uuid orderId_;
    Price price_;
    Quantity quantity_;
    Side side_;
    OrderType orderType_;
};
