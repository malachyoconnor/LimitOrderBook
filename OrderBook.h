#ifndef LIMITORDERBOOK_ORDERBOOK_H
#define LIMITORDERBOOK_ORDERBOOK_H

#include <list>

#include "typedefs.h"
#include "Order.h"
#include "Price.h"
#include "Quantity.h"

using BidList = std::list<std::shared_ptr<Order<BID> > >;
using AskList = std::list<std::shared_ptr<Order<ASK> > >;

class OrderBook {
public:
   OrderBook();

   bool add_bid(Order<BID> &&bid);
   bool add_ask(Order<ASK> &&ask);

   bool delete_order(Uuid uuid);

private:
   Quantity fill_any_asks(Order<BID>& bid);
   Quantity fill_any_bids(Order<BID>& bid);

   std::unordered_map<Price, BidList, PriceHasher> price_to_bid{};
   std::unordered_map<Price, AskList, PriceHasher> price_to_ask{};

   std::unordered_map<Uuid, std::shared_ptr<Order<BID> > > bid_map{};
   std::unordered_map<Uuid, std::shared_ptr<Order<ASK> > > ask_map{};
};

#endif //LIMITORDERBOOK_ORDERBOOK_H
