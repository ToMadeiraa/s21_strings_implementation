#include "s21_string.h"

void *s21_to_upper(const char *str) {
  if (!str) return s21_NULL;
  void *upper = malloc(s21_strlen(str) + 1);
  for (s21_size_t i = 0; i < s21_strlen(str) + 1; i++) {
    if (str[i] >= 97 && str[i] <= 122)
      ((char *)upper)[i] = str[i] - 32;
    else
      ((char *)upper)[i] = str[i];
  }
  return upper;
}
