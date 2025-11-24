#pragma once

#include "Order.h"
#include "typedefs.h"
#include "Quantity.h"
#include "OrderType.h"
#include "Price.h"

class Order;

class TradeInfo {
public:
   explicit TradeInfo(Order &order, Quantity &quantityTraded, Price &price)
      : orderId_(order.GetOrderId()),
        price_(price),
        quantity_(quantityTraded),
        orderType_(order.GetOrderType()) {
   }

private:
   Uuid orderId_;
   Price price_;
   Quantity quantity_;
   OrderType orderType_;
};

class Trade {
public:
   explicit Trade(Order &order1, Order &order2, Quantity quantityTraded, Price tradePrice)
      : order1_(order1, quantityTraded, tradePrice),
        order2_(order2, quantityTraded, tradePrice) {

      assert(order2.GetSide()!= order1.GetSide());
   }

private:
   TradeInfo order1_;
   TradeInfo order2_;
};
