# C++ Limit Order Book & Visualiser

A C++23 matching engine implementation featuring a real-time graphical visualiser built with Raylib.

## Features 🛠
* **Modern C++:** Uses C++23 `std::generator` (coroutines), `std::ranges`, and `std::format`.
* **Order Types:** Supports GoodTillCancel, FillAndKill, FillOrKill, and Market orders.
* **Matching Logic:** Price-Time priority (FIFO) using `std::map` and `std::list`.
* **Visualiser:** Real-time histograms for market depth (Bids/Asks) and trade frequency.
* **Analytics:** Live median and quartile trade price calculation.



## Technical Structure
* **OrderBook:** Manages two sorted maps (Bids descending, Asks ascending).
* **Order Tracking:** Uses an `unordered_map<Uuid, list::iterator>` for $O(1)$ cancellations.
* **Coroutines:** Uses `std::generator` to stream orders and trades without extra allocations.
* **Quantity/Price:** Dedicated classes using 64-bit integers to prevent floating-point errors.

## Visualiser
The GUI (built with Raylib) displays:
1. **Asks (Top):** Volume distribution of sell orders.
2. **Bids (Bottom):** Volume distribution of buy orders.
3. **Trades (Center):** Histogram of execution prices.
4. **Stats:** Live quartile data overlay based on historical trade volume.

## Building
**Requirements:**
* C++23 compatible compiler (GCC 13+, Clang 17+, MSVC 19.38+).
* Raylib 5.0+ installed on your system.

**Commands:**
```bash
mkdir build && cd build
cmake ..
cmake --build .