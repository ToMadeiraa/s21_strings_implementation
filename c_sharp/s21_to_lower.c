#include "../s21_string.h"

void *s21_to_lower(const char *str) {
  if (!str) return s21_NULL;
  void *lower = malloc(s21_strlen(str) + 1);
  for (s21_size_t i = 0; i < s21_strlen(str) + 1; i++) {
    if (str[i] >= 65 && str[i] <= 90)
      ((char *)lower)[i] = str[i] + 32;
    else
      ((char *)lower)[i] = str[i];
  }
  return lower;
}
