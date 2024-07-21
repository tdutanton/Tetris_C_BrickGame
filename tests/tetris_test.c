#include "tetris_test.h"

int main(void) {
  int n_failed = 0;
  Suite *suite = NULL;
  SRunner *sr = srunner_create(suite);

  Suite *suites[] = {test_tetris(), NULL};

  for (Suite **st = suites; *st != NULL; st++) srunner_add_suite(sr, *st);
  srunner_set_fork_status(sr, CK_NOFORK);  // запрещаем останавливать тесты
                                           // после первой найденной ошибки

  srunner_run_all(sr, CK_NORMAL);
  n_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  (void)n_failed;
  return 0;
}