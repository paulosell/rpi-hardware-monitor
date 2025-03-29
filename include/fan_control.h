#ifndef FAN_CONTROL_H
#define FAN_CONTROL_H

#include "rpi5_thermal_trips.h"

#define PWM_CHIP_FILE "/sys/class/pwm/pwmchip2/"
#define PWM_EXPORT_FILE PWM_CHIP_FILE "export"
#define PWM_UNEXPORT_FILE PWM_CHIP_FILE "unexport"
#define PWM_DEVICE_FILE PWM_CHIP_FILE "pwm3/"
#define PWM_ENABLE_FILE PWM_DEVICE_FILE "enable"
#define PWM_DUTY_CYCLE_FILE PWM_DEVICE_FILE "duty_cycle"

#define PWM_CHANNEL 3
#define PWM_PERIOD 41556

extern float period_multiplier_map[THERMAL_TRIPS_LEN];

typedef struct fan_control {
  int enable;
  int duty_cycle;
  thermal_trips_t state;
  thermal_trips_t previous_state;
} fan_control_t;

int fan_control_init(fan_control_t * fan);
int fan_control_stop(void);
int fan_control_set_duty_cycle(fan_control_t * fan);
int fan_control_set_state(fan_control_t * fan, thermal_trips_t state);

#endif /* FAN_CONTROL_H */
