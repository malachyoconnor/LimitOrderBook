#include "OrderBook.h"
#include <format>
#include <iostream>
#include <ranges>

// Returns `filled`
bool OrderBook::try_fill_ask(std::shared_ptr<Order> &ask, BidBook &book) {
   if (ask->quantity() == 0) return true;

   for (auto &[price, level]: book) {
      if (price < ask->price()) break;

      for (auto iter = level.begin(); iter != level.end(); ) {
         if (auto order = iter->lock(); order) {
            if (order->quantity() > ask->quantity()) {
               fill_order(order, ask->quantity());
               fill_order(ask, ask->quantity());
               ++iter;
            } else {
               fill_order(ask, order->quantity());
               fill_order(order, order->quantity());
               ++iter;
               level.pop_front();
            }

            if (ask->quantity() == 0) return true;
         } else {
            ++iter;
            level.pop_front();
         }
      }
   }

   return false;
}

// Returns `filled`
bool OrderBook::try_fill_bid(std::shared_ptr<Order> &bid, AskBook &book) {
   if (bid->quantity() == 0) return true;

   for (auto &[price, level]: book) {
      if (price > bid->price()) break;

      for (auto &weak_order: level) {

         if (auto order = weak_order.lock(); order) {
            if (order->quantity() > bid->quantity()) {
               fill_order(order, bid->quantity());
               fill_order(bid, bid->quantity());
            } else {
               fill_order(bid, order->quantity());
               fill_order(order, order->quantity());
               level.pop_front();
            }

            if (bid->quantity() == 0) return true;
         } else {
            level.pop_front();
         }
      }
   }

   return false;
}

bool OrderBook::add_order(const Order &order) {
   if (order.quantity() == 0) return false;
   if (uuid_to_order_map.contains(order.order_id()) && uuid_to_order_map[order.order_id()]) {
      return false;
   }

   auto order_pointer = std::make_shared<Order>(order);

   if (order.side() == ASK) {
      if (try_fill_ask(order_pointer, price_to_bids)) return true;

      price_to_asks[order.price()].emplace_back(order_pointer);
   } else {
      if (try_fill_bid(order_pointer, price_to_asks)) return true;

      price_to_bids[order.price()].emplace_back(order_pointer);
   }

   uuid_to_order_map[order.order_id()] = order_pointer;
   return true;
}

bool OrderBook::delete_order(const Uuid &orderId) {
   if (filled_orders.contains(orderId)) return false;
   if (!uuid_to_order_map.contains(orderId)) return false;

   if (auto weakOrderPtr = uuid_to_order_map[orderId]; !weakOrderPtr) {
      uuid_to_order_map.erase(orderId);
      return false;
   }

   auto orderPtr = uuid_to_order_map[orderId];

   if (!orderPtr.unique()) {
      throw std::logic_error(std::format("Non-unique pointer in the uuid to order map for order {}",
                                         orderPtr->order_id()));
   }

   uuid_to_order_map[orderId].reset();
   return true;
}

void OrderBook::fill_order(std::shared_ptr<Order> &order, Quantity quantity) {
   if (quantity > order->quantity()) {
      throw std::logic_error(std::format("Tried to overfill order {}", order->order_id()));
   }

   order->quantity_ = order->quantity() - quantity;
   if (order->quantity() == 0) {
      filled_orders.insert(order->order_id());
   }
}

void OrderBook::pretty_print() const {
   std::cout << std::format("|{:^40}|{:^40}|", "BID", "ASK");

   std::vector<std::shared_ptr<Order> > all_bids{};
   std::vector<std::shared_ptr<Order> > all_asks{};

   for (const auto &level: price_to_bids | std::views::values) {
      for (const auto &bid: level) {
         all_bids.emplace_back(bid);
      }
   }
   for (const auto &level: price_to_asks | std::views::values) {
      for (const auto &ask: level) {
         all_asks.emplace_back(ask);
      }
   }

   for (int bid_iter = 0, ask_iter = 0; bid_iter < all_bids.size() || ask_iter < all_asks.size();
        ++bid_iter, ++ask_iter) {
      std::cout << "\n";
      if (bid_iter < all_bids.size()) {
         std::cout << std::format("|{:^41}|", all_bids[bid_iter]->string());
      } else {
         std::cout << std::format("|{:^40}|", "");
      }

      if (ask_iter < all_asks.size()) {
         std::cout << std::format("{:^41}|", all_asks[ask_iter]->string());
      } else {
         std::cout << std::format("{:^40}|", "");
      }
   }
}
