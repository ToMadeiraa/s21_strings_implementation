#include "s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t size) {
  unznaked char *s1 = (unznaked char *)str1;
  unznaked char *s2 = (unznaked char *)str2;
  int result = 0;
  for (s21_size_t i = 0; result == 0 && i < size; i += 1)
    result = s1[i] - s2[i];
  return result;
}