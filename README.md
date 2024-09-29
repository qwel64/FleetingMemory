# FleetingMemory
Memory Analysis Tool

# Memory Analysis Tool

This program is a memory analysis tool that operates on Linux systems. It analyzes user space memory usage and detects potential memory leaks. Additionally, it monitors process memory usage in real-time.

## Features

1. User Space Memory Analysis
   - Analyzes memory usage of all processes
   - Detects potential memory leaks (when data segment is approximately 4GB or larger)

2. Real-time Memory Monitoring
   - Displays memory usage of all processes every second
   - Shows Process ID, process name, and memory usage

## Usage

1. Compile the program:
   ```
   g++ -std=c++17 -o Fleeting_Memory FleetingMemory.cpp
   ```

2. Run with root privileges:
   ```
   sudo ./Fleeting_Memory
   ```

3. The program operates in the following order:
   - Executes user space memory analysis
   - Starts real-time memory monitoring (terminate with Ctrl+C)

## Recommended Environment

- Operating System: Linux (Ubuntu 20.04 LTS or later recommended)
- Compiler: GCC 9.0 or later
- Required Libraries: Standard C++ Library

## Notes

- This program requires root privileges to access the `/proc` filesystem.
- The memory leak detection threshold (currently 4GB) should be adjusted according to your system requirements.

## Limitations

- The current version does not implement kernel space memory analysis.
- Process-specific memory usage patterns are not considered, which may lead to false positives.

## License

This program is released under the MIT License. See the `LICENSE` file for details.
