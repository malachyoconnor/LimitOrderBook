#ifndef LIMITORDERBOOK_TYPEDEFS_H
#define LIMITORDERBOOK_TYPEDEFS_H

#include <chrono>

using Timestamp = std::chrono::time_point<std::chrono::high_resolution_clock>;
using Clock = std::chrono::high_resolution_clock;
static_assert(std::same_as<std::chrono::time_point<Clock>, Timestamp>);

using Uuid = uint64_t;

#endif //LIMITORDERBOOK_TYPEDEFS_H
