#include "cy15b104q_driver.h"
#include <stddef.h>

// Static variables ----------------------------------------------------------

static cy15b104q_driver_io_struct module_io;

// Static prototypes ---------------------------------------------------------

__attribute__((always_inline))
static inline cy15b104q_driver_status cy15b104q_driver_io_transmit(
  const uint8_t *const data,
  const uint16_t size
);
__attribute__((always_inline))
static inline cy15b104q_driver_status cy15b104q_driver_io_receive(
  uint8_t *const data,
  const uint16_t size
);
__attribute__((always_inline))
static inline cy15b104q_driver_status cy15b104q_driver_io_write_cs_pin(
  const bool is_set
);
__attribute__((always_inline))
static inline cy15b104q_driver_status cy15b104q_driver_io_delay(
  uint32_t timeout
);

// Implementations -----------------------------------------------------------

void cy15b104q_driver_init_module(
  cy15b104q_driver_io_struct io_struct
)
{
  module_io = io_struct;
}

void cy15b104q_driver_deinit_module()
{
  module_io = (cy15b104q_driver_io_struct) { 0 };
}

cy15b104q_driver_status cy15b104q_driver_power_up()
{
  // CY15B104Q datasheet, pg. 6
  cy15b104q_driver_status status = cy15b104q_driver_io_write_cs_pin(true);
  status |= cy15b104q_driver_io_delay(CY15B104Q_POWER_UP_DELAY);

  return status;
}

cy15b104q_driver_status cy15b104q_driver_check_link(void)
{
  cy15b104q_driver_id id;

  cy15b104q_driver_status status = cy15b104q_driver_read_id(&id);
  if (*(uint16_t*)id.product_id == CY15B104Q_PRODUCT_ID)
    return status;
  
  return status | CY15B104Q_STATUS_ERROR;
}

cy15b104q_driver_status cy15b104q_driver_read_id(
  cy15b104q_driver_id *const result
)
{
  // CY15B104Q datasheet, pg. 11
  uint8_t data_out = CY15B104Q_CMD_ID;

  cy15b104q_driver_status status = cy15b104q_driver_io_write_cs_pin(false);
  status |= cy15b104q_driver_io_transmit(&data_out, sizeof(data_out));
  status |= cy15b104q_driver_io_receive(
    (uint8_t*)result,
    sizeof(cy15b104q_driver_id)
  );
  status |= cy15b104q_driver_io_write_cs_pin(true);

  return status;
}

cy15b104q_driver_status cy15b104q_driver_sleep()
{
  // CY15B104Q datasheet, pg. 10
  uint8_t data_out = CY15B104Q_CMD_SLEEP;

  cy15b104q_driver_status status = cy15b104q_driver_io_write_cs_pin(false);
  status |= cy15b104q_driver_io_transmit(&data_out, sizeof(data_out));
  status |= cy15b104q_driver_io_write_cs_pin(true);

  return status;
}

cy15b104q_driver_status cy15b104q_driver_sleep_recover(void)
{
  // CY15B104Q datasheet, pg. 10
  uint8_t cmd = CY15B104Q_CMD_STATUS; // dummy read
  uint8_t dummy_data;

  // exit sleep mode
  cy15b104q_driver_status status = cy15b104q_driver_io_write_cs_pin(false);
  status |= cy15b104q_driver_io_delay(CY15B104Q_SLEEP_RECOVER_DELAY);
  status |= cy15b104q_driver_io_transmit(&cmd, sizeof(cmd));
  status |= cy15b104q_driver_io_receive(&dummy_data, sizeof(dummy_data));
  status |= cy15b104q_driver_io_write_cs_pin(true);
  status |= cy15b104q_driver_io_delay(CY15B104Q_SLEEP_RECOVER_DELAY);

  return status;
}

__attribute__((always_inline))
static inline cy15b104q_driver_status cy15b104q_driver_io_transmit(
  const uint8_t *const data,
  const uint16_t size
)
{
  if (module_io.transmit == NULL)
    return CY15B104Q_STATUS_IS_NOT_INIT;

  return module_io.transmit(data, size, CY15B104Q_IO_TIMEOUT);
}

__attribute__((always_inline))
static inline cy15b104q_driver_status cy15b104q_driver_io_receive(
  uint8_t *const data,
  const uint16_t size
)
{
  if (module_io.receive == NULL)
    return CY15B104Q_STATUS_IS_NOT_INIT;

  return module_io.receive(data, size, CY15B104Q_IO_TIMEOUT);
}

__attribute__((always_inline))
static inline cy15b104q_driver_status cy15b104q_driver_io_write_cs_pin(
  const bool is_set
)
{
  if (module_io.write_cs_pin == NULL)
    return CY15B104Q_STATUS_IS_NOT_INIT;
  
  module_io.write_cs_pin(is_set);

  return CY15B104Q_STATUS_OK;
}

__attribute__((always_inline))
static inline cy15b104q_driver_status cy15b104q_driver_io_delay(
  uint32_t timeout
)
{
  if (module_io.delay == NULL)
    return CY15B104Q_STATUS_IS_NOT_INIT;

  module_io.delay(timeout);

  return CY15B104Q_STATUS_OK;
}
