#include "s21_string.h"

char *s21_strcat(char *dest, const char *src) {
  s21_size_t i = s21_strlen(src);
  s21_size_t old_len = s21_strlen(dest);
  s21_size_t new_len = i + old_len;
  for (s21_size_t t = 0; old_len < new_len; old_len++, t++) {
    dest[old_len] = src[t];
  }
  dest[new_len] = '\0';
  return dest;
}