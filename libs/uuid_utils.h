#ifndef LIMITORDERBOOK_UUID_UTILS_H
#define LIMITORDERBOOK_UUID_UTILS_H

#include <cassert>

#include "uuid.h"

static_assert(sizeof(std::byte) == sizeof(char));

namespace uuid_util {
   inline std::string to_string(const uuids::uuid &uuid) {
      std::stringstream ss{};
      ss << uuid;
      return ss.str();
   }
}

#endif //LIMITORDERBOOK_UUID_UTILS_H
