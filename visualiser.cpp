#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

#include "Random.h"
#include "raylib.h"
#include "visualiser_utils.h"

using namespace Util;

constexpr Color BUCKET_OUTLINE_COLOUR = RED;
constexpr Color BUCKET_INNER_COLOUR = BLACK;
constexpr Color BUCKET_INNER_HIGHLIGHT_COLOUR = WHITE;

constexpr int BUCKET_VERTICAL_OFFSET = 70;

class Visualiser {
public:
   Visualiser(int lowestPrice = 100, int highestPrice = 10000) : lowestPrice_(lowestPrice), highestPrice_(highestPrice) {
      SetConfigFlags(FLAG_WINDOW_RESIZABLE);
      InitWindow(400, 400, "Visualiser");
      int monitor = GetCurrentMonitor();
      int initialWidth = 3 * GetMonitorWidth(monitor) / 4;
      int initialHeight = 3 * GetMonitorHeight(monitor) / 4;
      buckets_fill_percentage_.reserve(highestPrice_ - lowestPrice_ + 1);

      SetWindowSize(initialWidth, initialHeight);
      SetWindowPosition(GetMonitorWidth(monitor) / 2 - initialWidth / 2,
                        GetMonitorHeight(monitor) / 2 - initialHeight / 2);
      SetTargetFPS(60);
   }

   void Loop() {
      while (!WindowShouldClose()) {
         BeginDrawing();
         ClearBackground(BLACK);

         // TMP
         if (random_normalised_double() < 0.01) {
            CalculateBucketFullness();
         }
         DrawBuckets();
         DrawTextOverlay();

         EndDrawing();
      }
   }

   void CalculateBucketFullness() {
      int num_buckets = GetNumberOfBuckets();
      buckets_fill_percentage_.resize(num_buckets);

      for (int i = 0; i <= num_buckets; i++) {
         buckets_fill_percentage_[i] = random_normalised_double();
      }
   }


   void DrawBuckets() {
      int num_buckets = GetNumberOfBuckets();
      auto [bucket_width, bucket_height] = GetBucketWidthAndHeight();
      int inner_width = bucket_width - 2;

      for (int i = 0; i <= num_buckets; i++) {
         auto [bucket_x, bucket_y] = GetBucketPosition(i);

         DrawRectangle(bucket_x, bucket_y, bucket_width, bucket_height, BUCKET_OUTLINE_COLOUR);

         auto inner_bucket_colour = MouseInBucket(i) ? BUCKET_INNER_HIGHLIGHT_COLOUR : BUCKET_INNER_COLOUR;
         DrawRectangle(bucket_x + 1, bucket_y + 1, inner_width, bucket_height - 2, inner_bucket_colour);

         if (int num_pixels = buckets_fill_percentage_[i] * (bucket_height - 2); num_pixels > 0) {
            DrawRectangle(bucket_x, bucket_y + 1, inner_width, num_pixels, GREEN);
         }
      }
   }

   void DrawTextOverlay() {
      int num_buckets = GetNumberOfBuckets();
      for (int i = 0; i <= num_buckets; i++) {
         if (MouseInBucket(i)) {
            int bucket_value = (highestPrice_ - lowestPrice_) / num_buckets / 100;

            auto hover_text = std::format("£{} - £{}", i * bucket_value , (i+1) * bucket_value);
            DrawText(hover_text.c_str(), GetMouseX(), GetMouseY(), 30, RED);
         }
      }
   }

   ~Visualiser() {
      CloseWindow();
   }

private:
   int lowestPrice_;
   int highestPrice_;
   std::vector<float> buckets_fill_percentage_{};

   int GetNumberOfBuckets() const {
      int num_buckets = highestPrice_ - lowestPrice_;
      int max_bucket_width = GetScreenWidth() / (num_buckets);
      while (max_bucket_width < 2) {
         num_buckets /= 10;
         max_bucket_width = GetScreenWidth() / (num_buckets);
      }
      assert(num_buckets > 1);
      return num_buckets;
   }

   std::pair<int, int> GetBucketWidthAndHeight() const {
      int bucket_width = GetScreenWidth() / GetNumberOfBuckets();
      assert(bucket_width > 2);
      int bucket_height = GetScreenHeight() - BUCKET_VERTICAL_OFFSET;
      return {bucket_width, bucket_height};
   }

   std::pair<int, int> GetBucketPosition(int bucket_index) {
      int num_buckets = GetNumberOfBuckets();
      auto [outline_width, _] = GetBucketWidthAndHeight();
      int histogram_width = outline_width * num_buckets;
      int outline_x = (GetScreenWidth() - histogram_width) / 2 + bucket_index * outline_width;
      return {outline_x, BUCKET_VERTICAL_OFFSET / 2};
   }

   bool MouseInBucket(int bucket_index) {
      int mouse_x = GetMouseX();
      int mouse_y = GetMouseY();

      auto [bucket_x, bucket_y] = GetBucketPosition(bucket_index);
      auto [bucket_width, bucket_height] = GetBucketWidthAndHeight();

      return is_between_inclusive(mouse_x, bucket_x, bucket_x + bucket_width) &&
             is_between_inclusive(mouse_y, bucket_y, bucket_y + bucket_height);
   }
};

int main() {
   auto vis = Visualiser();

   vis.Loop();

}
