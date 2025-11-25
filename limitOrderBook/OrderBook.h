#pragma once

#include "Order.h"
#include "Price.h"
#include <list>
#include <map>
#include <mutex>
#include <generator>
#include <unordered_map>

#include "Trade.h"

using enum Side;
using enum OrderType;
using OrderPointer = std::list<Order>::iterator;

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
   void PrintBook();

   template<Side Side_>
   std::generator<Order> orderGenerator();

   std::size_t Size() const { return order_map_.size(); };

private:
   template<Side Side_>
   Book<Side_> &getBook();

   template<Side Side_>
   bool CanBeFilled(Order orderToFill);
   std::optional<Price> GetWorstPrice(const Order &order) const;

   void OnMatch(Order &orderToFill, Order &otherOrder);

   std::map<Price, std::list<Order>, std::greater<> > bids_{};
   std::map<Price, std::list<Order>, std::less<> > asks_{};
   std::unordered_map<Uuid, OrderPointer> order_map_{};
   std::unordered_map<Uuid, std::vector<Trade> > all_trades_{};
};
