
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <signal.h>
#include <unistd.h>

#include "cpu_thermal_monitor.h"

volatile int exit_flag = 0;  

void alarm_handler(int signum) {

  if (exit_flag < 0) {
      syslog(LOG_INFO, "Exiting temperature monitor");
      exit(0);
  }

  exit_flag = monitor_cpu_temperature();

  alarm(5);
}


int main(){

  signal(SIGALRM, alarm_handler);

  syslog(LOG_INFO, "Initializing temperature monitor");

  alarm(1);

  while (1) {
      pause();  
  }

  return 0;
}
