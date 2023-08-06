#include "s21_string.h"

void *s21_trim(const char *src, const char *trim_chars) {
  if (!src || !trim_chars) return s21_NULL;
  s21_size_t count_start = 0;
  for (s21_size_t i = 0; i < s21_strlen(src) + 1; i++) {
    short int status = 0;
    if (s21_strlen(trim_chars) == 0) {
      if (src[i] <= 32)
        count_start++;
      else
        break;
    } else {
      for (s21_size_t j = 0; j < s21_strlen(trim_chars); j++) {
        if (src[i] == trim_chars[j]) {
          count_start++;
          status = 1;
          break;
        }
      }
      if (!status) break;
    }
  }
  s21_size_t count_end = 0;
  for (s21_size_t i = s21_strlen(src); i != 0; i--) {
    short int status = 0;
    if (s21_strlen(trim_chars) == 0) {
      if (0 <= src[i] && src[i] <= 32) {
        if (0 < src[i] && src[i] <= 32) count_end++;
      } else
        break;
    } else {
      for (s21_size_t j = 0; j < s21_strlen(trim_chars); j++) {
        if (src[i - 1] == trim_chars[j]) {
          count_end++;
          status = 1;
          break;
        }
      }
      if (!status) break;
    }
  }

  void *trim;
  if (s21_strlen(src) <= (count_start + count_end)) {
    trim = malloc(1);
    ((char *)trim)[0] = '\0';
  } else {
    s21_size_t size = s21_strlen(src) - count_start - count_end;
    trim = malloc(size + 1);
    for (s21_size_t i = 0;
         (i < (size)) && count_start != (s21_strlen(src) - count_end);
         i++, count_start++)
      ((char *)trim)[i] = src[count_start];
    ((char *)trim)[size] = '\0';
  }
  return trim;
}
