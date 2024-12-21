#include "unity_fixture.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// Defines -------------------------------------------------------------------

enum {
  SPI_MOCK_SIZE = 10U,
  CS_PIN_MOCK_SIZE = 20U
};

// Static variables ----------------------------------------------------------

// Static functions ----------------------------------------------------------

// Tests ---------------------------------------------------------------------

TEST_GROUP(cy15b104q_driver_test);

TEST_SETUP(cy15b104q_driver_test)
{
}

TEST_TEAR_DOWN(cy15b104q_driver_test)
{
}

TEST(cy15b104q_driver_test, init_is_ok)
{
  TEST_ASSERT_EQUAL(true, true);
}
