#include <ranges>
#include <generator>
#include "OrderBook.h"

#include <iostream>

#include "Order.h"

template<Side Side_>
Book<Side_> &OrderBook::getBook() {
   if constexpr (Side_ == ASK) return asks_;
   else return bids_;
}

std::optional<Price> OrderBook::GetWorstPrice(const Order &order) const {
   if (order.GetSide() == ASK) {
      if (bids_.empty()) return std::nullopt;
      return bids_.rbegin()->second.begin()->GetPrice();
   } else {
      if (asks_.empty()) return std::nullopt;
      return asks_.rbegin()->second.begin()->GetPrice();
   }
}

void OrderBook::OnMatch(Order &orderToFill, Order &otherOrder) {
   assert(orderToFill.GetSide() != otherOrder.GetSide());

   Quantity quantity_traded = std::min(orderToFill.GetQuantity(), otherOrder.GetQuantity());

   Trade resulting_trade = Trade(orderToFill, otherOrder, quantity_traded, otherOrder.GetPrice());

   all_trades_[orderToFill.GetOrderId()].push_back(resulting_trade);
   all_trades_[otherOrder.GetOrderId()].push_back(resulting_trade);

   orderToFill.fillOrder(quantity_traded);
   otherOrder.fillOrder(quantity_traded);
}


bool OrderBook::AddOrder(Order order) {
   if (order_map_.contains(order.GetOrderId())) return false;

   if (order.GetOrderType() == Market) {
      if (auto marketPrice = GetWorstPrice(order); marketPrice.has_value()) {
         order = order.ToGoodTillCancel(marketPrice.value());
      } else {
         return false; // We shouldn't add a market order if there's no-one to take the other side.
      }
   }

   if (order.GetSide() == ASK) TryFill<ASK>(order);
   else if (order.GetSide() == BID) TryFill<BID>(order);

   if (order.isFilled()) return true;
   if (order.GetOrderType() == FillAndKill) return false;
   if (order.GetOrderType() == FillOrKill) return false;

   if (order.GetSide() == ASK) {
      asks_[order.GetPrice()].push_back(order);
      order_map_[order.GetOrderId()] = --asks_[order.GetPrice()].end();
   } else {
      bids_[order.GetPrice()].push_back(order);
      order_map_[order.GetOrderId()] = --bids_[order.GetPrice()].end();
   }

   return false;
}

template<Side Side_>
bool OrderBook::TryFill(Order &orderToFill) {

   auto &book = getBook<otherSide(Side_)>();

   if (orderToFill.GetOrderType() == FillOrKill && !CanBeFilled<Side_>(orderToFill)) return false;

   for (auto &[price, level]: book) {

      if (orderToFill.GetOrderType() != Market) {
         if (Side_ == ASK && price < orderToFill.GetPrice()) return orderToFill.isFilled();
         if (Side_ == BID && price > orderToFill.GetPrice()) return orderToFill.isFilled();
      }

      for (auto iter = level.begin(); iter != level.end();) {
         Order &other_order = *iter;

         OnMatch(orderToFill, other_order);

         if (other_order.isFilled()) {
            order_map_.erase(other_order.GetOrderId());
            level.erase(iter++);
         } else {
            ++iter;
         }
         if (orderToFill.isFilled()) return true;
      }
   }

   return orderToFill.isFilled();
}

template<Side Side_>
bool OrderBook::CanBeFilled(Order orderToFill) {
   for (auto &book = getBook<otherSide(Side_)>(); auto &[price, level]: book) {

      if (orderToFill.GetOrderType() != Market) {
         if (Side_ == ASK && price < orderToFill.GetPrice()) return orderToFill.isFilled();
         if (Side_ == BID && price > orderToFill.GetPrice()) return orderToFill.isFilled();
      }

      for (auto iter = level.begin(); iter != level.end();) {
         Order &other_order = *iter;
         Quantity quantity_traded = std::min(orderToFill.GetQuantity(), other_order.GetQuantity());

         orderToFill.fillOrder(quantity_traded);
         if (orderToFill.isFilled()) return true;
         ++iter;
      }
   }

   return orderToFill.isFilled();
}

bool OrderBook::DeleteOrder(Uuid uuid) {
   if (!order_map_.contains(uuid)) return false;

   OrderPointer orderPtr = order_map_[uuid];
   if (orderPtr->GetSide() == BID) {
      bids_[orderPtr->GetPrice()].erase(order_map_[uuid]);
      order_map_.erase(uuid);
   } else {
      asks_[orderPtr->GetPrice()].erase(order_map_[uuid]);
      order_map_.erase(uuid);
   }

   return true;
}

template<Side Side_>
std::generator<Order &> orderGenerator(Book<Side_> &book) {
   for (auto &level: book | std::views::values) {
      for (auto &order: level) {
         co_yield order;
      }
   }
}

void OrderBook::PrintBook() {

   auto bid_gen = orderGenerator<BID>(bids_);
   auto ask_gen = orderGenerator<ASK>(asks_);
   auto bid_iter = bid_gen.begin();
   auto ask_iter = ask_gen.begin();

   std::cout << std::format("{:^40}{:^40}", "BIDS", "ASKS") << std::endl;

   while (bid_iter != bid_gen.end() || ask_iter != ask_gen.end()) {
      if (bid_iter != bid_gen.end()) {
         std::cout << std::format("{:^40}", (*bid_iter).string());
         ++bid_iter;
      } else {
         std::cout << std::format("{:^40}", "");
      }

      if (ask_iter != ask_gen.end()) {
         std::cout << std::format("{:^40}", (*ask_iter).string()) << std::endl;
         ++ask_iter;
      } else {
         std::cout << std::format("{:^40}", "") << std::endl;
      }
   }

}
