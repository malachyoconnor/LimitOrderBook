#include "HistogramRenderer.h"
#include "OrderBook.h"
#include "RandomOrderGenerator.h"

constexpr int64_t LOWEST_PRICE = 100;
constexpr int64_t HIGHEST_PRICE = 10000;

constexpr int NUM_ORDERS_TO_ADD = 1000;

int main() {

   auto randomOrderGenerator = RandomOrderGenerator(LOWEST_PRICE, HIGHEST_PRICE, 100);
   auto book = OrderBook();
   auto vis = HistogramRenderer(LOWEST_PRICE, HIGHEST_PRICE, book);

   for (int i = 0; i < NUM_ORDERS_TO_ADD; i++) {
      Order randomOrder = randomOrderGenerator.GetRandomGoodTillCancelOrder();
      book.AddOrder(randomOrder);

      vis.Loop();
      std::cout << i << "\r";
      std::flush(std::cout);
   }

   book.PrintBook();

   while (!WindowShouldClose()) vis.Loop();


}
