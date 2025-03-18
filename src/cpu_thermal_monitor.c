#include <signal.h>
#include "cpu_thermal_monitor.h"

static temperature_monitor_t cpu_temp = {
  .max_temperature = RPI_CPU_MAX_TEMP,
  .previous_temperature = 0.0,
  .state = NORMAL_TEMP,
};

int read_cpu_temperature(temperature_monitor_t * temp){
  int temp_milli;
  FILE * fp;

  fp = fopen(SYSFS_THERMAL_FILE, "r");

  if (fp == NULL) {
    syslog(LOG_ERR, "Failed to open thermal file %d", errno);
    return -1;
  }

  if (fscanf(fp, "%d", &temp_milli) != 1) {
    syslog(LOG_ERR, "Failed to read temperature value from file");
    fclose(fp);
    return -1;
  }

  fclose(fp);

  temp->current_temperature = (float) temp_milli * TEMPERATURE_MULTIPLIER;

  return 0;
  
}

int update_temperature_state(temperature_monitor_t * temp){

  switch(temp->state) {
    case OVERHEATING: {
      if (temp->current_temperature < temp->max_temperature) {
        temp->state = NORMAL_TEMP;
        syslog(LOG_INFO, "CPU is back in normal temperature: was %.2f C now %.2f C", temp->previous_temperature, temp->current_temperature);
      }
      break;
    }
    
    case NORMAL_TEMP: {
      if (temp->current_temperature >= temp->max_temperature) {
        temp->state = OVERHEATING;
        syslog(LOG_INFO, "CPU too hot at %.2f C", temp->current_temperature);
      }
      break;
    }
    default:
      return -1;
  }

  return 0;

}

int handle_temperature_update(temperature_monitor_t * temp){

  int status = 0;
  temp->previous_temperature = temp->current_temperature;

  if (read_cpu_temperature(temp) < 0){
    syslog(LOG_ERR, "Error getting current CPU temperature");
    return -1;
  }

  status = update_temperature_state(temp);
  
  if (status < 0) {
    syslog(LOG_ERR, "Error processing temperature state machine");
    return -1;
  }

  return 0;

}

int monitor_cpu_temperature(void){
 
  int status = 0;

  status = handle_temperature_update(&cpu_temp);

  if (status < 0) {
    syslog(LOG_INFO, "Error while monitoring CPU temperature");
  }

  return status;
}
