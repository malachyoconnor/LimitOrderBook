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

enum Side {
   BID,
   ASK
};

class Order {
public:
   explicit Order(Price price, Quantity quantity, Uuid order_id, Side side) : price_(price), quantity_(quantity),
                                                                              order_id_(order_id), side_(side) {
   }

   std::string string() const {
      return std::format("{} £{} #{:x} {}",
                         (side_ == ASK ? "ASK" : "BID"),
                         price_.string(),
                         quantity_.quantity(),
                         order_id_);
   }

   Quantity fill(Quantity &fill_by) {
      assert(fill_by <= quantity() && "Tried to overfill quantity");

      quantity_ = quantity() - fill_by;
      return quantity_;
   }

   [[nodiscard]] Price price() const {
      return price_;
   }

   [[nodiscard]] Quantity quantity() const {
      return quantity_;
   }

   [[nodiscard]] Uuid order_id() const {
      return order_id_;
   }

   [[nodiscard]] Side side() const {
      return side_;
   }

private:
   Price price_;
   Quantity quantity_;
   Uuid order_id_;
   Side side_;
};

using OrderStorage = std::vector<Order>;

template<Side Side_>
using OppositeBook = std::map<Price, std::list<OrderStorage::iterator>,
   std::conditional_t<Side_ == BID, std::less<>, std::greater<> > >;

template<Side Side_>
using CorrectBook = std::map<Price, std::list<OrderStorage::iterator>,
   std::conditional_t<Side_ == ASK, std::less<>, std::greater<> > >;


using BidBook = std::map<Price, std::list<OrderStorage::iterator>, std::greater<> >;
using AskBook = std::map<Price, std::list<OrderStorage::iterator>, std::less<> >;

static_assert(std::is_same_v<AskBook, OppositeBook<BID> >);
static_assert(std::is_same_v<BidBook, OppositeBook<ASK> >);

static_assert(std::is_same_v<AskBook, CorrectBook<ASK> >);
static_assert(std::is_same_v<BidBook, CorrectBook<BID> >);


class OrderBook {
public:
   OrderBook() = default;

   bool add_order(Order &order);

   bool remove_order(Uuid &);

   void pretty_print() const;

private:
   void fill_order(const OrderStorage::iterator &order_to_fill, Quantity quantity);

   template<Side Side_>
   bool try_fill_order(const OrderStorage::iterator &new_order, OppositeBook<Side_> &book);

   bool delete_from_level(Uuid orderId, std::list<OrderStorage::iterator> &book);

   OrderStorage::iterator store_order(Order &&to_store);

   BidBook bid_book_{};
   AskBook ask_book_{};

   OrderStorage order_storage_{};
   std::stack<OrderStorage::iterator> available_indexes_{};
};


#endif //LIMITORDERBOOK_ORDERBOOK_H
