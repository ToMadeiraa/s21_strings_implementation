#include "../s21_string.h"

void *s21_insert(const char *src, const char *str, size_t start_index) {
  if (start_index > s21_strlen(src)) return s21_NULL;
  void *insert = malloc(s21_strlen(src) + s21_strlen(str) + 1);
  s21_size_t i;
  for (i = 0; i < start_index; i++) ((char *)insert)[i] = src[i];
  s21_size_t j = i;
  for (int k = 0; str[k] != '\0'; i++, k++) ((char *)insert)[i] = str[k];
  for (; src[j] != '\0'; i++, j++) ((char *)insert)[i] = src[j];
  ((char *)insert)[i] = '\0';
  return (void *)insert;
}
