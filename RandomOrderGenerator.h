#pragma once
#include <algorithm>
#include <random>
#include <array>
#include <Order.h>


class RandomOrderGenerator {
public:
   RandomOrderGenerator(int min_price, int max_price, int max_quantity)
      : price_distribution_(min_price, max_price),
        quantity_distribution_(0, max_quantity) {

      std::random_device rd;
      static auto seed_data = std::array<int, std::mt19937::state_size>{};

      std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
      static std::seed_seq seq(std::begin(seed_data), std::end(seed_data));

      generator_ = std::mt19937(seq);
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
