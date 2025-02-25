#include <check.h>
#include <stdio.h>

#include "print.h"
#include "s21_decimal.h"

START_TEST(test_add) {
  char str1[100] = {0};
  s21_decimal value_1 = {0xffffffff, 0, 0, 0};
  s21_decimal value_2 = {0xffffffff, 0, 0, 0};

  s21_decimal result = {0};
  s21_decimal expected = {0xfffffffe, 1, 0, 0};
  s21_add(value_1, value_2, &result);
  print_96bit_decimal(result, str1);

  ck_assert_str_eq(str1, "8589934590");
  ck_assert_mem_eq(&result, &expected, sizeof(s21_decimal));
}
END_TEST

Suite *arithmetic_suite(void) {
    Suite *s;
    TCase *arithmetic;

    s = suite_create("arithmetic");

    arithmetic = tcase_create("arithmetic");

    tcase_add_test(arithmetic, test_add);

    suite_add_tcase(s, arithmetic);

    return s;
}

int main() {
    int number_failed = 0;
    Suite *su;
    SRunner *sr;

    su = arithmetic_suite();
    sr = srunner_create(su);
    srunner_run_all(sr, CK_VERBOSE);
    number_failed += srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? 0 : 1;
}