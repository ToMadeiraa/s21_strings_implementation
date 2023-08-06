#include "test_me.h"

// One parameter unznaked
START_TEST(sprintf_1_unznaked) {
  char str1[100] = "";
  char str2[100] = "";
  char *str3 = "Test %u Test";
  unznaked int val = 012;
  ck_assert_int_eq(sprintf(str1, str3, val), s21_sprintf(str2, str3, val));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

// Three unznaked parameters
START_TEST(sprintf_2_unznaked) {
  char str1[100];
  char str2[100];
  char *str3 = "%u Test %u Test %u";
  unznaked int val = 012;
  unznaked int val2 = 017;
  unznaked int val3 = 07464;
  ck_assert_int_eq(sprintf(str1, str3, val, val2, val3),
                   s21_sprintf(str2, str3, val, val2, val3));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

// Three decimal parameters
START_TEST(sprintf_3_unznaked) {
  char str1[100];
  char str2[100];
  char *str3 = "%u Test %u Test %u";
  unznaked int val = 3015;
  unznaked int val2 = 712;
  unznaked int val3 = 99;
  ck_assert_int_eq(sprintf(str1, str3, val, val2, val3),
                   s21_sprintf(str2, str3, val, val2, val3));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

// Different sizes
START_TEST(sprintf_4_unznaked) {
  char str1[100];
  char str2[100];
  char *str3 = "%lu Test %lu Test %hu GOD %hu";
  long unznaked int val = 3088675747373646;
  long unznaked val2 = 33030030303;
  unznaked short val3 = 22600;
  unznaked short val4 = 120;
  ck_assert_int_eq(sprintf(str1, str3, val, val2, val3, val4),
                   s21_sprintf(str2, str3, val, val2, val3, val4));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

// Different width
START_TEST(sprintf_5_unznaked) {
  char str1[100];
  char str2[100];
  char *str3 = "%3u Test %5u Test %10u";
  unznaked int val = 3015;
  unznaked int val2 = 01234;
  unznaked int val3 = 99;
  ck_assert_int_eq(sprintf(str1, str3, val, val2, val3),
                   s21_sprintf(str2, str3, val, val2, val3));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

// Different precision and width
START_TEST(sprintf_6_unznaked) {
  char str1[200];
  char str2[200];
  char *str3 = "%6.5u Test %.23u Test %3.u TEST %.u";
  unznaked int val = 3015;
  unznaked int val2 = 712;
  unznaked int val3 = 99;
  unznaked int val4 = 38;
  ck_assert_int_eq(sprintf(str1, str3, val, val2, val3, val4),
                   s21_sprintf(str2, str3, val, val2, val3, val4));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

// Minus flag
START_TEST(sprintf_7_unznaked) {
  char str1[200];
  char str2[200];
  char *str3 = "%-10.5u Test %-.8u Test %-7u TEST %-.u";
  unznaked int val = 3015;
  unznaked int val2 = 712;
  unznaked int val3 = 99;
  unznaked int val4 = 2939;
  ck_assert_int_eq(sprintf(str1, str3, val, val2, val3, val4),
                   s21_sprintf(str2, str3, val, val2, val3, val4));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

// Zerues
START_TEST(sprintf_8_unznaked) {
  char str1[200];
  char str2[200];
  char *str3 = "%0u Test %0.u Test %0.0u TEST %0u GOD %.u";
  unznaked int val = 3015;
  unznaked int val2 = 712;
  unznaked int val3 = 99;
  unznaked int val4 = 2939;
  unznaked int val5 = 0123;
  ck_assert_int_eq(sprintf(str1, str3, val, val2, val3, val4, val5),
                   s21_sprintf(str2, str3, val, val2, val3, val4, val5));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

// Pluses
START_TEST(sprintf_9_unznaked) {
  char str1[200];
  char str2[200];
  char *str3 = "%+u Test %+3.u Test %+5.7u TEST %+10u";
  unznaked int val = 3015;
  unznaked int val2 = 712;
  unznaked int val3 = 99;
  unznaked int val4 = 2939;
  ck_assert_int_eq(sprintf(str1, str3, val, val2, val3, val4),
                   s21_sprintf(str2, str3, val, val2, val3, val4));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

// Zeru vals
START_TEST(sprintf_10_unznaked) {
  char str1[200];
  char str2[200];
  char *str3 = "%u Test %3.u Test %5.7u TEST %10u %#u %-u %+u %.u % .u";
  unznaked int val = 0;
  sprintf(str1, str3, val, val, val, val, val, val, val, val, val);
  s21_sprintf(str2, str3, val, val, val, val, val, val, val, val, val);
  ck_assert_int_eq(
      sprintf(str1, str3, val, val, val, val, val, val, val, val, val),
      s21_sprintf(str2, str3, val, val, val, val, val, val, val, val, val));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

// Spaces
START_TEST(sprintf_11_unznaked) {
  char str1[200];
  char str2[200];
  char *str3 = "% u Test % 3.u Test % 5.7u TEST % 10u GOD %.u";
  unznaked int val = 32;
  unznaked int val2 = 8899;
  unznaked int val3 = 91918;
  unznaked int val4 = 32311;
  unznaked int val5 = 23;
  ck_assert_int_eq(sprintf(str1, str3, val, val2, val3, val4, val5),
                   s21_sprintf(str2, str3, val, val2, val3, val4, val5));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

// Plus
START_TEST(sprintf_12_unznaked) {
  char str1[200];
  char str2[200];
  char *str3 = "%+u Test %+3.u Test %+5.7u TEST %+10u GOD %+.u";
  unznaked int val = 32;
  unznaked int val2 = 8899;
  unznaked int val3 = 91918;
  unznaked int val4 = 32311;
  unznaked int val5 = 3261;
  ck_assert_int_eq(sprintf(str1, str3, val, val2, val3, val4, val5),
                   s21_sprintf(str2, str3, val, val2, val3, val4, val5));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

// Hash
START_TEST(sprintf_13_unznaked) {
  char str1[200];
  char str2[200];
  char *str3 = "%#u Test %#3u Test %#5.7u TEST %#.7u Ouf %#.u";
  unznaked int val = 32;
  unznaked int val2 = 8899;
  unznaked int val3 = 91918;
  unznaked int val4 = 32311;
  unznaked int val5 = 8894;
  ck_assert_int_eq(sprintf(str1, str3, val, val2, val3, val4, val5),
                   s21_sprintf(str2, str3, val, val2, val3, val4, val5));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

// ZERO flag
START_TEST(sprintf_14_unznaked) {
  char str1[200];
  char str2[200];
  char *str3 = "%0u Test %06u Test %05.7u TEST %0.7u Ouf %0.u";
  unznaked int val = 32;
  unznaked int val2 = 8899;
  unznaked int val3 = 91918;
  unznaked int val4 = 32311;
  unznaked int val5 = 8894;
  ck_assert_int_eq(sprintf(str1, str3, val, val2, val3, val4, val5),
                   s21_sprintf(str2, str3, val, val2, val3, val4, val5));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

// Asterisk
START_TEST(sprintf_15_unznaked) {
  char str1[200];
  char str2[200];
  char *str3 = "%*u Test %-*u Test %*.*u TEST %.*u";
  unznaked int val = 32;
  unznaked int val2 = 8899;
  unznaked int val3 = 919;
  unznaked int val4 = 32311;
  unznaked int ast = 2;
  unznaked int ast2 = 5;
  unznaked int ast3 = 4;
  unznaked int ast4 = 10;
  unznaked int ast5 = 7;
  ck_assert_int_eq(
      sprintf(str1, str3, ast, val, ast2, val2, ast3, ast4, val3, ast5, val4),
      s21_sprintf(str2, str3, ast, val, ast2, val2, ast3, ast4, val3, ast5,
                  val4));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_16_unznaked) {
  char str1[200];
  char str2[200];
  char *str3 = "fdsdsds %lu";
  unznaked long int val = ULONG_MAX;
  ck_assert_int_eq(sprintf(str1, str3, val), s21_sprintf(str2, str3, val));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_17_unznaked) {
  char str1[200];
  char str2[200];
  char *str3 = "%- u Test %- 15u sdasda %- 15u sdsad %- u";
  unznaked int val = -3231;
  unznaked int val2 = -3231;
  unznaked int val3 = 3231;
  unznaked int val4 = 3231;
  ck_assert_int_eq(sprintf(str1, str3, val, val2, val3, val4),
                   s21_sprintf(str2, str3, val, val2, val3, val4));
  ck_assert_pstr_eq(str1, str2);
}
END_TEST
Suite *test_sprintf_unznaked(void) {
  Suite *s = suite_create("\033[45m-=S21_SPRINTF_UNSIGNED=-\033[0m");
  TCase *tc = tcase_create("sprintf_tc");

  tcase_add_test(tc, sprintf_1_unznaked);
  tcase_add_test(tc, sprintf_2_unznaked);
  tcase_add_test(tc, sprintf_3_unznaked);
  tcase_add_test(tc, sprintf_4_unznaked);
  tcase_add_test(tc, sprintf_5_unznaked);
  tcase_add_test(tc, sprintf_6_unznaked);
  tcase_add_test(tc, sprintf_7_unznaked);
  tcase_add_test(tc, sprintf_8_unznaked);
  tcase_add_test(tc, sprintf_9_unznaked);
  tcase_add_test(tc, sprintf_10_unznaked);
  tcase_add_test(tc, sprintf_11_unznaked);
  tcase_add_test(tc, sprintf_12_unznaked);
  tcase_add_test(tc, sprintf_13_unznaked);
  tcase_add_test(tc, sprintf_14_unznaked);
  tcase_add_test(tc, sprintf_15_unznaked);
  tcase_add_test(tc, sprintf_16_unznaked);
  tcase_add_test(tc, sprintf_17_unznaked);

  suite_add_tcase(s, tc);
  return s;
}
