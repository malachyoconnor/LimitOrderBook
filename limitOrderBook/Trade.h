#pragma once

#include "Order.h"
#include "typedefs.h"
#include "Quantity.h"
#include "OrderType.h"
#include "Price.h"

class Order;

class TradeInfo {
public:
   explicit TradeInfo(const Order &order, const Quantity &quantityTraded, const Price &price)
      : orderId_(order.GetOrderId()),
        tradedPrice_(price),
        originalPrice_(order.GetPrice()),
        quantity_(quantityTraded),
        orderType_(order.GetOrderType()) {
   }

   Price GetTradedPrice() const {
      return tradedPrice_;
   }

   Price GetOriginalPrice() const {
      return originalPrice_;
   }

   Quantity GetQuantity() const {
      return quantity_;
   }

private:
   Uuid orderId_;
   Price tradedPrice_;
   Price originalPrice_;
   Quantity quantity_;
   OrderType orderType_;
};

class Trade {
public:
   explicit Trade(const Order &order1, const Order &order2, const Quantity quantityTraded, const Price tradePrice)
      : askTradeInfo_(order1, quantityTraded, tradePrice),
        bidTradeInfo_(order2, quantityTraded, tradePrice) {

      if (order1.GetSide() != Side::ASK) {
         std::swap(askTradeInfo_, bidTradeInfo_);
      }
      assert(order2.GetSide()!= order1.GetSide());
   }

   Price GetPrice() const {
      assert(askTradeInfo_.GetTradedPrice() == bidTradeInfo_.GetTradedPrice());
      return askTradeInfo_.GetTradedPrice();
   }

   Quantity GetQuantity() const {
      assert(askTradeInfo_.GetQuantity() == bidTradeInfo_.GetQuantity());
      return bidTradeInfo_.GetQuantity();
   }

   Price GetOriginalAskPrice() const {
      return askTradeInfo_.GetOriginalPrice();
   }

   Price GetOriginalBidPrice() const {
      return bidTradeInfo_.GetOriginalPrice();
   }

private:
   TradeInfo askTradeInfo_;
   TradeInfo bidTradeInfo_;
};
