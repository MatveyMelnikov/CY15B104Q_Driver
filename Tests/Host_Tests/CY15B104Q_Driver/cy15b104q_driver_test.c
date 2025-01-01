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
  mock_spi_verify_complete();
  mock_cs_pin_verify_complete();
  mock_delay_verify_complete();

  mock_spi_destroy();
  mock_cs_pin_destroy();
  mock_delay_destroy();

  cy15b104q_driver_deinit_module();
}

TEST(cy15b104q_driver_test, power_up_is_ok)
{
  uint32_t delay = 0xaU;
  uint8_t data_out = 0x5U;
  uint8_t data_in = 0x40U;

  mock_cs_pin_expect_write(&is_set);
  mock_delay_expect_delay(&delay);

  mock_cs_pin_expect_write(&is_reset);
  mock_spi_expect_transmit(&data_out, sizeof(uint8_t));
  mock_spi_expect_receive(&data_in, sizeof(uint8_t));
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
  mock_spi_expect_transmit(&data_out, sizeof(uint8_t));
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

TEST(cy15b104q_driver_test, read_status_register_is_ok)
{
  uint8_t data_out = 0x5U;
  uint8_t data_in = 0x40U;

  mock_cs_pin_expect_write(&is_reset);
  mock_spi_expect_transmit(&data_out, sizeof(uint8_t));
  mock_spi_expect_receive(&data_in, sizeof(uint8_t));
  mock_cs_pin_expect_write(&is_set);

  uint8_t result;
  cy15b104q_driver_status status = cy15b104q_driver_read_status_register(
    &result
  );

  TEST_ASSERT_EQUAL(CY15B104Q_STATUS_OK, status);
  TEST_ASSERT_EQUAL_INT8(0x40U, result);
}

TEST(cy15b104q_driver_test, enable_write_protect_is_ok)
{
  uint8_t data_out = CY15B104Q_CMD_WRITE_STATUS;
  uint8_t data_payload = 0x80U;

  mock_cs_pin_expect_write(&is_reset);
  mock_spi_expect_transmit(&data_out, sizeof(uint8_t));
  mock_spi_expect_transmit(&data_payload, sizeof(uint8_t));
  mock_cs_pin_expect_write(&is_set);

  uint8_t result;
  cy15b104q_driver_status status = cy15b104q_driver_write_status_register(
    true, false, false
  );

  TEST_ASSERT_EQUAL(CY15B104Q_STATUS_OK, status);
}

TEST(cy15b104q_driver_test, enable_block_0_protect_is_ok)
{
  uint8_t data_out = CY15B104Q_CMD_WRITE_STATUS;
  uint8_t data_payload = 0x4U;

  mock_cs_pin_expect_write(&is_reset);
  mock_spi_expect_transmit(&data_out, sizeof(uint8_t));
  mock_spi_expect_transmit(&data_payload, sizeof(uint8_t));
  mock_cs_pin_expect_write(&is_set);

  uint8_t result;
  cy15b104q_driver_status status = cy15b104q_driver_write_status_register(
    false, true, false
  );

  TEST_ASSERT_EQUAL(CY15B104Q_STATUS_OK, status);
}

TEST(cy15b104q_driver_test, enable_block_1_protect_is_ok)
{
  uint8_t data_out = CY15B104Q_CMD_WRITE_STATUS;
  uint8_t data_payload = 0x8U;

  mock_cs_pin_expect_write(&is_reset);
  mock_spi_expect_transmit(&data_out, sizeof(uint8_t));
  mock_spi_expect_transmit(&data_payload, sizeof(uint8_t));
  mock_cs_pin_expect_write(&is_set);

  uint8_t result;
  cy15b104q_driver_status status = cy15b104q_driver_write_status_register(
    false, false, true
  );

  TEST_ASSERT_EQUAL(CY15B104Q_STATUS_OK, status);
}

TEST(cy15b104q_driver_test, write_enable_is_ok)
{
  uint8_t data_out = CY15B104Q_CMD_WRITE_ENABLE;

  mock_cs_pin_expect_write(&is_reset);
  mock_spi_expect_transmit(&data_out, sizeof(uint8_t));
  mock_cs_pin_expect_write(&is_set);

  cy15b104q_driver_status status = cy15b104q_driver_write_enable();

  TEST_ASSERT_EQUAL(CY15B104Q_STATUS_OK, status);
}

TEST(cy15b104q_driver_test, write_disable_is_ok)
{
  uint8_t data_out = CY15B104Q_CMD_WRITE_ENABLE;

  mock_cs_pin_expect_write(&is_reset);
  mock_spi_expect_transmit(&data_out, sizeof(uint8_t));
  mock_cs_pin_expect_write(&is_set);

  cy15b104q_driver_status status = cy15b104q_driver_write_enable();

  TEST_ASSERT_EQUAL(CY15B104Q_STATUS_OK, status);
}

TEST(cy15b104q_driver_test, write_memory_data_is_ok)
{
  uint8_t cmd_out = CY15B104Q_CMD_WRITE_MEMORY_DATA;
  uint32_t address_out = 0x10;
  uint8_t data[] = { 0xaa, 0xbb, 0xcc };

  mock_cs_pin_expect_write(&is_reset);
  mock_spi_expect_transmit(&cmd_out, sizeof(uint8_t));
  mock_spi_expect_transmit((uint8_t*)&address_out, 3U);
  mock_spi_expect_transmit(data, sizeof(data));
  mock_cs_pin_expect_write(&is_set);

  cy15b104q_driver_address address = {
    .full = 0x10
  };
  cy15b104q_driver_status status = cy15b104q_driver_write_memory_data(
    address, data, sizeof(data)
  );

  TEST_ASSERT_EQUAL(CY15B104Q_STATUS_OK, status);
}

TEST(cy15b104q_driver_test, write_memory_data_bound_check_is_ok)
{
  uint8_t cmd_out = CY15B104Q_CMD_WRITE_MEMORY_DATA;
  uint32_t address_out = 0x80000U;
  uint8_t data[] = { 0xaa, 0xbb, 0xcc };

  cy15b104q_driver_address address = {
    .full = 0x80000U
  };
  cy15b104q_driver_status status = cy15b104q_driver_write_memory_data(
    address, data, sizeof(data)
  );

  TEST_ASSERT_EQUAL(CY15B104Q_STATUS_ERROR, status);
}

TEST(cy15b104q_driver_test, read_memory_data_is_ok)
{
  uint8_t cmd_out = CY15B104Q_CMD_READ_MEMORY_DATA;
  uint32_t address_out = 0x10;
  uint8_t expected_data[] = { 0xaa, 0xbb, 0xcc };
  uint8_t actual_data[3] = { 0 };

  mock_cs_pin_expect_write(&is_reset);
  mock_spi_expect_transmit(&cmd_out, sizeof(uint8_t));
  mock_spi_expect_transmit((uint8_t*)&address_out, 3U);
  mock_spi_expect_receive(expected_data, sizeof(expected_data));
  mock_cs_pin_expect_write(&is_set);

  cy15b104q_driver_address address = {
    .full = 0x10
  };
  cy15b104q_driver_status status = cy15b104q_driver_read_memory_data(
    address, actual_data, sizeof(actual_data)
  );

  TEST_ASSERT_EQUAL(CY15B104Q_STATUS_OK, status);
  TEST_ASSERT_EQUAL_INT8_ARRAY(
    expected_data,
    actual_data,
    sizeof(expected_data)
  );
}

TEST(cy15b104q_driver_test, read_memory_data_bound_check_is_ok)
{
  uint8_t cmd_out = CY15B104Q_CMD_READ_MEMORY_DATA;
  uint32_t address_out = 0x80000U;
  uint8_t data[] = { 0xaa, 0xbb, 0xcc };

  cy15b104q_driver_address address = {
    .full = 0x80000U
  };
  cy15b104q_driver_status status = cy15b104q_driver_write_memory_data(
    address, data, sizeof(data)
  );

  TEST_ASSERT_EQUAL(CY15B104Q_STATUS_ERROR, status);
}
