#include <functional>
#include <iostream>

#include "HistogramRenderer.h"
#include "OrderBook.h"
#include "RandomOrderGenerator.h"

constexpr int64_t LOWEST_PRICE = 100; // £1.00
constexpr int64_t HIGHEST_PRICE = 10000; // £100.00

constexpr int NUM_ORDERS_TO_ADD = 1000;

void DrawHistograms(HistogramRenderer &askHistogram, HistogramRenderer &bidHistogram,
                    HistogramRenderer &tradeHistogram) {
   if (!WindowShouldClose()) {
      BeginDrawing();
      ClearBackground(BLACK);
      askHistogram.Draw();
      bidHistogram.Draw();
      tradeHistogram.Draw();
      EndDrawing();
   }
}

int main() {
   SetConfigFlags(FLAG_WINDOW_RESIZABLE);
   InitWindow(0, 0, "Visualiser");

   int windowWidth = 3 * GetScreenWidth() / 4;
   int windowHeight = 3 * GetScreenHeight() / 4;
   // Note: GetMonitorWidth and GetMonitorHeight were returning 0 on Windows.
   SetWindowPosition(GetScreenWidth() / 2 - windowWidth / 2, GetScreenHeight() / 2 - windowHeight / 2);
   SetWindowSize(windowWidth, windowHeight);
   SetTargetFPS(120);

   OrderBook book = OrderBook();

   std::function<std::generator<PriceAndQuantity>()> askGenerator = [&book]()-> std::generator<PriceAndQuantity> {
      for (const auto &order: book.WalkOrders<ASK>()) {
         co_yield PriceAndQuantity{
            order.GetPrice().GetPrice(),
            order.GetQuantity().GetQuantity()
         };
      }
   };
   std::function<std::generator<PriceAndQuantity>()> bidGenerator = [&book]()-> std::generator<PriceAndQuantity> {
      for (const auto &order: book.WalkOrders<BID>()) {
         co_yield PriceAndQuantity{
            order.GetPrice().GetPrice(),
            order.GetQuantity().GetQuantity()
         };
      }
   };

   std::function<std::generator<PriceAndQuantity>()> tradeGenerator = [&book]()-> std::generator<PriceAndQuantity> {
      for (const Trade &trade: book.WalkTrades()) {
         co_yield PriceAndQuantity{
            trade.GetPrice().GetPrice(),
            trade.GetQuantity().GetQuantity()
         };
      }
   };

   auto randomOrderGenerator = RandomOrderGenerator(LOWEST_PRICE, HIGHEST_PRICE, 100);

   auto askHistogram = HistogramRenderer(LOWEST_PRICE, HIGHEST_PRICE,
                                         Dimensions{0.51, 0.05, 0.48, 0.5},
                                         askGenerator, 20);

   auto bidHistogram = HistogramRenderer(LOWEST_PRICE, HIGHEST_PRICE,
                                         Dimensions{0.0, 0.05, 0.48, 0.5},
                                         bidGenerator, 20);

   auto tradeHistogram = HistogramRenderer(LOWEST_PRICE, HIGHEST_PRICE,
                                           Dimensions{0.40, 0.65, 0.20, 0.2},
                                           tradeGenerator, 10, false);

   for (int i = 0; i < NUM_ORDERS_TO_ADD; i++) {
      Order randomOrder = randomOrderGenerator.GetRandomGoodTillCancelOrder();
      book.AddOrder(randomOrder);

      DrawHistograms(askHistogram, bidHistogram, tradeHistogram);
   }

   book.PrintBook();

   while (!WindowShouldClose()) {
      DrawHistograms(askHistogram, bidHistogram, tradeHistogram);
   }


}
