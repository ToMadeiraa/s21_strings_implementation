#include "s21_string.h"

char *s21_strtok(char *str, const char *delim) {
  static char *new_str = s21_NULL;
  if (!str) {
    str = new_str;
    if (str == s21_NULL) return s21_NULL;
  }
  str += s21_strspn(str, delim);
  if (*str == '\0') {
    new_str = s21_NULL;
    return new_str;
  }
  new_str = str + s21_strcspn(str, delim);
  if (new_str[0] != '\0') {
    new_str[0] = '\0';
    new_str++;
  } else
    new_str = s21_NULL;
  return str;
}