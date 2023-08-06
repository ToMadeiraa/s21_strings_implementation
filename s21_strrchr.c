#include "s21_string.h"

char *s21_strrchr(const char *str, int c) {
  for (int i = s21_strlen(str); i >= 0; i--) {
    if (str[i] == c) return (char *)(&str[i]);
  }
  return s21_NULL;
}