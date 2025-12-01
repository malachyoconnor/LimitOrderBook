#pragma once

namespace Util {
   template <typename T>
   bool is_between_inclusive(T value, T lower, T upper) {
      return (value >= lower && value <= upper);
   }
}

struct Dimensions {
   double x_percentage;
   double y_percentage;
   double width_percentage;
   double height_percentage;
};

struct PriceAndQuantity {
   int64_t price;
   int64_t quantity;
};