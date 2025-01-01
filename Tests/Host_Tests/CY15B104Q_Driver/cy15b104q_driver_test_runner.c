#include "unity_fixture.h"

TEST_GROUP_RUNNER(cy15b104q_driver_test)
{
  RUN_TEST_CASE(cy15b104q_driver_test, power_up_is_ok);
  RUN_TEST_CASE(cy15b104q_driver_test, get_id_is_ok);
  RUN_TEST_CASE(cy15b104q_driver_test, read_status_register_is_ok);
  RUN_TEST_CASE(cy15b104q_driver_test, enable_write_protect_is_ok);
  RUN_TEST_CASE(cy15b104q_driver_test, enable_block_0_protect_is_ok);
  RUN_TEST_CASE(cy15b104q_driver_test, enable_block_1_protect_is_ok);
  RUN_TEST_CASE(cy15b104q_driver_test, write_enable_is_ok);
  RUN_TEST_CASE(cy15b104q_driver_test, write_disable_is_ok);
  RUN_TEST_CASE(cy15b104q_driver_test, write_memory_data_is_ok);
  RUN_TEST_CASE(cy15b104q_driver_test, write_memory_data_bound_check_is_ok);
  RUN_TEST_CASE(cy15b104q_driver_test, read_memory_data_is_ok);
  RUN_TEST_CASE(cy15b104q_driver_test, read_memory_data_bound_check_is_ok);
}