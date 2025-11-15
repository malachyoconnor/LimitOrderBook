#include "OrderBook.h"

Quantity OrderBook::fill_any_asks(Order<BID> &bid) {

}


Quantity OrderBook::fill_any_bids(Order<BID> &bid) {
   return Quantity(0);
}

bool OrderBook::add_bid(Order<BID> &&bid) {
   if (bid_map.contains(bid.uuid())) return false;

   Quantity filled = fill_any_asks(bid);
   if (filled.get_quantity() != 0) {
      // WTF
      bid = Order<BID>(bid.uuid(), bid.created_ts(), bid.quantity(), bid.price());
   }

   const auto bid_ptr = std::make_shared<Order<BID> >(std::move(bid));
   bid_map[bid_ptr->uuid()] = bid_ptr;

   if (!price_to_bid.contains(bid_ptr->price())) {
      price_to_bid[bid_ptr->price()] = BidList();
   }

   price_to_bid[bid_ptr->price()].push_back(bid_ptr);
   return true;
}

bool OrderBook::add_ask(Order<ASK> &&ask) {
}

bool OrderBook::delete_order(Uuid uuid) {
}
