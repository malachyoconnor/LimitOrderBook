#include <ranges>
#include "OrderBook.h"
#include "Order.h"

template<Side Side_>
Book<Side_> &OrderBook::getBook() {
   if constexpr (Side_ == ASK) return asks_;
   else return bids_;
}

template<Side Side_>
bool OrderBook::TryFill(Order &orderToFill) {

   Book<otherSide(Side_)> book = getBook<otherSide(Side_)>();

   for (auto &level: book | std::views::values) {
      for (auto iter = level.begin(); iter != level.end();) {
         Order &other_order = *iter;

         OnMatch(orderToFill, other_order);

         if (other_order.isFilled()) {
            order_map_.erase(other_order.GetOrderId());
            level.erase(iter++);
         } else {
            ++iter;
         }
         if (orderToFill.isFilled()) return true;
      }
   }

   return orderToFill.isFilled();
}

void OrderBook::OnMatch(Order &orderToFill, Order &otherOrder) {
   assert(orderToFill.GetSide() != otherOrder.GetSide());

   Quantity quantity_traded = std::min(orderToFill.GetQuantity(), otherOrder.GetQuantity());

   Trade resulting_trade = Trade(orderToFill, otherOrder, quantity_traded, orderToFill.GetPrice());

   all_trades_[orderToFill.GetOrderId()].push_back(resulting_trade);
   all_trades_[otherOrder.GetOrderId()].push_back(resulting_trade);

   orderToFill.fillOrder(quantity_traded);
   otherOrder.fillOrder(quantity_traded);
}


bool OrderBook::AddOrder(Order order) {
   if (order_map_.contains(order.GetOrderId())) return false;

   if (order.GetSide() == ASK) TryFill<ASK>(order);
   else if (order.GetSide() == BID) TryFill<BID>(order);

   if (order.isFilled()) return true;

   if (order.GetSide() == ASK) {
      asks_[order.GetPrice()].push_back(order);
      order_map_[order.GetOrderId()] = --asks_[order.GetPrice()].end();
   } else {
      bids_[order.GetPrice()].push_back(order);
      order_map_[order.GetOrderId()] = --bids_[order.GetPrice()].end();
   }

   return false;
}

bool OrderBook::DeleteOrder(Uuid uuid) {
   if (!order_map_.contains(uuid)) return false;

   OrderPointer orderPtr = order_map_[uuid];
   if (orderPtr->GetSide() == BID) {
      bids_[orderPtr->GetPrice()].erase(order_map_[uuid]);
      order_map_.erase(uuid);
   } else {
      asks_[orderPtr->GetPrice()].erase(order_map_[uuid]);
      order_map_.erase(uuid);
   }

   return true;
}
