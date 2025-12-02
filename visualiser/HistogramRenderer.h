#pragma once

#include <filesystem>
#include <functional>
#include <generator>
#include <utility>
#include <vector>

#include "raylib.h"
#include "visualiser_utils.h"

class Order;
using namespace Util;

const Font TEXT_FONT = LoadFont(FONT_LOCATION);
constexpr Color BACKGROUND_COLOUR = ColourFromHex(0x121212);

constexpr Color BAR_FILL_COLOUR = ColourFromHex(0x4CAF50);
constexpr Color BAR_TEXT_COLOUR = ColourFromHex(0xE0E0E0);
constexpr Color BUCKET_HIGHLIGHT_COLOUR = ColourFromHex(0x00BCD4);
constexpr Color GRID_COLOUR = ColourFromHex(0x666666);

constexpr Color HIGHLIGHT_TEXT_COLOUR = ColourFromHex(0xFF9800);
constexpr Color HIGHLIGHT_TEXT_BACKGROUND_COLOUR = WHITE;

constexpr int HISTOGRAM_VERTICAL_OFFSET = 70;
constexpr int HISTOGRAM_SEPARATION_DISTANCE = 50;

class HistogramRenderer {
public:
   explicit HistogramRenderer(int lowestPrice, int highestPrice, Dimensions dimensions,
                              PriceAndQuantityGenerator priceAndQuantityGenerator,
                              int numberOfBuckets = 20, std::string title = "", bool barsGrowDownwards = true)
      : lowestPrice_(lowestPrice),
        highestPrice_(highestPrice),
        dimensions_(dimensions),
        priceAndQuantityGenerator_(std::move(priceAndQuantityGenerator)),
        numberOfBuckets_(numberOfBuckets),
        title_(std::move(title)),
        barsGrowDownwards_(barsGrowDownwards) {

      histogramWidth_ = static_cast<int>(GetScreenWidth() * dimensions_.width_percentage);
      histogramHeight_ = static_cast<int>(GetScreenHeight() * dimensions_.height_percentage);
      histogramX_ = static_cast<int>(GetScreenWidth() * dimensions_.x_percentage);
      histogramY_ = static_cast<int>(GetScreenHeight() * dimensions_.y_percentage);
      buckets_.reserve(static_cast<size_t>(dimensions.width_percentage * histogramWidth_) + 1);
   }

   void Draw() {
      histogramWidth_ = (static_cast<int>(GetScreenWidth() * dimensions_.width_percentage)
                         / numberOfBuckets_) * numberOfBuckets_;
      histogramHeight_ = static_cast<int>(GetScreenHeight() * dimensions_.height_percentage);
      histogramX_ = static_cast<int>(GetScreenWidth() * dimensions_.x_percentage);
      histogramY_ = static_cast<int>(GetScreenHeight() * dimensions_.y_percentage);
      if (!WindowShouldClose()) {
         CalculateBucketFullness();
         DrawBuckets();
         DrawTextOverlay();
         DrawTitle();
      }
   }

   void CalculateBucketFullness();
   void DrawBuckets() const;
   void DrawTextOverlay() const;
   void DrawTitle() const;

   int GetHistogramWidth() const { return histogramWidth_; }

private:
   bool MouseInsideHistogram() const;

   int64_t lowestPrice_;
   int64_t highestPrice_;
   Dimensions dimensions_;
   PriceAndQuantityGenerator priceAndQuantityGenerator_;
   int numberOfBuckets_;
   std::string title_;
   bool barsGrowDownwards_;

   std::vector<double> buckets_;

   int histogramWidth_;
   int histogramHeight_;
   int histogramX_;
   int histogramY_;
};
