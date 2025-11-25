#pragma once

enum class OrderType {
   GoodTillCancel, // Sticks around in the orderbook till the bitter end
   FillAndKill, // Fill as much as possible then kill
   FillOrKill, // If you can't fill everything, don't try
   // GoodForDay, // TODO
   Market, // Buy or sell a quantity at the highest or lowest level
};

static_assert(static_cast<int>(OrderType::Market) == 3);

namespace OrderTypes {
   inline constexpr int NUM_ORDER_TYPES = 1 + static_cast<int>(OrderType::Market);
}

inline std::string toString(const OrderType orderType) {
   switch (orderType) {
      case OrderType::GoodTillCancel:
         return "GoodTillCancel";
      case OrderType::FillAndKill:
         return "FillAndKill";
      case OrderType::FillOrKill:
         return "FillOrKill";
      // TODO:
      //case OrderType::GoodForDay:
      //   return "GoodForDay";
      case OrderType::Market:
         return "Market";
      default:
         assert(false);
   }
}
