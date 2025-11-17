#ifndef LIMITORDERBOOK_ORDERBOOK_H
#define LIMITORDERBOOK_ORDERBOOK_H

#include <list>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include "Order.h"
#include "typedefs.h"

template<Side _Side>
using GeneralBook = std::map<Price, std::list<std::weak_ptr<Order> >,
   std::conditional_t<_Side == BID, std::greater<>, std::less<> >>;

using BidBook = std::map<Price, std::list<std::weak_ptr<Order> >, std::greater<> >;
using AskBook = std::map<Price, std::list<std::weak_ptr<Order> >, std::less<> >;

static_assert(std::is_same_v<BidBook, GeneralBook<BID> >);
static_assert(std::is_same_v<AskBook, GeneralBook<ASK> >);

// NOTE: Nowhere do we actually tell the ask how much it's been paid

class OrderBook {
public:
   OrderBook() = default;

   bool add_order(const Order &order);
   bool delete_order(const Uuid& orderId);
   void pretty_print() const;

private:
   void fill_order(std::shared_ptr<Order> & order, Quantity quantity);
   bool try_fill_ask(std::shared_ptr<Order> &ask, BidBook &book);
   bool try_fill_bid(std::shared_ptr<Order> &bid, AskBook &book);

   BidBook price_to_bids{};
   AskBook price_to_asks{};

   std::unordered_map<Uuid, std::shared_ptr<Order> > uuid_to_order_map{};
   std::unordered_set<Uuid> filled_orders{};
};


#endif //LIMITORDERBOOK_ORDERBOOK_H
