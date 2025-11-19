#include "OrderBook.h"
#include <iostream>
#include <ranges>

template<Side Side_>
bool OrderBook::try_fill_order(const OrderStorage::iterator &new_order, OppositeBook<Side_> &book) {
   for (auto &[price, level]: book) {
      if constexpr (Side_ == ASK) {
         if (price < new_order->price()) return false;
      } else {
         if (price > new_order->price()) return false;
      }

      for (auto level_iter = level.begin(); level_iter != level.end();) {
         const auto other_order = *level_iter;

         if (other_order->quantity() > new_order->quantity()) {
            fill_order(other_order, new_order->quantity());
            fill_order(new_order, new_order->quantity());
            return true;
         }

         fill_order(new_order, other_order->quantity());
         fill_order(other_order, other_order->quantity());
         level.erase(level_iter++);
         if (new_order->quantity() == 0) return true;
      }
   }
   return false;
}

bool OrderBook::add_order(Order &order) {
   if (order.quantity() == 0) return false;

   OrderStorage::iterator order_iterator = store_order(std::move(order));

   if (order.side() == ASK) {
      if (try_fill_order<ASK>(order_iterator, bid_book_)) {
         return true;
      }

      ask_book_[order.price()].emplace_back(order_iterator);
   } else {
      if (try_fill_order<BID>(order_iterator, ask_book_)) {
         return true;
      }

      bid_book_[order.price()].emplace_back(order_iterator);
   }
   return false;
}

bool OrderBook::delete_from_level(Uuid orderId, std::list<OrderStorage::iterator> &level) {
   for (auto iter = level.begin(); iter != level.end();) {
      if ((*iter)->order_id() == orderId) {
         level.erase(iter++);
         return true;
      }
   }
   return false;
}

bool OrderBook::remove_order(Uuid &orderId) {
   for (size_t i = 0; i < order_storage_.size(); i++) {
      auto &relevant_order = order_storage_[i];
      if (relevant_order.order_id() == orderId) {
         available_indexes_.push(order_storage_.begin() + i);
         if (relevant_order.side() == ASK) {
            return delete_from_level(orderId, ask_book_[relevant_order.price()]);
         } else {
            return delete_from_level(orderId, bid_book_[relevant_order.price()]);
         }
      }
   }

   return false;
}

void OrderBook::fill_order(const OrderStorage::iterator &order_to_fill, Quantity quantity) {
   assert((order_to_fill->quantity() >= quantity) && "Tried to overfill an order");

   order_to_fill->fill(quantity);

   if (order_to_fill->quantity() == 0) {
      std::cout << "FILLED:" << order_to_fill->string() << std::endl;
      *order_to_fill = Order(Price(0), Quantity(0), 0, order_to_fill->side());
      available_indexes_.push(std::move(order_to_fill));
   }
}

OrderStorage::iterator OrderBook::store_order(Order &&to_store) {
   if (available_indexes_.size() == 0) {
      order_storage_.push_back(to_store);
      return --order_storage_.end();
   }

   OrderStorage::iterator iter = available_indexes_.top();
   available_indexes_.pop();
   *iter = to_store;
   return iter;
}

void OrderBook::pretty_print() const {
   std::cout << std::format("|{:^40}|{:^40}|", "BID", "ASK");

   std::vector<Order> all_bids{};
   std::vector<Order> all_asks{};

   for (const auto &level: bid_book_ | std::views::values) {
      for (const auto &bid: level) {
         all_bids.push_back(*bid);
      }
   }
   for (const auto &level: ask_book_ | std::views::values) {
      for (const auto &ask: level) {
         all_asks.push_back(*ask);
      }
   }

   for (size_t bid_iter = 0, ask_iter = 0; bid_iter < all_bids.size() || ask_iter < all_asks.size();
        ++bid_iter, ++ask_iter) {
      std::cout << "\n";
      if (bid_iter < all_bids.size()) {
         std::cout << std::format("|{:^40}|", all_bids[bid_iter].string());
      } else {
         std::cout << std::format("|{:^40}|", "");
      }

      if (ask_iter < all_asks.size()) {
         std::cout << std::format("{:^40}|", all_asks[ask_iter].string());
      } else {
         std::cout << std::format("{:^40}|", "");
      }
   }
   std::cout << '\n' << std::endl;
}
