#pragma once

#include <functional>
#include <generator>
#include <vector>

#include "raylib.h"
#include "visualiser_utils.h"
#include <OrderBook.h>

class Order;
using namespace Util;

constexpr Color BUCKET_GRID_COLOUR = RED;
constexpr Color BUCKET_BACKGROUND_COLOUR = BLACK;
constexpr Color BUCKET_FILL_COLOUR = GREEN;
constexpr Color BUCKET_INNER_HIGHLIGHT_COLOUR = WHITE;

constexpr int HISTOGRAM_VERTICAL_OFFSET = 70;
constexpr int HISTOGRAM_SEPARATION_DISTANCE = 50;

using PriceAndQuantityGenerator = std::function<std::generator<PriceAndQuantity>()>;

class HistogramRenderer {
public:
   HistogramRenderer(int lowestPrice, int highestPrice, Dimensions dimensions,
                     const PriceAndQuantityGenerator &priceAndQuantityGenerator,
                     int numberOfBuckets = 100, bool barsGrowDownwards = true)
      : lowestPrice_(lowestPrice),
        highestPrice_(highestPrice),
        dimensions_(dimensions),
        priceAndQuantityGenerator_(priceAndQuantityGenerator),
        numberOfBuckets_(numberOfBuckets),
        barsGrowDownwards_(barsGrowDownwards) {

      histogramWidth_ = static_cast<int>(GetScreenWidth() * dimensions_.width_percentage);
      histogramHeight_ = static_cast<int>(GetScreenHeight() * dimensions_.height_percentage);
      buckets_.reserve(static_cast<size_t>(dimensions.width_percentage * histogramWidth_) + 1);
   }

   void Draw() {
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
   PriceAndQuantityGenerator priceAndQuantityGenerator_;
   int numberOfBuckets_;
   bool barsGrowDownwards_;

   std::vector<double> buckets_;

   int histogramWidth_;
   int histogramHeight_;
   int histogramX_;
   int histogramY_;
};
