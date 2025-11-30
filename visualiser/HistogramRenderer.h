#pragma once

#include <functional>
#include <vector>

#include "raylib.h"
#include "../visualiser_utils.h"
#include "../limitOrderBook/OrderBook.h"

using namespace Util;

constexpr Color BUCKET_GRID_COLOUR = RED;
constexpr Color BUCKET_BACKGROUND_COLOUR = BLACK;
constexpr Color BUCKET_FILL_COLOUR = GREEN;
constexpr Color BUCKET_INNER_HIGHLIGHT_COLOUR = WHITE;

constexpr int HISTOGRAM_VERTICAL_OFFSET = 70;
constexpr int HISTOGRAM_SEPARATION_DISTANCE = 50;

struct Dimensions {
   double x_percentage;
   double y_percentage;
   double width_percentage;
   double height_percentage;
};


class HistogramRenderer {
public:
   HistogramRenderer(int lowestPrice, int highestPrice, Dimensions dimensions,
                     std::function<std::generator<Order>()> orderGenerator,
                     int numberOfBuckets = 100)
      : lowestPrice_(lowestPrice),
        highestPrice_(highestPrice),
        dimensions_(dimensions),
        orderGenerator_(orderGenerator),
        numberOfBuckets_(numberOfBuckets) {

      histogramWidth_ = static_cast<int>(GetScreenWidth() * dimensions_.width_percentage);
      histogramHeight_ = static_cast<int>(GetScreenHeight() * dimensions_.height_percentage);
      buckets_.reserve(static_cast<size_t>(dimensions.width_percentage * histogramWidth_) + 1);
   }

   void Loop() {
      histogramWidth_ = static_cast<int>(GetScreenWidth() * dimensions_.width_percentage);
      histogramHeight_ = static_cast<int>(GetScreenHeight() * dimensions_.height_percentage);
      histogramX_ = static_cast<int>(GetScreenWidth() * dimensions_.x_percentage);
      histogramY_ = static_cast<int>(GetScreenHeight() * dimensions_.y_percentage);
      if (!WindowShouldClose()) {
         CalculateBucketFullness();
         DrawBuckets();

         DrawTextOverlay();
      }
   }

   void CalculateBucketFullness();
   void DrawBuckets() const;
   void DrawTextOverlay() const;

   int GetHistogramWidth() const { return histogramWidth_; }

   ~HistogramRenderer() {
      CloseWindow();
   }

private:
   bool MouseInsideHistogram() const;

   int64_t lowestPrice_;
   int64_t highestPrice_;
   Dimensions dimensions_;
   std::function<std::generator<Order>()> orderGenerator_;
   int numberOfBuckets_;

   std::vector<double> buckets_;

   int histogramWidth_;
   int histogramHeight_;
   int histogramX_;
   int histogramY_;
};
