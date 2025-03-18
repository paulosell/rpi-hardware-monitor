# Raspberry Pi 5 CPU Thermal Monitor

This project monitors the CPU temperature of a Raspberry Pi 5 system and reports if the temperature exceeds a predefined threshold. It can help you ensure that your system is not overheating and automatically log or respond to temperature changes.

## Installation

1. Clone or download the repository.
2. Ensure you have the necessary development tools (like `gcc` and `make`) installed.
3. Build the project:

   ```bash
   make all
   ```

4. After the build completes, the executable `rpi_hardware_monitor` will be generated in `src` folder.

## Running the Monitor

To run the CPU temperature monitor, execute the following command:

```bash
src/rpi_hardware_monitor
```

This will start monitoring the CPU temperature. If the CPU temperature exceeds the defined threshold, an alert will be logged to the syslog.

## Makefile Instructions

- `make all`: Builds the program.
- `make clean`: Cleans up object files and the compiled executable.

## License

This project is open-source and available under the MIT License.

## Author

pfs
