#ifndef LIMITORDERBOOK_RANDOM_H
#define LIMITORDERBOOK_RANDOM_H
#include <random>

#include "typedefs.h"

inline namespace Random {
   static std::mt19937 get_random_generator() {
      std::random_device rd;
      static auto seed_data = std::array<int, std::mt19937::state_size>{};

      std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
      static std::seed_seq seq(std::begin(seed_data), std::end(seed_data));

      std::mt19937 generator(seq);
      return generator;
   }

   static std::mt19937 random_generator_ = get_random_generator();

   inline Uuid uuid() {
      return random_generator_();
   }
}


#endif //LIMITORDERBOOK_RANDOM_H
