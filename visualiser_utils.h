#pragma once

namespace Util {
   template <typename T>
   bool is_between_inclusive(T value, T lower, T upper) {
      return (value >= lower && value <= upper);
   }
}