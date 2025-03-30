#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>
#include <stdarg.h>

#include "temperature_monitor.h"

static temperature_monitor_t cpu_temp = {
  .state = NORMAL_TEMP,
};

void temperature_monitor_syslog(int log_level, const char * fmt, ...){
  va_list args;
  va_start(args, fmt);

  char message[256]; 
  snprintf(message, sizeof(message), "[TEMPERATURE MONITOR] %s", fmt);
  
  vsyslog(log_level, message, args);
    
  va_end(args);
}

int read_cpu_temperature(temperature_monitor_t * temp){
  int temp_milli;
  FILE * fp;

  fp = fopen(SYSFS_THERMAL_FILE, "r");

  if (fp == NULL) {
    temperature_monitor_syslog(LOG_ERR, "Failed to open thermal file %s", SYSFS_THERMAL_FILE);
    return -1;
  }

  if (fscanf(fp, "%d", &temp_milli) != 1) {
    temperature_monitor_syslog(LOG_ERR, "Failed to read temperature value from file");
    fclose(fp);
    return -1;
  }

  fclose(fp);

  temp->current_temperature = (float) temp_milli * TEMPERATURE_MULTIPLIER;

  return 0;
  
}

int update_temperature_state(temperature_monitor_t * temp){

  if (!temp) return -1;

  switch(temp->state) {
    case NORMAL_TEMP: 
      if (temp->current_temperature > TEPID_CELSIUS) {
        temp->state = TEPID;
      }
      break;
    case TEPID:
      if (temp->current_temperature > WARM_CELSIUS) {
        temp->state = WARM;
      } 
      if (temp->current_temperature <= TEPID_CELSIUS - HYSTERESIS_CELSIUS) {
        temp->state = NORMAL_TEMP;
      }
      break;
    case WARM:
      if (temp->current_temperature > HOT_CELSIUS) {
        temp->state = HOT;
      } 
      if (temp->current_temperature <= WARM_CELSIUS - HYSTERESIS_CELSIUS) {
        temp->state = TEPID;
      }
      break;
    case HOT:
      if (temp->current_temperature > VERY_HOT_CELSIUS) {
        temp->state = VHOT;
      } 
      if (temp->current_temperature <= HOT_CELSIUS - HYSTERESIS_CELSIUS) {
        temp->state = WARM;
      }
      break;
    case VHOT:
      if (temp->current_temperature <= VERY_HOT_CELSIUS - HYSTERESIS_CELSIUS) {
        temp->state = HOT;
      }
      break;
    default:
      return -1;
  }

  return 0;

}

int temperature_update(temperature_monitor_t * temp){

  int status = 0;

  if (read_cpu_temperature(temp) < 0){
    temperature_monitor_syslog(LOG_ERR, "Error getting current CPU temperature");
    return -1;
  }

  status = update_temperature_state(temp);
  
  if (status < 0) {
    temperature_monitor_syslog(LOG_ERR, "Error processing temperature state machine");
    return -1;
  }

  return 0;

}
