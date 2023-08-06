#include "test_me.h"

START_TEST(sscanf_spec_eb_1) {
  double a1 = 0, a2 = 0, b1 = 0, b2 = 0, c1 = 0, c2 = 0, d1 = 0, d2 = 0;

  const char str[] = "0.00001 -4123123 4. .";
  const char fstr[] = "%lf %lf %lf %lf";

  int16_t res1 = s21_sscanf(str, fstr, &a1, &b1, &c1, &d1);
  int16_t res2 = sscanf(str, fstr, &a2, &b2, &c2, &d2);

  ck_assert_int_eq(res1, res2);
  ck_assert_double_eq_tol(a1, a2, 1e-7);
  ck_assert_double_eq_tol(b1, b2, 1e-7);
  ck_assert_double_eq_tol(c1, c2, 1e-7);
  ck_assert_double_eq_tol(d1, d2, 1e-7);
}
END_TEST

START_TEST(sscanf_spec_eb_2) {
  float a1 = 0, a2 = 0, b1 = 0, b2 = 0, c1 = 0, c2 = 0, d1 = 0, d2 = 0;

  const char str[] = ". . . .";
  const char fstr[] = "%f %f %f %f";

  int16_t res1 = s21_sscanf(str, fstr, &a1, &b1, &c1, &d1);
  int16_t res2 = sscanf(str, fstr, &a2, &b2, &c2, &d2);

  ck_assert_int_eq(res1, res2);
  ck_assert_double_eq_tol(a1, a2, 1e-7);
  ck_assert_double_eq_tol(b1, b2, 1e-7);
  ck_assert_double_eq_tol(c1, c2, 1e-7);
  ck_assert_double_eq_tol(d1, d2, 1e-7);
}
END_TEST

START_TEST(sscanf_spec_eb_3) {
  float a1 = 0, a2 = 0, b1 = 0, b2 = 0, c1 = 0, c2 = 0;

  const char str[] = "53.1 -4.1135 411231.333 +2.0001";
  const char fstr[] = "%*f %f %f %f";

  int16_t res1 = s21_sscanf(str, fstr, &a1, &b1, &c1);
  int16_t res2 = sscanf(str, fstr, &a2, &b2, &c2);

  ck_assert_int_eq(res1, res2);
  ck_assert_float_eq_tol(a1, a2, 1e-7);
  ck_assert_float_eq_tol(b1, b2, 1e-7);
  ck_assert_float_eq_tol(c1, c2, 1e-7);
}
END_TEST

START_TEST(sscanf_spec_eb_4) {
  float a1 = 0, a2 = 0;

  const char str[] = "     .";
  const char fstr[] = "%E";

  int16_t res1 = s21_sscanf(str, fstr, &a1);
  int16_t res2 = sscanf(str, fstr, &a2);

  ck_assert_int_eq(res1, res2);
  ck_assert_float_eq_tol(a1, a2, 1e-7);
}
END_TEST

START_TEST(sscanf_spec_eb_5) {
  double a1 = 0, a2 = 0;
  float b1 = 0, b2 = 0;
  char c1 = 0, c2 = 0;

  const char str[] = "\t\t\n 1E1 -5 3450d";
  const char fstr[] = "%4lE %1E%c %*g";

  int16_t res1 = s21_sscanf(str, fstr, &a1, &b1, &c1);
  int16_t res2 = sscanf(str, fstr, &a2, &b2, &c2);

  ck_assert_int_eq(res1, res2);
  ck_assert_double_eq_tol(a1, a2, 1e-7);
  ck_assert_float_eq_tol(b1, b2, 1e-7);
  ck_assert_int_eq(c1, c2);
}
END_TEST

START_TEST(sscanf_spec_eb_6) {
  float a1 = 0, a2 = 0, c1 = 0, c2 = 0;
  int b1 = 0, b2 = 0;

  const char str[] = "15.E-2 -0 -0.e3";
  const char fstr[] = "%E %d %E";

  int16_t res1 = s21_sscanf(str, fstr, &a1, &b1, &c1);
  int16_t res2 = sscanf(str, fstr, &a2, &b2, &c2);

  ck_assert_int_eq(res1, res2);
  ck_assert_float_eq_tol(a1, a2, 1e-7);
  ck_assert_int_eq(b1, b2);
  ck_assert_float_eq_tol(c1, c2, 1e-7);
}
END_TEST

START_TEST(sscanf_spec_eb_7) {
  float a1 = 0, a2 = 0, c1 = 0, c2 = 0;
  int b1 = 0, b2 = 0;

  const char str[] = "-.3e -0 -0.e3";
  const char fstr[] = "%2E%d %E";

  int16_t res1 = s21_sscanf(str, fstr, &a1, &b1, &c1);
  int16_t res2 = sscanf(str, fstr, &a2, &b2, &c2);

  ck_assert_int_eq(res1, res2);
  ck_assert_float_eq_tol(a1, a2, 1e-7);
  ck_assert_int_eq(b1, b2);
  ck_assert_float_eq_tol(c1, c2, 1e-7);
}
END_TEST

START_TEST(sscanf_spec_eb_8) {
  long double a1 = 0, a2 = 0;

  const char str[] = "\t\t\n\r\v\f 1e3";
  const char fstr[] = "%LE";

  int16_t res1 = s21_sscanf(str, fstr, &a1);
  int16_t res2 = sscanf(str, fstr, &a2);

  ck_assert_int_eq(res1, res2);
  ck_assert_ldouble_eq(a1, a2);
}
END_TEST

START_TEST(sscanf_spec_eb_9) {
  double a1 = 0, a2 = 0;

  const char str[] = "e3";
  const char fstr[] = "%lE";

  int16_t res1 = s21_sscanf(str, fstr, &a1);
  int16_t res2 = sscanf(str, fstr, &a2);

  ck_assert_int_eq(res1, res2);
  ck_assert_double_eq_tol(a1, a2, 1e-7);
}
END_TEST

START_TEST(sscanf_spec_eb_10) {
  long double a1 = 0, a2 = 0;
  float b1 = 0, b2 = 0;

  const char str[] = "1e-10+0+1";
  const char fstr[] = "%LE %*E%E";

  int16_t res1 = s21_sscanf(str, fstr, &a1, &b1);
  int16_t res2 = sscanf(str, fstr, &a2, &b2);

  ck_assert_int_eq(res1, res2);
  ck_assert_ldouble_eq_tol(a1, a2, 1e-6);
  ck_assert_float_eq_tol(b1, b2, 1e-7);
}
END_TEST

Suite *test_sscanf_eb(void) {
  Suite *s = suite_create("\033[45m-=S21_SSCANF_EB=-\033[0m");
  TCase *tc = tcase_create("sscanf_tc");

  tcase_add_test(tc, sscanf_spec_eb_1);
  tcase_add_test(tc, sscanf_spec_eb_2);
  tcase_add_test(tc, sscanf_spec_eb_3);
  tcase_add_test(tc, sscanf_spec_eb_4);
  tcase_add_test(tc, sscanf_spec_eb_5);
  tcase_add_test(tc, sscanf_spec_eb_6);
  tcase_add_test(tc, sscanf_spec_eb_7);
  tcase_add_test(tc, sscanf_spec_eb_8);
  tcase_add_test(tc, sscanf_spec_eb_9);
  tcase_add_test(tc, sscanf_spec_eb_10);

  suite_add_tcase(s, tc);
  return s;
}
