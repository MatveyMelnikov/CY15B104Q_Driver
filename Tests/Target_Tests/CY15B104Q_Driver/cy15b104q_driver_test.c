#include "unity_fixture.h"
#include "cy15b104q_driver.h"
#include "main.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// Global variables ----------------------------------------------------------

extern SPI_HandleTypeDef hspi1;

// Defines -------------------------------------------------------------------

// Static variables ----------------------------------------------------------

// Static functions ----------------------------------------------------------

static cy15b104q_driver_status cy15b104q_transmit(
  const uint8_t *const data,
  const uint16_t size,
  const uint32_t timeout
)
{
  return (cy15b104q_driver_status)HAL_SPI_Transmit(
    &hspi1,
    (uint8_t*)data,
    size,
    timeout
  );
}

static cy15b104q_driver_status cy15b104q_receive(
  uint8_t *const data,
  const uint16_t size,
  const uint32_t timeout
)
{
  return (cy15b104q_driver_status)HAL_SPI_Receive(
    &hspi1,
    data,
    size,
    timeout
  );
}

static void cy15b104q_write_cs_pin(const bool is_set)
{
  HAL_GPIO_WritePin(
    CY15B104Q_NCS_GPIO_Port,
    CY15B104Q_NCS_Pin,
    is_set ? GPIO_PIN_SET : GPIO_PIN_RESET
  );
}

// Tests ---------------------------------------------------------------------

TEST_GROUP(cy15b104q_driver_test);

TEST_SETUP(cy15b104q_driver_test)
{
  cy15b104q_driver_init_module(
    (cy15b104q_driver_io_struct) {
      .transmit = cy15b104q_transmit,
      .receive = cy15b104q_receive,
      .write_cs_pin = cy15b104q_write_cs_pin,
      .delay = HAL_Delay
    }
  );
  (void)cy15b104q_driver_power_up();
  (void)cy15b104q_driver_sleep_recover();
}

TEST_TEAR_DOWN(cy15b104q_driver_test)
{
  cy15b104q_driver_deinit_module();
}

TEST(cy15b104q_driver_test, get_id_is_ok)
{
  cy15b104q_driver_id result = { 0 };

  cy15b104q_driver_status status = cy15b104q_driver_read_id(&result);

  TEST_ASSERT_EQUAL(CY15B104Q_STATUS_OK, status);
  TEST_ASSERT_EQUAL(0x7f7f7f7f, *(uint32_t*)result.manufacturer_id);
  TEST_ASSERT_EQUAL(0x0826, *(uint16_t*)result.product_id); // little-endian
}

TEST(cy15b104q_driver_test, check_link_is_ok)
{
  cy15b104q_driver_status status = cy15b104q_driver_check_link();

  TEST_ASSERT_EQUAL(CY15B104Q_STATUS_OK, status);
}

TEST(cy15b104q_driver_test, sleep_is_ok)
{
  cy15b104q_driver_status status = cy15b104q_driver_sleep();

  TEST_ASSERT_EQUAL(CY15B104Q_STATUS_OK, status);
}

TEST(cy15b104q_driver_test, sleep_recover_is_ok)
{
  cy15b104q_driver_status status = cy15b104q_driver_sleep_recover();

  TEST_ASSERT_EQUAL(CY15B104Q_STATUS_OK, status);
}
