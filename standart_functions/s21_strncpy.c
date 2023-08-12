#include "../s21_string.h"

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  s21_size_t length = s21_strlen(src);
  if (s21_strlen(src) > n) length = n;
  for (s21_size_t i = 0; i < length; i++) dest[i] = src[i];
  return dest;
}