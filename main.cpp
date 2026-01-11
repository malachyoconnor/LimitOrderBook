#include <iostream>
#include "OrderBook.h"
#include "limitOrderBook/OrderType.h"

int main() {

   OrderBook book{};

   const std::vector<Order> test_orders{
      Order::NewOrder(1, Price(10), Quantity(10), Side::BID, OrderType::GoodTillCancel),
      Order::NewOrder(2, Price(12), Quantity(12), Side::ASK, OrderType::GoodTillCancel),
      Order::NewMarketOrder(3, Quantity(8), Side::ASK),
      Order::NewOrder(4, Price(3), Quantity(3), Side::ASK, OrderType::FillOrKill),
   };

   for (auto &order: test_orders) {
      book.AddOrder(order);
      book.PrintBook();
      std::cout << std::endl;
   }


   return 0;
}
