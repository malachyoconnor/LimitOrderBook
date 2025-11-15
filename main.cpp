#include <algorithm>
#include <iostream>
#include <uuid.h>
#include "Order.h"
#include "OrderBook.h"
#include "Random.h"

using uuids::uuid;


int main() {

   auto order = Order<BID>(Random::uuid(), Clock::now(), Quantity(5), Price(1003.31));



   return 0;
}
