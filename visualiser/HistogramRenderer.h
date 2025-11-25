#pragma once

#include <cassert>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "raylib.h"
#include "../visualiser_utils.h"
#include "../limitOrderBook/OrderBook.h"

using namespace Util;

constexpr Color BUCKET_OUTLINE_COLOUR = RED;
constexpr Color BUCKET_INNER_COLOUR = BLACK;
constexpr Color BUCKET_INNER_HIGHLIGHT_COLOUR = WHITE;

constexpr int BUCKET_VERTICAL_OFFSET = 70;

// TEMP TEMP
inline double random_normalised_double() {
   return rand() / (RAND_MAX + 1.0);
}


class HistogramRenderer {
public:
   HistogramRenderer(int lowestPrice, int highestPrice, OrderBook &orderBook)
      : lowestPrice_(lowestPrice),
        highestPrice_(highestPrice),
        orderBook_(orderBook) {

      SetConfigFlags(FLAG_WINDOW_RESIZABLE);
      InitWindow(400, 400, "Visualiser");
      int monitor = GetCurrentMonitor();
      int initialWidth = 3 * GetMonitorWidth(monitor) / 4;
      int initialHeight = 3 * GetMonitorHeight(monitor) / 4;
      buckets_fill_percentage_.reserve(highestPrice_ - lowestPrice_ + 1);

      SetWindowSize(initialWidth, initialHeight);
      SetWindowPosition(GetMonitorWidth(monitor) / 2 - initialWidth / 2,
                        GetMonitorHeight(monitor) / 2 - initialHeight / 2);
      SetTargetFPS(120);
   }

   void Loop() {
      if (!WindowShouldClose()) {
         BeginDrawing();
         ClearBackground(BLACK);

         // TMP
         CalculateBucketFullness();
         DrawBuckets();
         DrawTextOverlay();

         EndDrawing();
      }
   }

   void CalculateBucketFullness();
   void DrawBuckets();
   void DrawTextOverlay();

   ~HistogramRenderer() {
      CloseWindow();
   }

private:
   int64_t lowestPrice_;
   int64_t highestPrice_;
   OrderBook &orderBook_;
   std::vector<double> buckets_fill_percentage_{};

   int GetNumberOfBuckets() const;
   std::pair<int, int> GetBucketWidthAndHeight() const;
   std::pair<int, int> GetBucketPosition(int bucket_index) const;
   bool IsMouseInBucket(int bucket_index) const;
};
