#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <sys/stat.h>

#include "fan_control.h"

float period_multiplier_map[THERMAL_TRIPS_LEN] = {0, 0.3, 0.5, 0.7, 1.0};

void fan_control_syslog(int log_level, const char * fmt, ...){
  va_list args;
  va_start(args, fmt);

  char message[256]; 
  snprintf(message, sizeof(message), "[FAN CONTROL] %s", fmt);
  
  vsyslog(log_level, message, args);
    
  va_end(args);
}

int write_file(const char * file_path, const char * data, size_t len){
  int fd;

  fd = open(file_path, O_WRONLY);

  if (fd < 0) {
    fan_control_syslog(LOG_ERR, "Error opening %s", file_path);
    return -1;
  }
  
  if (write(fd, data, len) < 0){
    close(fd);
    return -1;
  }

  close(fd);

  return 0;

}

int fan_control_export_channel(void){

  char aux[4];
  struct stat pwm_dir;

  snprintf(aux, sizeof(aux), "%d", PWM_CHANNEL);

  if (stat(PWM_DEVICE_FILE, &pwm_dir) == 0 && S_ISDIR(pwm_dir.st_mode)) {
    fan_control_syslog(LOG_INFO, "PWM Channel already exported");
    return 0;
    
  }
  
  if (write_file(PWM_EXPORT_FILE, aux, sizeof(aux)) < 0){
    return -1;
  }

  return 0;
}

int fan_control_enable_pwm(void) {
  char aux[4];

  snprintf(aux, sizeof(aux), "%d", 1);

  if (write_file(PWM_ENABLE_FILE, aux, sizeof(aux)) < 0){
    return -1;
  }

  return 0;

}

int fan_control_init(fan_control_t * fan){
  char aux[4];

  if (fan_control_export_channel() < 0) {
    fan_control_syslog(LOG_ERR, "Error exporting PWM channel");
    return -1;
  }

  if (fan_control_enable_pwm() < 0) {
    fan_control_syslog(LOG_ERR, "Error enabling PWM channel");
    return -1;
  }

  fan->enable = 1;

  return 0;  

}

int fan_control_set_state(fan_control_t * fan, thermal_trips_t state){
  
  if (state < NORMAL_TEMP || state >= THERMAL_TRIPS_LEN ) {
    fan_control_syslog(LOG_ERR, "State %d not in thermal trips range", state);
    return -1;
  }

  fan->previous_state = fan->state;
  fan->state = state;

  return 0;
}


void fan_control_calculate_duty_cycle(fan_control_t * fan){

  fan->duty_cycle = ceil(PWM_PERIOD * period_multiplier_map[fan->state]);

  if (fan->state == VHOT)
    fan->duty_cycle -= 1;
  
  return;
}

int fan_control_set_duty_cycle(fan_control_t * fan){
  char aux[16];

  fan_control_calculate_duty_cycle(fan);

  snprintf(aux, sizeof(aux), "%d", fan->duty_cycle);

  if (write_file(PWM_DUTY_CYCLE_FILE, aux, sizeof(aux)) < 0) {
    fan_control_syslog(LOG_ERR, "Error setting PWM duty cycle");
    return -1;
  }

  fan_control_syslog(LOG_INFO, "PWM duty cycle set to %d in %s state", fan->duty_cycle, thermal_trips[fan->state]);
  
  return 0;

}

int fan_control_stop(void){
  char aux[4];

  snprintf(aux, sizeof(aux), "%d", PWM_CHANNEL);

  if (write_file(PWM_UNEXPORT_FILE, aux, sizeof(aux)) < 0) {
    fan_control_syslog(LOG_ERR, "Error unexporting PWM channel");
    return -1;

  }

  return 0;

}
