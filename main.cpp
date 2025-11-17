#include <algorithm>
#include <iostream>
#include "Order.h"
#include "OrderBook.h"
#include "Random.h"



int main() {
   auto book = OrderBook();

   Uuid id = 0;

   std::vector orders = {
      Order(id++, Clock::now(), Quantity(3), Price(3'00), BID),
      Order(id++, Clock::now(), Quantity(10), Price(30'00), ASK),
      Order(id++, Clock::now(), Quantity(3), Price(10'00), BID),
      Order(id++, Clock::now(), Quantity(10), Price(20'00), BID),
      Order(id++, Clock::now(), Quantity(10), Price(20'00), ASK),
   };

   for (auto order : orders) {
      book.add_order(order);
   }

   book.pretty_print();

   return 0;
}
