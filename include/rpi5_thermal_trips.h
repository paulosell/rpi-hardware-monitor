#ifndef RPI5_THERMAL_TRIPS_H
#define RPI5_THERMAL_TRIPS_H

#define HYSTERESIS_CELSIUS 5.0

#define TEPID_CELSIUS 50.0
#define WARM_CELSIUS 60.0
#define HOT_CELSIUS 67.5
#define VERY_HOT_CELSIUS 75.0 

typedef enum {
  NORMAL_TEMP,
  TEPID,
  WARM,
  HOT,
  VHOT,
  THERMAL_TRIPS_LEN
} thermal_trips_t;

extern char thermal_trips[THERMAL_TRIPS_LEN][64];

#endif /* RPI5_THERMAL_TRIPS_H */
