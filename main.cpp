#include <iostream>

#include "OrderBook.h"
#include "Random.h"

int main() {

   OrderBook book {};

   std::vector<Order> orders {
      Order(1, Price(10), Quantity(10), BID, OrderType::GoodTillCancel),
      Order(2, Price(10), Quantity(10), ASK, OrderType::GoodTillCancel)
   };

   for (auto& order : orders) {
      book.AddOrder(order);
      std::cout << book.Size()  << std::endl;
   }


   return 0;
}
