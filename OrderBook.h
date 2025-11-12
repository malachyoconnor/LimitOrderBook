#ifndef LIMITORDERBOOK_ORDERBOOK_H
#define LIMITORDERBOOK_ORDERBOOK_H

#include <queue>

#include "typedefs.h"
#include "Order.h"
#include "Price.h"
#include "Quantity.h"

template<OrderType _OrderType>
using Queue = std::priority_queue<Order<_OrderType>, std::vector<Order<_OrderType> >, std::greater<> >;

class OrderBook {
public:
   template<OrderType _OrderType>
   std::optional<Uuid> new_order(Order<_OrderType> &&order) {
      Queue<_OrderType> &queue = get_queue<_OrderType>();


      if (queue.find(order)) {
         return std::nullopt;
      }

      auto result_uuid = order.uuid();
      queue.push(order);

      return result_uuid;
   }

private:
   Queue<BID> bids_{};
   Queue<ASK> asks_{};

   template<OrderType _OrderType>
   constexpr Queue<_OrderType> &get_queue() {
      if constexpr (_OrderType == BID) {
         return bids_;
      } else {
         return asks_;
      }
   }
};


#endif //LIMITORDERBOOK_ORDERBOOK_H
