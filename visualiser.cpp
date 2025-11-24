#include <cassert>

#include "raylib.h"

class Visualiser {
public:
   Visualiser(int lowestPrice = 1, int highestPrice = 1000) : lowestPrice_(lowestPrice), highestPrice_(highestPrice) {
      InitWindow(400, 400, "Visualiser");
      int monitor = GetCurrentMonitor();
      windowWidth_ = GetMonitorWidth(monitor) / 2;
      windowHeight_ = GetMonitorHeight(monitor) / 2;

      SetWindowSize(windowWidth_, windowHeight_);
      SetWindowPosition(windowWidth_ - windowWidth_ / 2, windowHeight_);
      SetTargetFPS(60);
   }

   void Loop() {
      while (!WindowShouldClose()) {
         BeginDrawing();

         DrawBuckets();

         EndDrawing();
      }
   }

   void DrawBuckets() {

      int num_buckets = highestPrice_ - lowestPrice_;
      int max_bucket_width = windowWidth_ / (num_buckets);
      while (max_bucket_width < 2) {
         num_buckets /= 10;
         max_bucket_width = windowWidth_ / (num_buckets);
      }

      int outline_width = max_bucket_width;
      int inner_width = max_bucket_width - 2;
      int offset = 70;
      int bucket_height = windowHeight_ - offset;
      int histogram_width = max_bucket_width * num_buckets;

      for (int i = 0; i <= num_buckets; i++) {
         DrawRectangle((windowWidth_ - histogram_width) / 2 + i * outline_width, offset / 2, outline_width,
                       bucket_height, RED);
         DrawRectangle((windowWidth_ - histogram_width) / 2 + i * outline_width + 1, offset / 2 + 1, inner_width,
                       bucket_height - 2,
                       GRAY);
      }

   }

   ~Visualiser() {
      CloseWindow();
   }

private:
   int windowWidth_;
   int windowHeight_;
   int lowestPrice_;
   int highestPrice_;
};

int main() {
   auto vis = Visualiser();

   vis.Loop();

}
