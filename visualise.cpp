#include <functional>
#include <queue>

#include "HistogramRenderer.h"
#include "OrderBook.h"
#include "RandomOrderGenerator.h"

constexpr int64_t LOWEST_PRICE = 100;    // £1.00
constexpr int64_t HIGHEST_PRICE = 10000; // £100.00

constexpr int NUM_ORDERS_TO_ADD = 1000;

void DrawHistograms(HistogramRenderer &askHistogram, HistogramRenderer &bidHistogram,
                    HistogramRenderer &tradeHistogram) {
   if (!WindowShouldClose()) {
      BeginDrawing();
      ClearBackground(BACKGROUND_COLOUR);
      askHistogram.Draw();
      bidHistogram.Draw();
      tradeHistogram.Draw();
      EndDrawing();
   }
}

void DrawQuartilesText(PriceAndQuantityGenerator &tradeGenerator) {
   std::vector<int64_t> allPrices{};

   for (const auto [price, _]: tradeGenerator()) {
      allPrices.push_back(price);
   }
   if (allPrices.size() < 5) return;
   std::ranges::sort(allPrices);

   const int median_index = allPrices.size() / 2;
   const std::string median_text =
         std::format("Median Trade Price: £{}.{}", allPrices.at(median_index) / 100, allPrices.at(median_index) % 100);

   const int first_quartile_index = 1 * allPrices.size() / 4;
   const std::string first_quartile_text =
         std::format("First Quartile Trade Price: £{}.{}", allPrices.at(first_quartile_index) / 100,
                     allPrices.at(first_quartile_index) % 100);

   const int third_quartile_index = 3 * allPrices.size() / 4;
   const std::string third_quartile_text =
         std::format("Third Quartile Trade Price: £{}.{}", allPrices.at(third_quartile_index) / 100,
                     allPrices.at(third_quartile_index) % 100);

   const int TextX = (GetScreenWidth() * 0.05);
   const int TextY = (GetScreenHeight() * 0.7);
   constexpr int FontSize = 34;

   DrawText(first_quartile_text.c_str(), TextX, TextY, FontSize, BAR_TEXT_COLOUR);
   DrawText(median_text.c_str(), TextX, TextY + FontSize, FontSize, BAR_TEXT_COLOUR);
   DrawText(third_quartile_text.c_str(), TextX, TextY + 2 * FontSize, FontSize, BAR_TEXT_COLOUR);
}

int main() {
   SetConfigFlags(FLAG_WINDOW_RESIZABLE);
   InitWindow(0, 0, "Visualiser");

   int windowWidth = 3 * GetScreenWidth() / 4;
   int windowHeight = 3 * GetScreenHeight() / 4;
   SetWindowPosition(GetScreenWidth() / 2 - windowWidth / 2, GetScreenHeight() / 2 - windowHeight / 2);
   SetWindowSize(windowWidth, windowHeight);
   SetTargetFPS(120);

   OrderBook book = OrderBook();

   std::function<std::generator<PriceAndQuantity>()> askGenerator = [&book]()-> std::generator<PriceAndQuantity> {
      for (const auto &order: book.WalkOrders<Side::ASK>()) {
         co_yield PriceAndQuantity{
            order.GetPrice().GetPrice(),
            order.GetQuantity().GetQuantity()
         };
      }
   };
   std::function<std::generator<PriceAndQuantity>()> bidGenerator = [&book]()-> std::generator<PriceAndQuantity> {
      for (const auto &order: book.WalkOrders<Side::BID>()) {
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
                                         askGenerator, 15, "ASKS", true);

   auto bidHistogram = HistogramRenderer(LOWEST_PRICE, HIGHEST_PRICE,
                                         Dimensions{0.0, 0.05, 0.48, 0.5},
                                         bidGenerator, 15, "BIDS", true);

   auto tradeHistogram = HistogramRenderer(LOWEST_PRICE, HIGHEST_PRICE,
                                           Dimensions{0.40, 0.65, 0.20, 0.2},
                                           tradeGenerator, 10,  "TRADE PRICES", false);

   for (int i = 0; i < NUM_ORDERS_TO_ADD; i++) {
      Order randomOrder = randomOrderGenerator.GetRandomGoodTillCancelOrder();
      book.AddOrder(randomOrder);

      DrawHistograms(askHistogram, bidHistogram, tradeHistogram);
      DrawQuartilesText(tradeGenerator);
   }

   book.PrintBook();

   while (!WindowShouldClose()) {
      DrawHistograms(askHistogram, bidHistogram, tradeHistogram);
      DrawQuartilesText(tradeGenerator);
   }

   CloseWindow();
}
