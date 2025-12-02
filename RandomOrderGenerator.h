#pragma once
#include <algorithm>
#include <random>
#include <array>
#include <Order.h>


class RandomOrderGenerator {
public:
   using enum Side;
   using enum OrderType;

   RandomOrderGenerator(int min_price, int max_price, int average_quantity)
      : price_distribution_((min_price + max_price) / 2, 5000),
        quantity_distribution_(average_quantity, 5),
        min_price_(min_price),
        max_price_(max_price) {

      std::random_device rd;
      static auto seed_data = std::array<int, std::mt19937::state_size>{};

      std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
      static std::seed_seq seq(std::begin(seed_data), std::end(seed_data));

      generator_ = std::mt19937(seq);
   }

   Order GetRandomGoodTillCancelOrder() {
      Uuid orderId = generator_();
      Price price = Price(std::clamp(static_cast<int64_t>(price_distribution_(generator_)), min_price_, max_price_));
      Quantity quantity = Quantity(std::clamp(static_cast<int64_t>(quantity_distribution_(generator_)), 0L, 10000000L));
      Side side = generator_() % 2 ? BID : ASK;
      OrderType orderType = GoodTillCancel;

      return Order::NewOrder(orderId, price, quantity, side, orderType);
   }

   Order GetRandomOrder() {
      Uuid orderId = generator_();
      Price price = Price(std::clamp(static_cast<int64_t>(price_distribution_(generator_)), min_price_, max_price_));
      Quantity quantity = Quantity(std::clamp(static_cast<int64_t>(quantity_distribution_(generator_)), 0L, 10000000L));
      Side side = generator_() % 2 ? BID : ASK;
      OrderType orderType = static_cast<OrderType>(generator_() % OrderTypes::NUM_ORDER_TYPES);

      if (orderType == Market) {
         return Order::NewMarketOrder(orderId, quantity, side);
      }
      return Order::NewOrder(orderId, price, quantity, side, orderType);
   }

private:
   std::mt19937 generator_;
   std::normal_distribution<double> price_distribution_;
   std::normal_distribution<double> quantity_distribution_;
   int64_t min_price_;
   int64_t max_price_;
};
