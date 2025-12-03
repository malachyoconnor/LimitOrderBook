#include "HistogramRenderer.h"

#include <algorithm>
#include <cassert>
#include <iostream>

void HistogramRenderer::CalculateBucketFullness() {
   const int bucket_value = static_cast<int>(highestPrice_ - lowestPrice_) / numberOfBuckets_;
   buckets_.resize(numberOfBuckets_);

   int64_t total_quantity = 0;
   double max_quantity = 0;
   std::ranges::fill(buckets_, 0);

   for (const auto &[price, quantity]: priceAndQuantityGenerator_()) {
      if (!is_between_inclusive(price, lowestPrice_, highestPrice_)) continue;

      int bucket_index = price / bucket_value;
      bucket_index = std::clamp(bucket_index, 0, numberOfBuckets_ - 1);

      buckets_.at(bucket_index) += static_cast<double>(quantity);
      total_quantity += quantity;
      max_quantity = std::max(max_quantity, buckets_.at(bucket_index));
   }

   if (total_quantity == 0) return;

   double multiplier = 1;
   const double largest_fraction = max_quantity / total_quantity;
   if (largest_fraction < 0.7) {
      multiplier = 0.7 / largest_fraction;
   }

   for (int i = 0; i < numberOfBuckets_; i++) {
      buckets_.at(i) /= static_cast<double>(total_quantity);
      buckets_.at(i) *= multiplier;
      assert(buckets_.at(i) <= 1.0);
   }
}

void HistogramRenderer::DrawBuckets() const {
   const int bucketWidth = histogramWidth_ / numberOfBuckets_;

   DrawLine(histogramX_, histogramY_, histogramX_ + histogramWidth_ - 1, histogramY_, GRID_COLOUR);
   DrawLine(histogramX_, histogramY_ + histogramHeight_ - 1, histogramX_ + histogramWidth_ - 1,
            histogramY_ + histogramHeight_ - 1, GRID_COLOUR);

   for (int i = 0; i <= numberOfBuckets_; i++) {
      const int x = histogramX_ + (i * bucketWidth);
      const int y = histogramY_;

      // Start by drawing the bucket grid. If we have 10 buckets, we need to draw 11 lines.
      DrawLine(x, y, x, y + histogramHeight_, GRID_COLOUR);
      if (i < numberOfBuckets_) {
         if (barsGrowDownwards_) {
            DrawRectangle(x + 1, y, bucketWidth - 1, histogramHeight_ * buckets_[i], BAR_FILL_COLOUR);
         } else {
            int distanceFromZero = histogramHeight_ * (1 - buckets_[i]);
            DrawRectangle(x + 1, y + distanceFromZero, bucketWidth - 1, histogramHeight_ * buckets_[i],
                          BAR_FILL_COLOUR);
         }
      }
   }

   if (MouseInsideHistogram() && (GetMouseX() <= numberOfBuckets_ * bucketWidth + histogramX_ + histogramWidth_)) {
      const int mouseX = GetMouseX();

      int bucket_index = (mouseX - histogramX_) / bucketWidth;
      const int x = histogramX_ + (bucket_index * bucketWidth);
      const int y = histogramY_;
      DrawRectangle(x + 1, y, bucketWidth - 1, histogramHeight_, BUCKET_HIGHLIGHT_COLOUR);
   }
}

void HistogramRenderer::DrawTextOverlay() const {
   const int mouseX = GetMouseX();
   const int mouseY = GetMouseY();
   const int bucketWidth = histogramWidth_ / numberOfBuckets_;
   const int bucket_value = static_cast<int>(highestPrice_ - lowestPrice_) / numberOfBuckets_;

   for (int i = 0; i < numberOfBuckets_; i++) {
      const int textX = histogramX_ + 4 + (i * bucketWidth);
      const int textY = histogramY_ + histogramHeight_;

      const int64_t interval_start_price = lowestPrice_ + i * bucket_value;
      std::string text = std::format("£{} ", interval_start_price / 100);

      const int offset = (bucketWidth - MeasureText(text.c_str(), 18)) / 2;

      DrawTextEx(TEXT_FONT, text.c_str(), Vector2(textX + offset, textY), 18, 1, BAR_TEXT_COLOUR);
   }

   if (MouseInsideHistogram() && (mouseX <= numberOfBuckets_ * bucketWidth + histogramX_ + histogramWidth_)) {
      const int bucket_index = (mouseX - histogramX_) / bucketWidth;

      const int64_t interval_start_price = lowestPrice_ + bucket_index * bucket_value;
      const int64_t interval_end_price = lowestPrice_ + (bucket_index + 1) * bucket_value;
      const double percentage = 100 * buckets_.at(bucket_index);

      const std::string text = std::format("{:.2}% ${}.{:02}-£{}.{:02}", percentage,
                                           interval_start_price / 100, interval_start_price % 100,
                                           interval_end_price / 100, interval_end_price % 100);

      int textX = mouseX + 20;
      const int textY = mouseY + 20;
      constexpr int fontSize = 35;

      const int boxHeight = 35 + 10;
      const int boxWidth = MeasureText(text.c_str(), 35) + 10;

      if (boxWidth + textX >= GetScreenWidth()) {
         textX -= boxWidth + textX - GetScreenWidth() + 5;
      }

      DrawRectangle(textX - 5, textY - 5, boxWidth, boxHeight, HIGHLIGHT_TEXT_BACKGROUND_COLOUR);
      DrawTextEx(TEXT_FONT, text.c_str(), Vector2(textX, textY), fontSize, 1, HIGHLIGHT_TEXT_COLOUR);
   }
}

void HistogramRenderer::DrawTitle() const {
   if (title_.empty()) return;

   constexpr int fontSize = 35;
   const int titleSize = MeasureText(title_.c_str(), fontSize);

   const int titleX = histogramX_ + (histogramWidth_ - titleSize) / 2;
   const int titleY = histogramY_ - fontSize;

   DrawTextEx(TEXT_FONT, title_.c_str(), Vector2(titleX, titleY), fontSize, 5, BAR_FILL_COLOUR);
}

bool HistogramRenderer::MouseInsideHistogram() const {
   return is_between_inclusive(GetMouseX(), histogramX_ + 1, histogramX_ + histogramWidth_ - 1)
          && is_between_inclusive(GetMouseY(), histogramY_, histogramY_ + histogramHeight_);
}
