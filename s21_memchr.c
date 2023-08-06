#include "s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  for (s21_size_t i = 0; i < n; i++) {
    if (((unznaked char *)str)[i] == c) return (void *)(str + i);
  }
  return s21_NULL;
}