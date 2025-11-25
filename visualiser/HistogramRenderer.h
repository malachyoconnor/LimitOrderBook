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

constexpr int HISTOGRAM_VERTICAL_OFFSET = 70;
constexpr int HISTOGRAM_SEPARATION_DISTANCE = 50;

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
      windowWidth_ = 3 * GetMonitorWidth(monitor) / 4;
      windowHeight_ = 3 * GetMonitorHeight(monitor) / 4;

      ask_buckets_fill_percentage_.reserve(highestPrice_ - lowestPrice_ + 1);
      bid_buckets_fill_percentage_.reserve(highestPrice_ - lowestPrice_ + 1);

      SetWindowSize(windowWidth_, windowHeight_);
      SetWindowPosition(GetMonitorWidth(monitor) / 2 - windowWidth_ / 2,
                        GetMonitorHeight(monitor) / 2 - windowHeight_ / 2);
      SetTargetFPS(120);
   }

   void Loop() {
      windowWidth_ = GetScreenWidth();
      windowHeight_ = GetScreenHeight();
      if (!WindowShouldClose()) {
         BeginDrawing();
         ClearBackground(BLACK);

         CalculateBucketFullness();
         DrawBuckets();

         DrawTextOverlay();

         EndDrawing();
      }
   }

   void CalculateBucketFullness();
   void DrawBuckets() const;
   void DrawTextOverlay() const;

   ~HistogramRenderer() {
      CloseWindow();
   }

private:
   int64_t lowestPrice_;
   int64_t highestPrice_;
   OrderBook &orderBook_;
   int windowWidth_;
   int windowHeight_;
   std::vector<double> bid_buckets_fill_percentage_{};
   std::vector<double> ask_buckets_fill_percentage_{};

   int GetTotalNumberOfBuckets() const;
   std::pair<int, int> GetSingleBucketWidthAndHeight() const;
   std::pair<int, int> GetBucketPosition(int bucket_index) const;
   bool IsMouseInBucket(int bucket_index) const;

   template<Side Side_>
   const std::vector<double> &GetCorrectBucketVector() const;

   template<Side Side_>
   std::vector<double> &GetCorrectBucketVector();

   template<Side Side_>
   void CalculateBucketFullness();

   template<Side Side_>
   void DrawBuckets() const;
};
