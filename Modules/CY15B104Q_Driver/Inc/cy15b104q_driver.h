#ifndef CY15B104Q_DRIVER_H
#define CY15B104Q_DRIVER_H

#include "cy15b104q_driver_defs.h"

void cy15b104q_driver_init_module(
  cy15b104q_driver_io_struct io_struct
);
void cy15b104q_driver_deinit_module(void);
cy15b104q_driver_status cy15b104q_driver_power_up(void);
cy15b104q_driver_status cy15b104q_driver_check_link(void);
cy15b104q_driver_status cy15b104q_driver_read_id(
  cy15b104q_driver_id *const result
);
cy15b104q_driver_status cy15b104q_driver_sleep(void);
cy15b104q_driver_status cy15b104q_driver_sleep_recover(void);

#endif
