#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_PROCESSES 10
#define TIME_QUANTUM 4

// ANSI color codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

typedef enum {
    READY,
    RUNNING,
    WAITING,
    COMPLETED
} ProcessState;

typedef struct {
    int registers[4];
} Context;

typedef struct {
    int pid;
    ProcessState state;
    int cpu_burst_time;
    int remaining_time;
    Context context;
} Process;

typedef struct {
    Process* processes[MAX_PROCESSES];
    int front;
    int rear;
    int size;
} ProcessQueue;

ProcessQueue ready_queue;
Process* current_process = NULL;
int current_time = 0;
int time_in_slice = 0;

void init_queue();
void enqueue(Process* process);
Process* dequeue();
bool is_queue_empty();
void create_process(int pid, int burst_time);
void schedule();
void run_process(Process* process);
void clear_screen();
void draw_tui_view();

void init_queue() {
    ready_queue.front = 0;
    ready_queue.rear = -1;
    ready_queue.size = 0;
}

void enqueue(Process* process) {
    if (ready_queue.size == MAX_PROCESSES) {
        printf("Queue full, cannot add more processes.\n");
        return;
    }
    ready_queue.rear = (ready_queue.rear + 1) % MAX_PROCESSES;
    ready_queue.processes[ready_queue.rear] = process;
    ready_queue.size++;
}

Process* dequeue() {
    if (is_queue_empty()) {
        return NULL;
    }
    Process* process = ready_queue.processes[ready_queue.front];
    ready_queue.front = (ready_queue.front + 1) % MAX_PROCESSES;
    ready_queue.size--;
    return process;
}

bool is_queue_empty() {
    return ready_queue.size == 0;
}

void create_process(int pid, int burst_time) {
    Process* new_process = (Process*)malloc(sizeof(Process));
    new_process->pid = pid;
    new_process->state = READY;
    new_process->cpu_burst_time = burst_time;
    new_process->remaining_time = burst_time;
    for (int i = 0; i < 4; i++) {
        new_process->context.registers[i] = 0;
    }
    enqueue(new_process);
}

void schedule() {
    if (current_process == NULL || time_in_slice >= TIME_QUANTUM || current_process->remaining_time == 0) {
        if (current_process != NULL && current_process->remaining_time > 0) {
            current_process->state = READY;
            enqueue(current_process);
        }

        current_process = dequeue();

        if (current_process != NULL) {
            current_process->state = RUNNING;
            time_in_slice = 0;
        }
    }
}

void run_process(Process* process) {
    if (process != NULL) {
        process->remaining_time--;
        time_in_slice++;

        if (process->remaining_time == 0) {
            process->state = COMPLETED;
        }
    }
}

void clear_screen() {
    printf("\033[H\033[J");
}

void draw_tui_view() {
    clear_screen();

    printf(BOLD BLUE "=== Simple TUI OS Scheduler ===\n" RESET);
    printf(CYAN "Time: %d\n" RESET, current_time);

    printf(BOLD YELLOW "\nRunning Process:\n" RESET);
    if (current_process != NULL) {
        printf(BOLD GREEN "Process %d" RESET " | Remaining Time: " BOLD RED "%d\n" RESET,
               current_process->pid, current_process->remaining_time);
    } else {
        printf(BOLD RED "No process running\n" RESET);
    }

    printf(BOLD MAGENTA "\nReady Queue:\n" RESET);
    if (is_queue_empty()) {
        printf(BOLD RED "Empty\n" RESET);
    } else {
        int index = ready_queue.front;
        for (int i = 0; i < ready_queue.size; i++) {
            printf(CYAN "P%d(%d) " RESET, ready_queue.processes[index]->pid, ready_queue.processes[index]->remaining_time);
            index = (index + 1) % MAX_PROCESSES;
        }
        printf("\n");
    }

    printf("\n" BOLD YELLOW "Press CTRL+C to stop the simulation...\n" RESET);
}

int main() {
    int num_processes, pid, burst_time;

    init_queue();

    printf(BOLD "Enter number of processes: " RESET);
    scanf("%d", &num_processes);

    for (int i = 0; i < num_processes; i++) {
        printf(BOLD "Enter Process %d burst time: " RESET, i + 1);
        scanf("%d", &burst_time);
        create_process(i + 1, burst_time);
    }

    while (1) {
        schedule();
        run_process(current_process);
        draw_tui_view();
        usleep(500000);

        current_time++;
    }

    return 0;
}
