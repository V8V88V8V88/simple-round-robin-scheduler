#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PROCESSES 10
#define TIME_QUANTUM 4

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
void print_state();

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

void print_state() {
    printf("Time: %d\n", current_time);
    if (current_process != NULL) {
        printf("Running: Process %d (Remaining Time: %d)\n", 
               current_process->pid, current_process->remaining_time);
    } else {
        printf("No process running\n");
    }
    
    printf("Ready Queue: ");
    if (is_queue_empty()) {
        printf("Empty");
    } else {
        int index = ready_queue.front;
        for (int i = 0; i < ready_queue.size; i++) {
            printf("P%d(%d) ", ready_queue.processes[index]->pid, 
                   ready_queue.processes[index]->remaining_time);
            index = (index + 1) % MAX_PROCESSES;
        }
    }
    printf("\n\n");
}

int main() {
    init_queue();
    
    create_process(1, 10);
    create_process(2, 15);
    create_process(3, 8);
    
    for (current_time = 0; current_time < 100; current_time++) {
        schedule();
        run_process(current_process);
        print_state();
    }
    
    return 0;
}
