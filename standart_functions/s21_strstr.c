#include "../s21_string.h"

char *s21_strstr(const char *haystack, const char *needle) {
  if (*needle == '\0') return (char *)haystack;
  if (!haystack) return s21_NULL;
  char *ptr;
  for (int i = 0; haystack[i] != '\0'; i++) {
    short int status = 0;
    if (haystack[i] == needle[0]) {
      ptr = (char *)haystack + i;
      for (s21_size_t j = i, k = 0; k < s21_strlen(needle); j++, k++) {
        if (haystack[j] != needle[k]) {
          status = 0;
          break;
        } else
          status = 1;
      }
    }
    if (status) return ptr;
  }
  return s21_NULL;
}