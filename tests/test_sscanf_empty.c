#include "test_me.h"

START_TEST(sscanf_empty_1) {
  char format[] = "\t";
  char str[] = "\t\n\n   \n  \t";
  int p1, p2;

  int16_t res1 = s21_sscanf(str, format, &p1);
  int16_t res2 = sscanf(str, format, &p2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(sscanf_empty_2) {
  char format[] = "";
  char str[] = "\t\n\n   \n  \t";
  int p1, p2;

  int16_t res1 = s21_sscanf(str, format, &p1);
  int16_t res2 = sscanf(str, format, &p2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(sscanf_empty_3) {
  char format[] = "";
  char str[] = "    \t";
  int p1, p2;

  int16_t res1 = s21_sscanf(str, format, &p1);
  int16_t res2 = sscanf(str, format, &p2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(sscanf_empty_4) {
  char format[] = "";
  char str[] = "\t\n\n ";
  int p1, p2;

  int16_t res1 = s21_sscanf(str, format, &p1);
  int16_t res2 = sscanf(str, format, &p2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(sscanf_empty_5) {
  char format[] = "";
  char str[] = "\t\n\n\n\t";
  int p1, p2;

  int16_t res1 = s21_sscanf(str, format, &p1);
  int16_t res2 = sscanf(str, format, &p2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(sscanf_empty_6) {
  char format[] = "";
  char str[] = "\t\n\n   \n  \t";
  int p1, p2;

  int16_t res1 = s21_sscanf(str, format, &p1);
  int16_t res2 = sscanf(str, format, &p2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(sscanf_empty_7) {
  char format[] = "\t";
  char str[] = "\t\n\n   \n  \t";
  int p1, p2;

  int16_t res1 = s21_sscanf(str, format, &p1);
  int16_t res2 = sscanf(str, format, &p2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

Suite *test_sscanf_empty(void) {
  Suite *s = suite_create("\033[45m-=S21_SSCANF_EMPTY=-\033[0m");
  TCase *tc = tcase_create("sscanf_tc");

  tcase_add_test(tc, sscanf_empty_1);
  tcase_add_test(tc, sscanf_empty_2);
  tcase_add_test(tc, sscanf_empty_3);
  tcase_add_test(tc, sscanf_empty_4);
  tcase_add_test(tc, sscanf_empty_5);
  tcase_add_test(tc, sscanf_empty_6);
  tcase_add_test(tc, sscanf_empty_7);

  suite_add_tcase(s, tc);
  return s;
}

// TEST CASE 46
// Running suite(s): s21_sscanf_s
// 100%: Checks: 11, Failures: 0, Errors: 0
// TEST CASE 48
// Running suite(s): s21_sscanf_u
// 100%: Checks: 8, Failures: 0, Errors: 0
// TEST CASE 49
// Running suite(s): s21_sscanf_xb
// 100%: Checks: 21, Failures: 0, Errors: 0
// TEST CASE 51
// Running suite(s): s21_sscanf_f
// 100%: Checks: 10, Failures: 0, Errors: 0
// TEST CASE 52
// Running suite(s): s21_sscanf_e
// 100%: Checks: 8, Failures: 0, Errors: 0
// TEST CASE 53
// Running suite(s): s21_sscanf_eb
// 100%: Checks: 10, Failures: 0, Errors: 0
// TEST CASE 54
// Running suite(s): s21_sscanf_g
// 100%: Checks: 5, Failures: 0, Errors: 0
// TEST CASE 55
// Running suite(s): s21_sscanf_gb
// 100%: Checks: 3, Failures: 0, Errors: 0
// TEST CASE 58
// Running suite(s): s21_sscanf_pr
// 100%: Checks: 5, Failures: 0, Errors: 0