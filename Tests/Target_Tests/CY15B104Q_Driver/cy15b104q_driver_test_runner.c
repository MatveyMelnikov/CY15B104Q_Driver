#include "unity_fixture.h"

TEST_GROUP_RUNNER(cy15b104q_driver_test)
{
  RUN_TEST_CASE(cy15b104q_driver_test, get_id_is_ok);
  RUN_TEST_CASE(cy15b104q_driver_test, check_link_is_ok);
}