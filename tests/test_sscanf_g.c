#include "test_me.h"

START_TEST(sscanf_spec_g_1) {
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

START_TEST(sscanf_spec_g_2) {
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

START_TEST(sscanf_spec_g_3) {
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

START_TEST(sscanf_spec_g_4) {
  float a1 = 0, a2 = 0, b1 = 0, b2 = 0, c1 = 0, c2 = 0;

  const char str[] = "53.1 -12 -a +2.0001";
  const char fstr[] = "%*g %g %g %G";

  int16_t res1 = s21_sscanf(str, fstr, &a1, &b1, &c1);
  int16_t res2 = sscanf(str, fstr, &a2, &b2, &c2);

  ck_assert_int_eq(res1, res2);
  ck_assert_float_eq_tol(a1, a2, 1e-7);
  ck_assert_float_eq_tol(b1, b2, 1e-7);
  ck_assert_float_eq_tol(c1, c2, 1e-7);
}
END_TEST

START_TEST(sscanf_spec_g_5) {
  double a1 = 0, a2 = 0, b1 = 0, b2 = 0, c1 = 0, c2 = 0;

  const char str[] = "53.1 .3 -15e-17 .0001";
  const char fstr[] = "%*g %lg %lg %1lg";

  int16_t res1 = s21_sscanf(str, fstr, &a1, &b1, &c1);
  int16_t res2 = sscanf(str, fstr, &a2, &b2, &c2);

  ck_assert_int_eq(res1, res2);
  ck_assert_double_eq_tol(a1, a2, 1e-7);
  ck_assert_double_eq_tol(b1, b2, 1e-7);
  ck_assert_double_eq_tol(c1, c2, 1e-7);
}
END_TEST

Suite *s21_sscanf_g(void) {
  Suite *s = suite_create("\033[45m-=S21_SSCANF_G=-\033[0m");
  TCase *tc = tcase_create("sscanf_tc");

  tcase_add_test(tc, sscanf_spec_g_1);
  tcase_add_test(tc, sscanf_spec_g_2);
  tcase_add_test(tc, sscanf_spec_g_3);
  tcase_add_test(tc, sscanf_spec_g_4);
  tcase_add_test(tc, sscanf_spec_g_5);
  suite_add_tcase(s, tc);
  return s;
}
