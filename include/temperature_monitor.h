#ifndef CPU_THERMAL_MONITOR_H
#define CPU_THERMAL_MONITOR_H

#include "rpi5_thermal_trips.h"

#define SYSFS_THERMAL_FILE "/sys/class/thermal/thermal_zone0/temp"
#define RPI_CPU_MAX_TEMP 75.0
#define TEMPERATURE_MULTIPLIER 0.001

typedef struct temperature_monitor {
  float current_temperature;
  thermal_trips_t state;
} temperature_monitor_t;

int temperature_update(temperature_monitor_t * temp);

#endif /* CPU_THERMAL_MONITOR_H */
