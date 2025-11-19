#ifndef LIMITORDERBOOK_ORDERBOOK_H
#define LIMITORDERBOOK_ORDERBOOK_H

#include <cassert>
#include <string>
#include <format>
#include <list>
#include <map>
#include <stack>

#include "Price.h"
#include "Quantity.h"
#include "typedefs.h"
#include "Order.h"

using OrderStorage = std::vector<Order>;

class OrderStorageIterator;

template<Side Side_>
using OppositeBook = std::map<Price, std::list<OrderStorageIterator>,
   std::conditional_t<Side_ == BID, std::less<>, std::greater<> > >;

template<Side Side_>
using CorrectBook = std::map<Price, std::list<OrderStorageIterator>,
   std::conditional_t<Side_ == ASK, std::less<>, std::greater<> > >;


using BidBook = std::map<Price, std::list<OrderStorageIterator>, std::greater<> >;
using AskBook = std::map<Price, std::list<OrderStorageIterator>, std::less<> >;

static_assert(std::is_same_v<AskBook, OppositeBook<BID> >);
static_assert(std::is_same_v<BidBook, OppositeBook<ASK> >);

static_assert(std::is_same_v<AskBook, CorrectBook<ASK> >);
static_assert(std::is_same_v<BidBook, CorrectBook<BID> >);

class OrderBook {
public:
   OrderBook() = default;

   bool add_order(Order order);

   bool delete_order(Uuid order_id);

   void pretty_print() const;

private:
   void fill_order(const OrderStorageIterator &order_to_fill, Quantity quantity);

   template<Side Side_>
   bool try_fill_order(const OrderStorageIterator &new_order, OppositeBook<Side_> &book);

   bool delete_from_level(Uuid orderId, std::list<OrderStorageIterator> &book);

   OrderStorageIterator store_order(const Order &to_store);

   BidBook bid_book_{};
   AskBook ask_book_{};

   OrderStorage order_storage_{};
   std::stack<OrderStorageIterator> available_indexes_{};
};

class OrderStorageIterator {
public:
   OrderStorageIterator() = delete;

   OrderStorageIterator(OrderStorage &order_storage_, const size_t index)
      : order_storage_(order_storage_), index_(index) {
      assert(index < order_storage_.size());
   }

   Order operator*() const {
      return order_storage_[index_];
   }

   Order* operator->() const {
      return &order_storage_[index_];
   }

private:
   OrderStorage &order_storage_;
   size_t index_;
};

#endif //LIMITORDERBOOK_ORDERBOOK_H
