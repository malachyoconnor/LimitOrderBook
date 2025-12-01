#pragma once

namespace Util {
   template<typename T>
   bool is_between_inclusive(T value, T lower, T upper) {
      return (value >= lower && value <= upper);
   }
}

struct PriceAndQuantity;
using PriceAndQuantityGenerator = std::function<std::generator<PriceAndQuantity>()>;

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

constexpr Color ColourFromHex(const uint64_t hexColour) {
   constexpr uint8_t mask = 0b11111111;
   return Color{
      static_cast<unsigned char>(hexColour >> 16 & mask),
      static_cast<unsigned char>(hexColour >> 8 & mask),
      static_cast<unsigned char>(hexColour & mask),
      255
   };
}
