#include "HistogramRenderer.h"

#include <numeric>

void HistogramRenderer::CalculateBucketFullness() {
   int num_buckets = GetNumberOfBuckets();
   buckets_fill_percentage_.resize(num_buckets);

   int bucket_value = (highestPrice_ - lowestPrice_) / num_buckets;

   for (Order order: orderBook_.orderGenerator<BID>()) {
      assert(is_between_inclusive(order.GetPrice().GetPrice(), lowestPrice_, highestPrice_));

      int bucket_index = order.GetPrice().GetPrice() / bucket_value;

      buckets_fill_percentage_[bucket_index] += order.GetQuantity().GetQuantity();
   }

   double total_quantity = std::reduce(buckets_fill_percentage_.begin(), buckets_fill_percentage_.end(), 0.0);

   if (total_quantity == 0) {
      std::ranges::fill(buckets_fill_percentage_, 0);
      return;
   }

   for (double &bucket_percentage: buckets_fill_percentage_) {
      assert(is_between_inclusive(bucket_percentage/total_quantity, 0.0, 1.0));

      bucket_percentage /= total_quantity;
   }
}

void HistogramRenderer::DrawBuckets() {
   int num_buckets = GetNumberOfBuckets();
   auto [bucket_width, bucket_height] = GetBucketWidthAndHeight();
   int inner_width = bucket_width - 2;

   for (int i = 0; i <= num_buckets; i++) {
      auto [bucket_x, bucket_y] = GetBucketPosition(i);

      DrawRectangle(bucket_x, bucket_y, bucket_width, bucket_height, BUCKET_OUTLINE_COLOUR);

      auto inner_bucket_colour = IsMouseInBucket(i) ? BUCKET_INNER_HIGHLIGHT_COLOUR : BUCKET_INNER_COLOUR;
      DrawRectangle(bucket_x + 1, bucket_y + 1, inner_width, bucket_height - 2, inner_bucket_colour);

      if (int num_pixels = buckets_fill_percentage_[i] * (bucket_height - 2); num_pixels > 0) {
         DrawRectangle(bucket_x, bucket_y + 1, inner_width, num_pixels, GREEN);
      }
   }
}

void HistogramRenderer::DrawTextOverlay() {
   int num_buckets = GetNumberOfBuckets();
   for (int i = 0; i <= num_buckets; i++) {
      if (IsMouseInBucket(i)) {
         int bucket_value = (highestPrice_ - lowestPrice_) / num_buckets / 100;

         auto hover_text = std::format("£{} - £{}", i * bucket_value, (i + 1) * bucket_value);
         DrawText(hover_text.c_str(), GetMouseX(), GetMouseY(), 30, SKYBLUE);
      }
   }
}

int HistogramRenderer::GetNumberOfBuckets() const {
   int num_buckets = highestPrice_ - lowestPrice_;
   int max_bucket_width = GetScreenWidth() / (num_buckets);
   while (max_bucket_width < 2) {
      num_buckets /= 10;
      max_bucket_width = GetScreenWidth() / (num_buckets);
   }
   assert(num_buckets > 1);
   return num_buckets;
}

std::pair<int, int> HistogramRenderer::GetBucketWidthAndHeight() const {
   int bucket_width = GetScreenWidth() / GetNumberOfBuckets();
   assert(bucket_width > 2);
   int bucket_height = GetScreenHeight() - BUCKET_VERTICAL_OFFSET;
   return {bucket_width, bucket_height};
}

std::pair<int, int> HistogramRenderer::GetBucketPosition(int bucket_index) const {
   int num_buckets = GetNumberOfBuckets();
   auto [outline_width, _] = GetBucketWidthAndHeight();
   int histogram_width = outline_width * num_buckets;
   int outline_x = (GetScreenWidth() - histogram_width) / 2 + bucket_index * outline_width;
   return {outline_x, BUCKET_VERTICAL_OFFSET / 2};
}

bool HistogramRenderer::IsMouseInBucket(int bucket_index) const {
   int mouse_x = GetMouseX();
   int mouse_y = GetMouseY();

   auto [bucket_x, bucket_y] = GetBucketPosition(bucket_index);
   auto [bucket_width, bucket_height] = GetBucketWidthAndHeight();

   return is_between_inclusive(mouse_x, bucket_x, bucket_x + bucket_width) &&
          is_between_inclusive(mouse_y, bucket_y, bucket_y + bucket_height);
}
