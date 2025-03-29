#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "temperature_monitor.h"
#include "fan_control.h"

#define PID_FILE "/var/run/monitor.pid"

fan_control_t fan;
temperature_monitor_t temp;

volatile int exit_flag = 0;  

int monitor_cpu_temperature() {
  if (temperature_update(&temp) < 0)
    return -1;
    
  if (fan_control_set_state(&fan, temp.state) < 0)
    return -1;

  if (fan.state != fan.previous_state) {
    syslog(LOG_INFO, "[MONITOR]: Current CPU temperature %.2f", temp.current_temperature);
    if (fan_control_set_duty_cycle(&fan) < 0)
      return -1;
  }

  return 0;
}

void exit_monitor(int signum) {
  exit_flag = -1;
}

void alarm_handler(int signum) {

  if (exit_flag < 0) {
    syslog(LOG_INFO, "[MONITOR]: Exiting active temperature monitor");
    fan_control_stop();
    unlink(PID_FILE);  
    closelog();
    exit(0);
  }

  exit_flag = monitor_cpu_temperature();

  alarm(5);
}

int daemonize() {

  pid_t pid = fork();

  if (pid < 0) {
    syslog(LOG_ERR, "[MONITOR]: Failed to fork process for daemonization");
    return -1;
  }

  if (pid > 0) {
    exit(0);  // Parent exits
  }

  // Create a new session and process group
  pid_t sid = setsid();
  if (sid < 0) {
    syslog(LOG_ERR, "[MONITOR]: Failed to create new session");
    return -1;
  }

  // Change the working directory to root
  if (chdir("/") < 0) {
    syslog(LOG_ERR, "[MONITOR]: Failed to change directory to /");
    return -1;
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  FILE *pid_file = fopen(PID_FILE, "w");
  if (pid_file == NULL) {
    syslog(LOG_ERR, "[MONITOR]: Failed to open PID file");
    return -1;
  }

  fprintf(pid_file, "%d", getpid());
  fclose(pid_file);

  return 0;
}


int main(){ 

  openlog("monitor", LOG_PID | LOG_CONS, LOG_DAEMON);

  if (daemonize() < 0) {
    syslog(LOG_ERR, "[MONITOR]: Daemonization failed");
    return -1;
  }

  signal(SIGALRM, alarm_handler);
  signal(SIGTERM, exit_monitor);  // Handle SIGTERM properly
  signal(SIGINT, exit_monitor);   // Handle Ctrl+C (for debugging)

  syslog(LOG_INFO, "[MONITOR]: Starting active temperature monitor");
  
  if (fan_control_init(&fan) < 0) {
    syslog(LOG_INFO, "[MONITOR]: Error in fan controller initialization");
    return -1;
  }

  alarm(1);

  while (1) {
      pause();  
  }

  // should never reach here
  syslog(LOG_INFO, "[MONITOR]: Stopping active temperature monitor");
  fan_control_stop();
  unlink(PID_FILE);  
  closelog();

  return 0;
}
