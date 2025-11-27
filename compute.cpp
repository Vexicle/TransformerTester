#include <Arduino_FreeRTOS.h>
#include "compute.h"

int compute::levenshtein(const String& a, const String& b) { // modified from https://dev.to/best_codes/understanding-levenshtein-distance-a-powerful-algorithm-for-string-comparison-4p4a
  // i have no clue whether this works but it does compile
  int lengthA = a.length();
  int lengthB = b.length();
  int dp[lengthA + 1][lengthB + 1];

  for (int i = 0; i <= lengthA; i++) dp[i][0] = i;
  for (int j = 0; j <= lengthB; j++) dp[0][j] = j;

  for (int i = 1; i <= lengthA; i++) { // compute table of shortest characters to modify needed to
    for (int j = 1; j <= lengthB; j++) {
      int cost = (a[i - 1] == b[j - 1]) ? 0 : 1;
      int del = dp[i - 1][j] + 1;
      int ins = dp[i][j - 1] + 1;
      int sub = dp[i - 1][j - 1] + cost;
      dp[i][j] = min(min(del, ins), sub);
      }
    }
  return dp[lengthA][lengthB];
}