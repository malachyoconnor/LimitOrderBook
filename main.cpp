#include <iostream>

#include "Random.h"
#include "OrderBook.h"

int main() {
   uint64_t id = 1;
   std::vector<Order> orders{
      Order{Price(10), Quantity(10), id++, BID},
      Order{Price(10), Quantity(10), id++, BID},
      Order{Price(10), Quantity(10), id++, BID},
      Order{Price(10), Quantity(10), id++, BID},
      Order{Price(10), Quantity(10), id++, BID},

   };

   OrderBook book{};
   for (auto &order: orders) {
      book.add_order(order);
   }

   book.pretty_print();

   return 0;
}
