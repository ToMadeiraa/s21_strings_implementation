#include "s21_string.h"

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  for (s21_size_t i = 0; i < n; i++)
    ((unznaked char *)dest)[i] = ((unznaked char *)src)[i];
  return dest;
}