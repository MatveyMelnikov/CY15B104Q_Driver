#include "unity_fixture.h"
#include "cy15b104q_driver.h"
#include "main.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// Global variables ----------------------------------------------------------

extern SPI_HandleTypeDef hspi1;

// Defines -------------------------------------------------------------------

#define TEST_PATTERN 0xA5B5U

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
}

TEST_TEAR_DOWN(cy15b104q_driver_test)
{
  (void)cy15b104q_driver_write_enable();
  (void)cy15b104q_driver_write_status_register(false, false, false);
  (void)cy15b104q_driver_write_disable();
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

TEST(cy15b104q_driver_test, read_status_register_is_ok)
{
  uint8_t result;

  cy15b104q_driver_status status = cy15b104q_driver_read_status_register(
    &result
  );

  TEST_ASSERT_EQUAL(CY15B104Q_STATUS_OK, status);
  TEST_ASSERT_EQUAL_INT8(0x40U, result);
}

TEST(cy15b104q_driver_test, write_status_register_is_ok)
{
  cy15b104q_driver_status status = cy15b104q_driver_write_enable();

  status |= cy15b104q_driver_write_status_register(true, true, true);

  TEST_ASSERT_EQUAL(CY15B104Q_STATUS_OK, status);

  uint8_t result_status_register;
  status |= cy15b104q_driver_read_status_register(&result_status_register);

  TEST_ASSERT_EQUAL(CY15B104Q_STATUS_OK, status);
  TEST_ASSERT_EQUAL_INT8(0xccU, result_status_register);
}

TEST(cy15b104q_driver_test, write_and_read_memory_data_is_ok)
{
  cy15b104q_driver_address addr = {
    .full = 0x0U
  };
  uint8_t data_out[] = { 0xaa, 0xbb, 0xcc };
  uint8_t data_in[3] = { 0 };

  cy15b104q_driver_status status = cy15b104q_driver_write_enable();
  status |= cy15b104q_driver_write_memory_data(
    addr,
    data_out,
    sizeof(data_out)
  );

  TEST_ASSERT_EQUAL(CY15B104Q_STATUS_OK, status);

  status |= cy15b104q_driver_read_memory_data(addr, data_in, sizeof(data_in));

  TEST_ASSERT_EQUAL(CY15B104Q_STATUS_OK, status);
  TEST_ASSERT_EQUAL_INT8_ARRAY(
    data_out,
    data_in,
    sizeof(data_in)
  );
}

TEST(cy15b104q_driver_test, write_and_read_large_memory_data_is_ok)
{
  cy15b104q_driver_address addr = {
    .full = 0x0U
  };
  uint8_t data_out[1024U] = { 0 };
  uint8_t data_in[1024U] = { 0 };
  *(uint16_t*)&data_out[0] = TEST_PATTERN;
  *(uint16_t*)&data_out[256] = TEST_PATTERN;
  *(uint16_t*)&data_out[512] = TEST_PATTERN;
  *(uint16_t*)&data_out[768] = TEST_PATTERN;
  *(uint16_t*)&data_out[1022] = TEST_PATTERN;

  cy15b104q_driver_status status = cy15b104q_driver_write_enable();
  status |= cy15b104q_driver_write_memory_data(
    addr,
    data_out,
    sizeof(data_out)
  );

  TEST_ASSERT_EQUAL(CY15B104Q_STATUS_OK, status);

  status |= cy15b104q_driver_read_memory_data(addr, data_in, sizeof(data_in));

  TEST_ASSERT_EQUAL(CY15B104Q_STATUS_OK, status);
  TEST_ASSERT_EQUAL_INT8_ARRAY(
    data_out,
    data_in,
    sizeof(data_in)
  );
}

TEST(cy15b104q_driver_test, write_and_read_last_memory_data_is_ok)
{
  cy15b104q_driver_address addr = {
    .full = CY15B104Q_SIZE - 4
  };
  uint8_t data_out[] = { 0xaa, 0xbb, 0xcc };
  uint8_t data_in[3] = { 0 };

  cy15b104q_driver_status status = cy15b104q_driver_write_enable();
  status |= cy15b104q_driver_write_memory_data(
    addr,
    data_out,
    sizeof(data_out)
  );

  TEST_ASSERT_EQUAL(CY15B104Q_STATUS_OK, status);

  status |= cy15b104q_driver_read_memory_data(addr, data_in, sizeof(data_in));

  TEST_ASSERT_EQUAL(CY15B104Q_STATUS_OK, status);
  TEST_ASSERT_EQUAL_INT8_ARRAY(
    data_out,
    data_in,
    sizeof(data_in)
  );
}

TEST(cy15b104q_driver_test, byte_read_is_ok)
{
  cy15b104q_driver_address addr = {
    .full = 150
  };
  uint8_t data_out[] = { 0xaa, 0xbb, 0xcc };
  uint8_t data_in = 0;

  cy15b104q_driver_status status = cy15b104q_driver_write_enable();
  status |= cy15b104q_driver_write_memory_data(
    addr,
    data_out,
    sizeof(data_out)
  );

  TEST_ASSERT_EQUAL(CY15B104Q_STATUS_OK, status);

  for (uint8_t i = 0; i < sizeof(data_out); i++)
  {
    status |= cy15b104q_driver_read_memory_data(
      (cy15b104q_driver_address) {
        .full = addr.full + i
      },
      &data_in,
      sizeof(data_in)
    );

    TEST_ASSERT_EQUAL(CY15B104Q_STATUS_OK, status);
    TEST_ASSERT_EQUAL_INT8(
      data_out[i],
      data_in
    );
  }
}
