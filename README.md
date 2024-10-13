# Simple Round-Robin Scheduler

This project implements a basic process scheduler that can be used in a toy operating system kernel. The scheduler manages multiple processes, switches between them, and ensures each process gets its share of CPU time using a round-robin scheduling algorithm.

## Features

1. **Process Structure:**
   - Each process has a unique ID, state, CPU burst time, remaining time, and a simplified context structure.

2. **Process States:**
   - READY: The process is ready to be executed.
   - RUNNING: The process is currently running.
   - WAITING: The process is waiting for an I/O operation to complete.

3. **Scheduler Logic:**
   - Implements round-robin scheduling with a fixed time quantum (4 time units).
   - Handles process completion and waiting states.
   - Performs context switching between processes.

4. **Timer Interrupt:**
   - Simulates a timer interrupt to force context switches after the time quantum expires.

5. **Context Switching:**
   - Saves and loads process contexts during switches.

6. **Process Queue:**
   - Uses a circular queue for managing processes in round-robin fashion.

7. **Simulation:**
   - Simulates process execution, reducing remaining time for each process.
   - Removes completed processes from the queue.

8. **Example Setup:**
   - Creates 3 processes with different CPU burst times.
   - Simulates the scheduler running these processes for 100 time units.

9. **Output:**
   - Prints the state of the system at each time unit, including the running process, remaining time for each process, and current time.

## Project Structure

```
simple-round-robin-scheduler/
│
├── src/
│   └── scheduler.c
│
├── README.md
└── .gitignore
```

## How to Compile and Run

1. Ensure you have a C compiler installed (e.g., GCC).
2. Clone the repository:
   ```
   git clone https://github.com/v8v88v8v88/simple-round-robin-scheduler.git
   cd simple-round-robin-scheduler
   ```
3. Compile the program:
   ```
   gcc -o scheduler src/scheduler.c
   ```
4. Run the program:
   ```
   ./scheduler
   ```

The program will output the state of the system at each time step, showing which process is running and the state of the ready queue.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
