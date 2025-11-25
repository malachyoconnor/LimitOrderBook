#include "HistogramRenderer.h"

#include <numeric>


void HistogramRenderer::CalculateBucketFullness() {
   CalculateBucketFullness<BID>();
   CalculateBucketFullness<ASK>();
}

void HistogramRenderer::DrawBuckets() const {
   DrawBuckets<BID>();
   DrawBuckets<ASK>();
}

template<Side Side_>
const std::vector<double> &HistogramRenderer::GetCorrectBucketVector() const {
   if constexpr (Side_ == ASK) {
      return ask_buckets_fill_percentage_;
   } else {
      return bid_buckets_fill_percentage_;
   }
}

template<Side Side_>
std::vector<double> &HistogramRenderer::GetCorrectBucketVector() {
   if constexpr (Side_ == ASK) {
      return ask_buckets_fill_percentage_;
   } else {
      return bid_buckets_fill_percentage_;
   }
}

template<Side Side_>
void HistogramRenderer::CalculateBucketFullness() {
   int num_buckets = GetTotalNumberOfBuckets();
   auto &buckets_vector = GetCorrectBucketVector<Side_>();

   buckets_vector.resize(num_buckets);
   int bucket_value = (highestPrice_ - lowestPrice_) / num_buckets;

   for (Order order: orderBook_.orderGenerator<Side_>()) {
      assert(is_between_inclusive(order.GetPrice().GetPrice(), lowestPrice_, highestPrice_));
      int bucket_index = order.GetPrice().GetPrice() / bucket_value;
      buckets_vector[bucket_index] += order.GetQuantity().GetQuantity();
   }

   double total_quantity = std::reduce(buckets_vector.begin(), buckets_vector.end(), 0.0);

   if (total_quantity == 0) {
      std::ranges::fill(buckets_vector, 0);
      return;
   }

   for (double &bucket_percentage: buckets_vector) {
      bucket_percentage /= total_quantity;
      assert(is_between_inclusive(bucket_percentage, 0.0, 1.0));
   }
}

template<Side Side_>
void HistogramRenderer::DrawBuckets() const {
   int single_histogram_buckets = GetTotalNumberOfBuckets() / 2;
   auto [bucket_width, bucket_height] = GetSingleBucketWidthAndHeight();
   int inner_width = bucket_width - 2;

   auto &bucket_vector = GetCorrectBucketVector<Side_>();

   for (int i = 0; i <= single_histogram_buckets; i++) {
      auto [bucket_x, bucket_y] = GetBucketPosition(i);

      if constexpr (Side_ == ASK) {
         bucket_x += HISTOGRAM_SEPARATION_DISTANCE + (bucket_width * single_histogram_buckets);
      }

      DrawRectangle(bucket_x, bucket_y, bucket_width, bucket_height, BUCKET_OUTLINE_COLOUR);

      auto inner_bucket_colour = IsMouseInBucket(i) ? BUCKET_INNER_HIGHLIGHT_COLOUR : BUCKET_INNER_COLOUR;
      DrawRectangle(bucket_x + 1, bucket_y + 1, inner_width, bucket_height - 2, inner_bucket_colour);

      if (int num_pixels = bucket_vector[i] * (bucket_height - 2); num_pixels > 0) {
         DrawRectangle(bucket_x, bucket_y + 1, inner_width, num_pixels, GREEN);
      }
   }
}

void HistogramRenderer::DrawTextOverlay() const {
   int num_buckets = GetTotalNumberOfBuckets();
   for (int i = 0; i <= num_buckets; i++) {
      if (IsMouseInBucket(i)) {
         int bucket_value = (highestPrice_ - lowestPrice_) / num_buckets / 100;

         auto hover_text = std::format("£{} - £{}", i * bucket_value, (i + 1) * bucket_value);
         DrawText(hover_text.c_str(), GetMouseX(), GetMouseY(), 30, SKYBLUE);
      }
   }
}

int HistogramRenderer::GetTotalNumberOfBuckets() const {
   int num_buckets = (highestPrice_ - lowestPrice_) * 2;
   int max_bucket_width = (windowWidth_ - HISTOGRAM_SEPARATION_DISTANCE) / num_buckets;
   while (max_bucket_width < 2) {
      num_buckets /= 10;
      max_bucket_width = (windowWidth_ - HISTOGRAM_SEPARATION_DISTANCE) / num_buckets;
   }
   assert(num_buckets > 1);
   return num_buckets;
}

std::pair<int, int> HistogramRenderer::GetSingleBucketWidthAndHeight() const {
   int bucket_width = (windowWidth_ - HISTOGRAM_SEPARATION_DISTANCE) / GetTotalNumberOfBuckets();
   int bucket_height = windowHeight_ - HISTOGRAM_VERTICAL_OFFSET;

   assert(bucket_width > 2);
   return {bucket_width, bucket_height};
}

std::pair<int, int> HistogramRenderer::GetBucketPosition(int bucket_index) const {
   int single_bucket_width = GetSingleBucketWidthAndHeight().first;

   int total_buckets = GetTotalNumberOfBuckets();
   int gap_size = (windowWidth_ - total_buckets * single_bucket_width )/ 3;

   int outline_x = single_bucket_width * bucket_index + gap_size;

   return {outline_x, HISTOGRAM_VERTICAL_OFFSET / 2};
}

bool HistogramRenderer::IsMouseInBucket(int bucket_index) const {
   int mouse_x = GetMouseX();
   int mouse_y = GetMouseY();

   auto [bucket_x, bucket_y] = GetBucketPosition(bucket_index);
   auto [bucket_width, bucket_height] = GetSingleBucketWidthAndHeight();

   return is_between_inclusive(mouse_x, bucket_x, bucket_x + bucket_width) &&
          is_between_inclusive(mouse_y, bucket_y, bucket_y + bucket_height);
}
