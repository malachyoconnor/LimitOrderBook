#pragma once
#include <random>
#include "Order.h"
#include "Random.h"


class RandomOrderGenerator {
public:
   RandomOrderGenerator(int min_price, int max_price, int max_quantity, std::mt19937 &generator)
      : price_distribution_(min_price, max_price),
        quantity_distribution_(0, max_quantity),
        generator_(generator) {
   }

   Order GetRandomOrder() {
      Uuid orderId = uuid();
      Price price = Price(price_distribution_(generator_));
      Quantity quantity = Quantity(quantity_distribution_(generator_));
      Side side = generator_() % 2 ? Side::BID : Side::ASK;
      OrderType orderType = static_cast<OrderType>(generator_() % OrderTypes::NUM_ORDER_TYPES);

      if (orderType == OrderType::Market) {
         return Order(orderId, quantity, side, orderType);
      }
      return Order(orderId, price, quantity, side, orderType);
   }

private:
   std::mt19937 &generator_;
   std::uniform_int_distribution<uint64_t> price_distribution_;
   std::uniform_int_distribution<uint64_t> quantity_distribution_;
};
