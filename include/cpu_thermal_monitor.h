#ifndef CPU_THERMAL_MONITOR_H
#define CPU_THERMAL_MONITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>

#define SYSFS_THERMAL_FILE "/sys/class/hwmon/hwmon3/temp2_input"
#define RPI_CPU_MAX_TEMP 85.0 
#define TEMPERATURE_MULTIPLIER 0.001

enum {
  NORMAL_TEMP,
  OVERHEATING,
};

typedef struct temperature_monitor {
  float current_temperature;
  float max_temperature;
  float previous_temperature;
  int state;
} temperature_monitor_t;

int monitor_cpu_temperature	(void);

#endif /* CPU_THERMAL_MONITOR_H */
