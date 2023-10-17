
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <limits.h>
#define PS_MAX 10

// holds the scheduling data of one process
typedef struct {
    int idx; // process idx (index)
    int at, bt, rt, wt, ct, tat; // arrival time, burst time, response time, waiting time, completion time, turnaround time.
    int burst; // remaining burst (this should decrement when the process is being executed)
} ProcessData;

// the idx of the running process
int running_process = -1; // -1 means no running processes

// the total time of the timer
unsigned total_time; // should increment one second at a time by the scheduler

// data of the processes
ProcessData data[PS_MAX]; // array of process data

// array of all process pids
pid_t ps[PS_MAX]; // zero valued pids - means the process is terminated or not created yet

// size of data array
unsigned data_size;

void read_file(FILE* file){

    // TODO: extract the data of processes from the {file}
    // and store them in the array {data}
    // initialize ps array to zeros (the process is terminated or not created yet)


    int i = 0;
    char line[256];  // Buffer for reading lines from the file

    while (fgets(line, sizeof(line), file) != NULL) {
        int idx, at, bt;
        int parsed = sscanf(line, "%d %d %d", &idx, &at, &bt);

        // Check if exactly three values were parsed
        if (parsed == 3) {
            data[i].idx = idx;
            data[i].at = at;
            data[i].bt = bt;
            data[i].rt = -1;
            data[i].ct = data[i].tat = data[i].wt = 0;
            data[i].burst = bt;
            ps[i] = 0;
            i++;
        }
    }
    data_size = i;
}

void resume(pid_t process) {
    if (process > 0 && ps[running_process] != 0) {
        kill(process, SIGCONT);
    }
}

void suspend(pid_t process) {
    if (process > 0 && ps[running_process] != 0) {
        kill(process, SIGTSTP);
    }
}

void terminate(pid_t process) {
    if (process > 0 && ps[running_process] != 0) {
        kill(process, SIGTERM);
    }
}

void create_process(int new_process) {
    // Stop the running process if there is one
    if (running_process >= 0) {
        suspend(ps[running_process]);
    }

    // Fork a new process and add it to the ps array
    pid_t child_pid = fork();
    if (child_pid == 0) {
        // Child process
        char new_process_str[10];
        snprintf(new_process_str, sizeof(new_process_str), "%d", new_process);
        execlp("./worker", "./worker", new_process_str, NULL);
       
        // If execlp fails, exit the child process
        exit(EXIT_FAILURE);
    } else if (child_pid > 0) {
        // Parent process
        ps[new_process] = child_pid;
        running_process = new_process;
        printf("Scheduler: Starting Process %d (Remaining Time: %d)\n", new_process, data[new_process].burst);
    }
}

ProcessData find_next_process() {
    int location = -1; // Location of the next process in the data array
    int min_burst = INT_MAX;

    for (int i = 0; i < data_size; i++) {
        if (data[i].burst > 0 && data[i].burst < min_burst) {
            location = i;
            min_burst = data[i].burst;
        }
    }

    if (data[location].at > total_time || location == -1) {
        printf("Scheduler: Runtime: %u seconds. No process to run.\n", total_time);
        total_time++;
        return find_next_process();
    }

    return data[location];
}

//reports the metrics and simulation results
void report(){
    printf("Simulation results.....\n");
    int sum_wt = 0;
    int sum_tat = 0;
    for (int i=0; i< data_size; i++){
        printf("process %d: \n", i);
         printf("  at=%d\n", data[i].at);
        printf("  bt=%d\n", data[i].bt);
        printf("  ct=%d\n", data[i].ct);
        printf("  tat=%d\n", data[i].tat);
        printf("  wt=%d\n", data[i].wt);
        printf("\n");

        sum_wt = data[i].wt;
        sum_tat = data[i].tat;
    }

    double avg_wt = (double)sum_wt / data_size;
    double avg_tat = (double)sum_tat / data_size;

    printf("Average Waiting Time: %.2lf\n", avg_wt);
    printf("Average Turnaround Time: %.2lf\n", avg_tat);
}

void schedule_handler(int signum) {
    ProcessData current_process = data[running_process];

    // Decrement the remaining burst time of the current process
    current_process.burst--;

    if (current_process.rt == -1) {
        current_process.rt = total_time - current_process.at;
    }

    total_time++;

    // Check if the current process has completed
    if (current_process.burst == 0) {
        current_process.ct = total_time;
        current_process.tat = current_process.ct - current_process.at;
        current_process.wt = current_process.tat - current_process.bt;

        data[running_process] = current_process;
        printf("Scheduler: Process %d completed. CT=%d\n", running_process, current_process.ct);

        // Check if there are any remaining processes
        int remaining_processes = 0;
        for (int i = 0; i < data_size; i++) {
            if (data[i].burst > 0) {
                remaining_processes = 1;
                break;
            }
        }

        if (remaining_processes) {
            ProcessData next_process = find_next_process();
            create_process(next_process.idx);
        } else {
       	    terminate(ps[running_process]);
       	   
            report();
            exit(EXIT_SUCCESS);
        }
    } else {
        data[running_process] = current_process;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE* input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        printf("Error opening input file.\n");
        exit(EXIT_FAILURE);
    }

    read_file(input_file);
    fclose(input_file);

    signal(SIGALRM, schedule_handler);

    ProcessData next_process = find_next_process();
    create_process(next_process.idx);

    // Set up timer to interrupt every second
    struct itimerval timer;
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;

    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
        printf("Error setting up timer.\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        pause();
    }

    return 0;
}

