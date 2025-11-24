#pragma once

#include "Order.h"
#include "Price.h"
#include <list>
#include <map>
#include <unordered_map>

#include "Trade.h"

using OrderPointer = std::list<Order>::iterator;

using enum Side;

template<Side Side_>
using Book =
std::map<Price, std::list<Order>, std::conditional_t<Side_ == BID, std::greater<>, std::less<> > >;

class OrderBook {
public:
   OrderBook() = default;

   template<Side Side_>
   bool TryFill(Order &orderToFill);

   bool AddOrder(Order order);
   bool DeleteOrder(Uuid uuid);

   std::size_t Size() const { return order_map_.size(); };



private:
   template<Side Side_>
   Book<Side_> &getBook();

   void OnMatch(Order &orderToFill, Order &otherOrder);

   std::map<Price, std::list<Order>, std::greater<> > bids_{};
   std::map<Price, std::list<Order>, std::less<> > asks_{};
   std::unordered_map<Uuid, OrderPointer> order_map_{};
   std::unordered_map<Uuid, std::vector<Trade> > all_trades_{};
};
