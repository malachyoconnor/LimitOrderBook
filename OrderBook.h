#ifndef LIMITORDERBOOK_ORDERBOOK_H
#define LIMITORDERBOOK_ORDERBOOK_H

#include <list>
#include <unordered_map>

#include "typedefs.h"
#include "Order.h"
#include "Price.h"
#include "Quantity.h"

using BidList = std::list<std::weak_ptr<Order<BID> > >;
using AskList = std::list<std::weak_ptr<Order<ASK> > >;

class OrderBook {
public:
   OrderBook() = default;

   bool add_bid(Order<BID> &bid);
   bool add_ask(Order<ASK> &ask);

   bool delete_order(Uuid uuid);

   void pretty_print();

private:
   Quantity try_fill_bid(Order<BID> &bid);
   Quantity try_fill_ask(Order<ASK> & ask);

   std::unordered_map<Price, BidList, PriceHasher> price_to_bid{};
   std::unordered_map<Price, AskList, PriceHasher> price_to_ask{};

   std::unordered_map<Uuid, std::shared_ptr<Order<BID> > > bid_map{};
   std::unordered_map<Uuid, std::shared_ptr<Order<ASK> > > ask_map{};
};

#endif //LIMITORDERBOOK_ORDERBOOK_H
