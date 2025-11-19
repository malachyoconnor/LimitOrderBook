#include <iostream>

#include "Random.h"
#include "OrderBook.h"

int main() {

   std::vector<Order> orders{
      Order{Price(10), Quantity(10), 1, BID},
      Order{Price(10), Quantity(10), 2, BID},
      Order{Price(10), Quantity(10), 3, BID},
      Order{Price(10), Quantity(10), 4, BID},
      Order{Price(10), Quantity(10), 5, BID},
      Order{Price(10), Quantity(100),6, ASK},
   };

   OrderBook book{};
   for (auto &order: orders) {
      book.add_order(order);
   }

   book.pretty_print();

   book.delete_order(6);

   book.pretty_print();

   return 0;
}
