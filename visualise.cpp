#include <functional>

#include "HistogramRenderer.h"
#include "OrderBook.h"
#include "RandomOrderGenerator.h"

constexpr int64_t LOWEST_PRICE = 100;
constexpr int64_t HIGHEST_PRICE = 10000;

constexpr int NUM_ORDERS_TO_ADD = 1000;

void DrawHistograms(HistogramRenderer &askHistogram, HistogramRenderer &bidHistogram) {
   if (!WindowShouldClose()) {
      BeginDrawing();
      ClearBackground(BLACK);
      askHistogram.Loop();
      bidHistogram.Loop();
      EndDrawing();
   }
}

int main() {

   int monitor = GetCurrentMonitor();
   int windowWidth = 3 * GetMonitorWidth(monitor) / 4;
   int windowHeight = 3 * GetMonitorHeight(monitor) / 4;

   SetConfigFlags(FLAG_WINDOW_RESIZABLE);
   InitWindow(windowWidth, windowHeight, "Visualiser");
   SetTargetFPS(120);

   OrderBook book = OrderBook();
   auto askGenerator = [&book]() { return book.orderGenerator<ASK>(); };
   auto bidGenerator = [&book]() { return book.orderGenerator<BID>(); };
   auto randomOrderGenerator = RandomOrderGenerator(LOWEST_PRICE, HIGHEST_PRICE, 100);

   auto bidHistogram = HistogramRenderer(LOWEST_PRICE, HIGHEST_PRICE,
                                         Dimensions{0.0, 0.05, 0.48, 0.5},
                                         bidGenerator, 30);

   auto askHistogram = HistogramRenderer(LOWEST_PRICE, HIGHEST_PRICE,
                                         Dimensions{0.51, 0.05, 0.48, 0.5},
                                         askGenerator, 30);

   for (int i = 0; i < NUM_ORDERS_TO_ADD; i++) {
      Order randomOrder = randomOrderGenerator.GetRandomGoodTillCancelOrder();
      book.AddOrder(randomOrder);

      DrawHistograms(askHistogram, bidHistogram);
   }

   book.PrintBook();

   while (!WindowShouldClose()) {
      DrawHistograms(askHistogram, bidHistogram);
   }


}
