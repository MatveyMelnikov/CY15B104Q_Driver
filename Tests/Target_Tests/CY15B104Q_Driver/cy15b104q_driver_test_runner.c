#include "unity_fixture.h"

TEST_GROUP_RUNNER(cy15b104q_driver_test)
{
  RUN_TEST_CASE(cy15b104q_driver_test, get_id_is_ok);
  RUN_TEST_CASE(cy15b104q_driver_test, check_link_is_ok);
  RUN_TEST_CASE(cy15b104q_driver_test, read_status_register_is_ok);
  RUN_TEST_CASE(cy15b104q_driver_test, write_status_register_is_ok);
  RUN_TEST_CASE(cy15b104q_driver_test, write_and_read_memory_data_is_ok);
  RUN_TEST_CASE(
    cy15b104q_driver_test,
    write_and_read_large_memory_data_is_ok
  );
  RUN_TEST_CASE(cy15b104q_driver_test, write_and_read_last_memory_data_is_ok);
}