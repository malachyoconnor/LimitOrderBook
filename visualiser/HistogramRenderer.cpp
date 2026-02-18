#include "HistogramRenderer.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>

int HistogramRenderer::GetBucketIndexFromPrice(const int64_t price) const {
   if (!is_between_inclusive(price, lowestPrice_, highestPrice_))
      return -1;
   int bucket_value = static_cast<int>(highestPrice_ - lowestPrice_) / numberOfBuckets_;

   int bucket_index = static_cast<int>(price - lowestPrice_) / bucket_value;
   bucket_index = std::clamp(bucket_index, 0, numberOfBuckets_ - 1);

   return bucket_index;
}

void HistogramRenderer::AddPriceAndQuantity(const PriceAndQuantity &priceAndQuantity) {
   const int bucket_index = GetBucketIndexFromPrice(priceAndQuantity.price);
   assert(bucket_index != -1);

   buckets_[bucket_index] += priceAndQuantity.quantity;
   totalQuantity_ += priceAndQuantity.quantity;
   largestSingleQuantity_ = std::max(largestSingleQuantity_, buckets_[bucket_index]);

   assert(largestSingleQuantity_ >= 0);
   assert(totalQuantity_ >= 0);
}

void HistogramRenderer::RemovePriceAndQuantity(const PriceAndQuantity &priceAndQuantity) {
   const int bucket_index = GetBucketIndexFromPrice(priceAndQuantity.price);
   assert(bucket_index != -1);

   buckets_[bucket_index] -= priceAndQuantity.quantity;
   totalQuantity_ -= priceAndQuantity.quantity;
   largestSingleQuantity_ = std::reduce(buckets_.begin(), buckets_.end(), buckets_[0], std::ranges::max);

   assert(largestSingleQuantity_ >= 0);
   assert(totalQuantity_ >= 0);
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

      if (i < numberOfBuckets_ && totalQuantity_ > 0 && largestSingleQuantity_ > 0) {
         const double barHeightFrac = static_cast<double>(buckets_[i]) / static_cast<double>(totalQuantity_);
         int barHeight = barHeightFrac * histogramHeight_;

         if (static_cast<double>(largestSingleQuantity_) / static_cast<double>(totalQuantity_) < 0.7) {
            barHeight *= 0.7 / (static_cast<double>(largestSingleQuantity_) / static_cast<double>(totalQuantity_));
         }

         if (barsGrowDownwards_) {
            DrawRectangle(x + 1, y, bucketWidth - 1, barHeight, BAR_FILL_COLOUR);
         } else {
            int distanceFromZero = histogramHeight_ - barHeight;
            DrawRectangle(x + 1, y + distanceFromZero, bucketWidth - 1, barHeight, BAR_FILL_COLOUR);
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
      const double percentage = (100.0 * buckets_.at(bucket_index)) / totalQuantity_;

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
