INCLUDE_DIR = ../include
CFLAGS = -I$(INCLUDE_DIR) 

OBJS = temperature_monitor.o fan_control.o monitor.o rpi5_thermal_trips.o
TARGET = monitor

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) -lm

monitor.o: monitor.c $(INCLUDE_DIR)/temperature_monitor.h $(INCLUDE_DIR)/fan_control.h 

temperature_monitor.o: temperature_monitor.c $(INCLUDE_DIR)/temperature_monitor.h  

fan_control.o: fan_control.c $(INCLUDE_DIR)/fan_control.h

rpi5_thermal_trips.o: rpi5_thermal_trips.c $(INCLUDE_DIR)/rpi5_thermal_trips.h


clean:
	rm -rf *.o *~ $(TARGET)
