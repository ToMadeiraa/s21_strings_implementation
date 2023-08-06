#include "test_me.h"

int main(void) {
  int failed = 0;
  Suite *s21_string_test[] = {test_memchr(),
                              test_memcmp(),
                              test_memcpy(),
                              test_memmove(),
                              test_memset(),
                              test_strcat(),
                              test_strncat(),
                              test_strchr(),
                              test_strcmp(),
                              test_strncmp(),
                              test_strcpy(),
                              test_strncpy(),
                              test_strcspn(),
                              test_strerror(),
                              test_strlen(),
                              test_strpbrk(),
                              test_strrchr(),
                              test_strspn(),
                              test_strstr(),
                              test_strtok(),
                              test_sprintf_octal(),
                              test_sprintf_f(),
                              test_sprintf_c(),
                              test_sprintf_e(),
                              test_sprintf_g(),
                              test_sprintf_percent(),
                              test_sprintf_string(),
                              test_sprintf_unsigned(),
                              test_sprintf_HEX(),
                              test_sprintf_hex(),
                              test_sprintf_pointer(),
                              test_sprintf_signed(),
                              test_sscanf_empty(),
                              test_sscanf_c(),
                              s21_sscanf_s(),
                              s21_sscanf_u(),
                              s21_sscanf_xb(),
                              test_sscanf_d(),
                              test_sscanf_e(),
                              test_sscanf_eb(),
                              s21_sscanf_f(),
                              s21_sscanf_g(),
                              test_sscanf_i(),
                              test_sscanf_o(),
                              test_sscanf_x(),
                              test_sscanf_p(),
                              test_sscanf_real(),
                              test_sscanf_n(),
                              test_to_upper(),
                              test_to_lower(),
                              test_insert(),
                              test_trim(),
                              NULL};

  for (int i = 0; s21_string_test[i] != NULL; i++) {
    SRunner *sr = srunner_create(s21_string_test[i]);

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);

    failed += srunner_ntests_failed(sr);
    srunner_free(sr);
  }
  printf("========= FAILED: %d =========\n", failed);

  return failed == 0 ? 0 : 1;
}