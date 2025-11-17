#include "OrderBook.h"

#include <iostream>
#include <ranges>
#include <set>

#include "Order.h"

Quantity OrderBook::try_fill_bid(Order<BID> &bid) {
   assert(bid.quantity() != 0);
   if (!price_to_ask.contains(bid.price())) return Quantity(0);

   std::list<AskWeakPointer> level = price_to_ask[bid.price()];

   Quantity initial_quantity{bid.quantity()};

   while (!level.empty() && !bid.isFilled()) {
      if (level.front().expired()) {
         level.pop_front();
         continue;
      }

      auto ask = level.front().lock();

      if (ask->quantity() > bid.quantity()) {
         ask->fillOrder(bid.quantity());
         bid.fillOrder(bid.quantity());
         return initial_quantity;
      } else {
         bid.fillOrder(ask->quantity());
         ask->fillOrder(ask->quantity());
         level.pop_front();
      }
   }

   return initial_quantity - bid.quantity();
}


Quantity OrderBook::try_fill_ask(Order<ASK> &ask) {
   assert(ask.quantity() != 0);
   if (!price_to_ask.contains(ask.price())) return Quantity(0);

   std::list<BidWeakPointer> level = price_to_bid[ask.price()];

   Quantity initial_quantity{ask.quantity()};

   while (!level.empty() && !ask.isFilled()) {

      if (level.front().expired()) {
         level.pop_front();
         continue;
      }

      auto bid = level.front().lock();

      if (bid->quantity() > ask.quantity()) {
         bid->fillOrder(ask.quantity());
         ask.fillOrder(ask.quantity());
         return initial_quantity;
      } else {
         ask.fillOrder(bid->quantity());
         bid->fillOrder(bid->quantity());
         level.pop_front();
      }
   }

   return initial_quantity - ask.quantity();
}

bool OrderBook::add_bid(Order<BID> &bid) {
   if (bid_map.contains(bid.orderId())) return false;

   try_fill_bid(bid);

   if (bid.isFilled()) {
      return true;
   }

   const auto bid_ptr = std::make_shared<Order<BID> >(std::move(bid));
   bid_map[bid_ptr->orderId()] = bid_ptr;

   if (!price_to_bid.contains(bid_ptr->price())) {
      price_to_bid[bid_ptr->price()] = BidList();
   }

   price_to_bid[bid_ptr->price()].push_back(bid_ptr);
   return true;
}

bool OrderBook::add_ask(Order<ASK> &ask) {
   if (ask_map.contains(ask.orderId())) return false;

   try_fill_ask(ask);

   if (ask.isFilled()) {
      return true;
   }

   const auto bid_ptr = std::make_shared<Order<ASK> >(std::move(ask));
   ask_map[bid_ptr->orderId()] = bid_ptr;

   if (!price_to_ask.contains(bid_ptr->price())) {
      price_to_ask[bid_ptr->price()] = AskList();
   }

   price_to_ask[bid_ptr->price()].push_back(bid_ptr);
   return true;
}

bool OrderBook::delete_order(Uuid uuid) {
   if (!bid_map.contains(uuid) && !ask_map.contains(uuid)) return false;

   if (bid_map.contains(uuid)) {
      bid_map.erase(uuid);
   } else {
      ask_map.erase(uuid);
   }

   return true;
}

void OrderBook::pretty_print() {
   std::vector<Price> all_prices{};

   for (const auto &key: price_to_bid | std::views::keys) all_prices.push_back(key);
   for (const auto &key: price_to_ask | std::views::keys) all_prices.push_back(key);

   std::sort(all_prices.begin(), all_prices.end());
   all_prices.erase(std::unique(all_prices.begin(), all_prices.end()), all_prices.end());

   std::cout << std::format("|{:^40}|{:^40}|", "ASK", "BID")
         << std::endl;

   for (const Price &price: all_prices) {
      auto bid_level = price_to_bid[price];
      auto ask_level = price_to_ask[price];

      auto ask_iter = ask_level.begin();
      auto bid_iter = bid_level.begin();

      for (int i = 0; i < std::max(bid_level.size(), ask_level.size()); i++) {
         if (ask_level.size() > i) {
            std::cout << std::format("|{:^40}", (ask_iter++)->lock()->to_string());
         } else {
            std::cout << std::format("|{:>40}|", "");
         }

         if (bid_level.size() > i) {
            std::cout << std::format("{:^40}|", (bid_iter++)->lock()->to_string());
         } else {
            std::cout << std::format("{:>40}|", "");
         }
         std::cout << "\n";
      }

   }
}
