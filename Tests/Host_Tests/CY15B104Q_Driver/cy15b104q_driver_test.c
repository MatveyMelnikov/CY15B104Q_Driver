#include "unity_fixture.h"
#include "cy15b104q_driver.h"
#include "mock_spi.h"
#include "mock_cs_pin.h"
#include "mock_delay.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// Defines -------------------------------------------------------------------

enum {
  SPI_MOCK_SIZE = 10U,
  CS_PIN_MOCK_SIZE = 20U,
  DELAY_MOCK_SIZE = 5U
};

// Static variables ----------------------------------------------------------

static bool is_set = true;
static bool is_reset = false;

// Static functions ----------------------------------------------------------

static cy15b104q_driver_status cy15b104q_transmit(
  const uint8_t *const data,
  const uint16_t size,
  const uint32_t timeout
)
{
  (void)timeout;

  return (cy15b104q_driver_status)mock_spi_transmit(
    (uint8_t *const)data,
    size
  );
}

static cy15b104q_driver_status cy15b104q_receive(
  uint8_t *const data,
  const uint16_t size,
  const uint32_t timeout
)
{
  (void)timeout;

  return (cy15b104q_driver_status)mock_spi_receive(data, size);
}

static void cy15b104q_write_cs_pin(const bool is_set)
{
  mock_cs_pin_write((bool *const)&is_set);
}

// Tests ---------------------------------------------------------------------

TEST_GROUP(cy15b104q_driver_test);

TEST_SETUP(cy15b104q_driver_test)
{
  mock_spi_create(SPI_MOCK_SIZE);
  mock_cs_pin_create(CS_PIN_MOCK_SIZE);
  mock_delay_create(DELAY_MOCK_SIZE);

  cy15b104q_driver_init_module(
    (cy15b104q_driver_io_struct) {
      .transmit = cy15b104q_transmit,
      .receive = cy15b104q_receive,
      .write_cs_pin = cy15b104q_write_cs_pin,
      .delay = mock_delay_delay
    }
  );
}

TEST_TEAR_DOWN(cy15b104q_driver_test)
{
  cy15b104q_driver_deinit_module();
}

TEST(cy15b104q_driver_test, power_up_is_ok)
{
  uint32_t delay = 1U;

  mock_cs_pin_expect_write(&is_set);
  mock_delay_expect_delay(&delay);

  cy15b104q_driver_status status = cy15b104q_driver_power_up();

  TEST_ASSERT_EQUAL(CY15B104Q_STATUS_OK, status);
}

TEST(cy15b104q_driver_test, get_id_is_ok)
{
  uint8_t data_out = 0x9fU; // CY15B104Q datasheet, pg. 6
  uint8_t data_in[] = { 
    0x01U, 0x02U, 0x03U,
    0x04U, 0xaaU, 0xbbU,
    0xccU, 0x01, 0xaaU 
  };
  cy15b104q_driver_id result = { 0 };

  mock_cs_pin_expect_write(&is_reset);
  mock_spi_expect_transmit(&data_out, sizeof(data_out));
  mock_spi_expect_receive(data_in, sizeof(data_in));
  mock_cs_pin_expect_write(&is_set);

  cy15b104q_driver_status status = cy15b104q_driver_read_id(&result);

  TEST_ASSERT_EQUAL(CY15B104Q_STATUS_OK, status);
  TEST_ASSERT_EQUAL_INT8_ARRAY(
    data_in,
    (uint8_t*)&result,
    sizeof(data_in)
  );
}
