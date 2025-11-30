#include <iostream>
#include "OrderBook.h"
#include "libs/raylib.h"

int main() {

   OrderBook book{};

   std::vector<Order> test_orders{
      Order(1, Price(10), Quantity(10), BID, GoodTillCancel),
      Order(2, Price(12), Quantity(12), ASK, GoodTillCancel),
      Order(3, Quantity(8), ASK, Market),
      Order(3, Price(3), Quantity(3), ASK, FillOrKill),
   };

   for (auto &order: test_orders) {
      book.AddOrder(order);
      book.PrintBook();
      std::cout << std::endl;
   }


   return 0;
}
