#include "HistogramRenderer.h"

void HistogramRenderer::CalculateBucketFullness() {
   int bucket_value = (highestPrice_ - lowestPrice_) / numberOfBuckets_;
   buckets_.resize(numberOfBuckets_);

   int64_t total_quantity = 0;
   std::fill(buckets_.begin(), buckets_.end(), 0);

   for (auto order: orderGenerator_()) {
      const int64_t price = order.GetPrice().GetPrice();
      int bucket_index = price / bucket_value;
      bucket_index = std::clamp(bucket_index, 0, numberOfBuckets_ - 1);

      const auto quantity = order.GetQuantity().GetQuantity();
      buckets_.at(bucket_index) += static_cast<double>(quantity);
      total_quantity += quantity;
   }

   if (total_quantity == 0) return;

   for (int i = 0; i < numberOfBuckets_; i++) {
      buckets_.at(i) /= static_cast<double>(total_quantity);
   }
}

void HistogramRenderer::DrawBuckets() const {
   const int bucketWidth = histogramWidth_ / numberOfBuckets_;

   for (int i = 0; i <= numberOfBuckets_; i++) {
      const int x = histogramX_ + (i * bucketWidth);
      const int y = histogramY_;

      // Start by drawing the bucket grid. If we have 10 buckets, we need to draw 11 lines.
      DrawLine(x, y, x, y + histogramHeight_, BUCKET_GRID_COLOUR);
      if (i < numberOfBuckets_) {
         DrawRectangle(x + 1, y, bucketWidth - 1, histogramHeight_, BUCKET_BACKGROUND_COLOUR);

         DrawRectangle(x + 1, y, bucketWidth - 1, histogramHeight_ * buckets_[i], BUCKET_FILL_COLOUR);
      }
   }

   if (MouseInsideHistogram() && (GetMouseX() <= numberOfBuckets_ * bucketWidth + histogramX_ + histogramWidth_)) {
      const int mouseX = GetMouseX();

      int bucket_index = (mouseX - histogramX_) / bucketWidth;
      const int x = histogramX_ + (bucket_index * bucketWidth);
      const int y = histogramY_;
      DrawRectangle(x + 1, y, bucketWidth - 1, histogramHeight_, BUCKET_INNER_HIGHLIGHT_COLOUR);
   }
}

void HistogramRenderer::DrawTextOverlay() const {
   const int mouseX = GetMouseX();
   const int mouseY = GetMouseY();
   const int bucketWidth = histogramWidth_ / numberOfBuckets_;

   if (MouseInsideHistogram() && (mouseX <= numberOfBuckets_ * bucketWidth + histogramX_ + histogramWidth_)) {
      const int bucket_index = (mouseX - histogramX_) / bucketWidth;
      const int bucket_value = (highestPrice_ - lowestPrice_) / numberOfBuckets_;
      std::string text = std::format("${}-£{}", (bucket_index * bucket_value) / 100,
                                     ((bucket_index + 1) * bucket_value) / 100);

      DrawText(text.c_str(), mouseX, mouseY, 35, BLUE);
   }
}

bool HistogramRenderer::MouseInsideHistogram() const {
   return is_between_inclusive(GetMouseX(), histogramX_, histogramX_ + histogramWidth_ - 1)
          && is_between_inclusive(GetMouseY(), histogramY_, histogramY_ + histogramHeight_);
}
