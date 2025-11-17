#include <iostream>
#include "Order.h"
#include "OrderBook.h"
#include "Random.h"


int main() {

   auto b1 = Order<BID>(Random::uuid(), Clock::now(), Quantity{5}, Price{1003'31});
   auto b2 = Order<BID>(Random::uuid(), Clock::now(), Quantity{5}, Price{1023'31});
   auto b3 = Order<BID>(Random::uuid(), Clock::now(), Quantity{5}, Price{1103'31});

   auto book = OrderBook();
   book.add_bid(b1);
   book.add_bid(b2);
   book.add_bid(b3);


   book.pretty_print();



   return 0;
}
