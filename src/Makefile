INCLUDE_DIR = ../include
CFLAGS = -I$(INCLUDE_DIR) 

OBJS = cpu_thermal_monitor.o rpi_hardware_monitor.o  
TARGET = rpi_hardware_monitor

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS)

rpi_hardware_monitor.o: rpi_hardware_monitor.c $(INCLUDE_DIR)/cpu_thermal_monitor.h 

cpu_thermal_monitor.o: cpu_thermal_monitor.c $(INCLUDE_DIR)/cpu_thermal_monitor.h

clean:
	rm -rf *.o *~ $(TARGET)
