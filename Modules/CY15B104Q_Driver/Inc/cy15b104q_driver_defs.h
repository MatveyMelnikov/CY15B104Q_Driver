#ifndef CY15B104Q_DRIVER_DEFS_H
#define CY15B104Q_DRIVER_DEFS_H

#include <stdint.h>
#include <stdbool.h>

enum {
  CY15B104Q_CMD_ID = 0x9fU,
  CY15B104Q_CMD_SLEEP = 0xb9U,
  CY15B104Q_CMD_STATUS = 0x5U,
  CY15B104Q_POWER_UP_DELAY = 0x1U, // CY15B104Q datasheet, pg. 6
  CY15B104Q_IO_TIMEOUT = 0xaU,
  CY15B104Q_SLEEP_RECOVER_DELAY = 2U,
  CY15B104Q_PRODUCT_ID = 0x826U, // MSB, little-endian
};

typedef enum {
  CY15B104Q_STATUS_OK = 0x0U,
  CY15B104Q_STATUS_ERROR = 0x1U,
  CY15B104Q_STATUS_BUSY = 0x2U,
  CY15B104Q_STATUS_TIMEOUT = 0x3U,
  CY15B104Q_STATUS_IS_NOT_INIT = 0x4U,
} cy15b104q_driver_status;

typedef struct {
  // data, size, timeout
  cy15b104q_driver_status (*transmit)(
    const uint8_t *const,
    const uint16_t,
    const uint32_t
  );
  // data, size, timeout
  cy15b104q_driver_status (*receive)(
    uint8_t *const,
    const uint16_t,
    const uint32_t
  );
  // is_set
  void (*write_cs_pin)(const bool);
  // timeout
  void (*delay)(uint32_t);
} cy15b104q_driver_io_struct;

typedef struct {
  // CY15B104Q datasheet, pg. 11
  uint8_t manufacturer_id[7U];
  uint8_t product_id[2U];
} cy15b104q_driver_id;

#endif