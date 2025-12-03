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
                              int numberOfBuckets = 20, std::string title = "", bool barsGrowDownwards = true)
      : lowestPrice_(lowestPrice),
        highestPrice_(highestPrice),
        dimensions_(dimensions),
        numberOfBuckets_(numberOfBuckets),
        title_(std::move(title)),
        barsGrowDownwards_(barsGrowDownwards) {

      buckets_.resize(numberOfBuckets);
      histogramWidth_ = static_cast<int>(GetScreenWidth() * dimensions_.width_percentage);
      histogramHeight_ = static_cast<int>(GetScreenHeight() * dimensions_.height_percentage);
      histogramX_ = static_cast<int>(GetScreenWidth() * dimensions_.x_percentage);
      histogramY_ = static_cast<int>(GetScreenHeight() * dimensions_.y_percentage);
   }

   void Draw() {
      histogramWidth_ = (static_cast<int>(GetScreenWidth() * dimensions_.width_percentage)
                         / numberOfBuckets_) * numberOfBuckets_;
      histogramHeight_ = static_cast<int>(GetScreenHeight() * dimensions_.height_percentage);
      histogramX_ = static_cast<int>(GetScreenWidth() * dimensions_.x_percentage);
      histogramY_ = static_cast<int>(GetScreenHeight() * dimensions_.y_percentage);
      if (!WindowShouldClose()) {
         DrawBuckets();
         DrawTextOverlay();
         DrawTitle();
      }
   }

   void AddPriceAndQuantity(const PriceAndQuantity &priceAndQuantity);
   void RemovePriceAndQuantity(const PriceAndQuantity &priceAndQuantity);

   int GetHistogramWidth() const { return histogramWidth_; }

private:
   void DrawBuckets() const;
   void DrawTextOverlay() const;
   void DrawTitle() const;
   bool MouseInsideHistogram() const;
   int GetBucketIndexFromPrice(int64_t price) const;

   int64_t lowestPrice_;
   int64_t highestPrice_;
   Dimensions dimensions_;
   int numberOfBuckets_;
   std::string title_;
   bool barsGrowDownwards_;

   std::vector<int64_t> buckets_;

   int histogramWidth_;
   int histogramHeight_;
   int histogramX_;
   int histogramY_;
   int64_t totalQuantity_;
   int64_t largestSingleQuantity_;
};
