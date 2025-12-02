#include <ranges>
#include <generator>
#include "OrderBook.h"

#include <iostream>

#include "Order.h"

using enum Side;
using enum OrderType;

template<Side Side_>
Book<Side_> &OrderBook::GetBook() {
   if constexpr (Side_ == ASK) return asks_;
   else return bids_;
}

std::optional<Price> OrderBook::GetWorstPrice(const Order &order) const {
   if (order.GetSide() == ASK) {
      for (const auto &level: bids_ | std::views::values) {
         if (!level.empty()) return level.begin()->GetPrice();
      }
   } else {
      for (const auto &level: asks_ | std::views::values) {
         if (!level.empty()) return level.begin()->GetPrice();
      }
   }
   return std::nullopt;
}

void OrderBook::OnMatch(Order &orderToFill, Order &otherOrder) {
   assert(orderToFill.GetSide() != otherOrder.GetSide());

   Quantity quantity_traded = std::min(orderToFill.GetQuantity(), otherOrder.GetQuantity());
   all_trades_.emplace_back(orderToFill, otherOrder, quantity_traded, otherOrder.GetPrice());

   orderToFill.fillOrder(quantity_traded);
   otherOrder.fillOrder(quantity_traded);
}


bool OrderBook::AddOrder(Order order) {
   if (order_map_.contains(order.GetOrderId())) return false;

   if (order.GetOrderType() == Market) {
      auto marketPrice = GetWorstPrice(order);
      if (!marketPrice) return false; // No liquidity

      order.ToFillAndKill(*marketPrice);
   }

   if (order.GetSide() == ASK) TryFill<ASK>(order);
   else TryFill<BID>(order);

   if (order.IsFilled()) return true;
   if (order.GetOrderType() == FillAndKill ||
       order.GetOrderType() == FillOrKill) {
      return false;
   }

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

   auto &book = GetBook<otherSide(Side_)>();
   if (orderToFill.GetOrderType() == FillOrKill && !CanBeFilled<Side_>(orderToFill)) return false;

   for (auto book_iter = book.begin(); book_iter != book.end();) {
      auto &[price, level] = *book_iter;

      if (orderToFill.GetOrderType() != Market) {
         if (Side_ == ASK && price < orderToFill.GetPrice()) return orderToFill.IsFilled();
         if (Side_ == BID && price > orderToFill.GetPrice()) return orderToFill.IsFilled();
      }

      for (auto level_iter = level.begin(); level_iter != level.end();) {
         Order &other_order = *level_iter;
         OnMatch(orderToFill, other_order);

         if (other_order.IsFilled()) {
            order_map_.erase(other_order.GetOrderId());
            level.erase(level_iter++);
         } else {
            ++level_iter;
         }
         if (orderToFill.IsFilled()) return true;
      }

      if (level.empty()) {
         book.erase(book_iter++);
      } else {
         ++book_iter;
      }
   }
   return orderToFill.IsFilled();
}

template<Side Side_>
bool OrderBook::CanBeFilled(Order orderToFill) {
   for (auto &book = GetBook<otherSide(Side_)>(); auto &[price, level]: book) {

      if (orderToFill.GetOrderType() != Market) {
         if (Side_ == ASK && price < orderToFill.GetPrice()) return orderToFill.IsFilled();
         if (Side_ == BID && price > orderToFill.GetPrice()) return orderToFill.IsFilled();
      }

      for (auto iter = level.begin(); iter != level.end();) {
         Order &other_order = *iter;
         Quantity quantity_traded = std::min(orderToFill.GetQuantity(), other_order.GetQuantity());

         orderToFill.fillOrder(quantity_traded);
         if (orderToFill.IsFilled()) return true;
         ++iter;
      }
   }

   return orderToFill.IsFilled();
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
std::generator<Order> OrderBook::WalkOrders() {

   Book<Side_> &book = GetBook<Side_>();

   for (auto &level: book | std::views::values) {
      for (auto &order: level) {
         assert(order.GetPrice().GetPrice() != 0);
         co_yield order;
      }
   }
}

void OrderBook::PrintBook() {

   auto bid_gen = WalkOrders<BID>();
   auto ask_gen = WalkOrders<ASK>();
   auto bid_iter = bid_gen.begin();
   auto ask_iter = ask_gen.begin();

   std::cout << std::format("{:^50}{:^50}", "BIDS", "ASKS") << std::endl;

   while (bid_iter != bid_gen.end() || ask_iter != ask_gen.end()) {
      if (bid_iter != bid_gen.end()) {
         std::cout << std::format("{:^50}", (*bid_iter).string());
         ++bid_iter;
      } else {
         std::cout << std::format("{:^50}", "");
      }

      if (ask_iter != ask_gen.end()) {
         std::cout << std::format("{:^50}", (*ask_iter).string()) << std::endl;
         ++ask_iter;
      } else {
         std::cout << std::format("{:^50}", "") << std::endl;
      }
   }
}

std::generator<Trade> OrderBook::WalkTrades() {
   for (const auto &trade: all_trades_) {
      co_yield trade;
   }
}
