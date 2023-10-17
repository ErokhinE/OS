#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
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
    if (process > 0&&ps[running_process]!=0) {
        kill(process, SIGCONT);
    }
}

void suspend(pid_t process) {
    if (process > 0&&ps[running_process]!=0) {
        kill(process, SIGTSTP);
    }
}

void terminate(pid_t process) {
    if (process > 0&&ps[running_process]!=0) {
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

    for (int i = 0; i < data_size; i++) {
        if (data[i].burst > 0) {
            if (location == -1) {
                location = i;
            } else {
                if (data[i].at < data[location].at) {
                    location = i;
                }
            }
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
        printf("	at=%d\n", data[i].at);
        printf("	bt=%d\n", data[i].bt);
        printf("	ct=%d\n", data[i].ct);
        printf("	wt=%d\n", data[i].wt);
        printf("	tat=%d\n", data[i].tat);
        printf("	rt=%d\n", data[i].rt);
        sum_wt += data[i].wt;
        sum_tat += data[i].tat;
    }

    printf("data size = %d\n", data_size);
    float avg_wt = (float)sum_wt/data_size;
    float avg_tat = (float)sum_tat/data_size;
    printf("Average results for this run:\n");
    printf("	avg_wt=%f\n", avg_wt);
    printf("	avg_tat=%f\n", avg_tat);
}

void check_burst() {

    for (int i = 0; i < data_size; i++) {
        if (data[i].burst > 0) {
            return;
        }
    }

    // Report simulation results
    report();

    // Terminate the scheduler
    exit(EXIT_SUCCESS);
}

void schedule_handler(int signum) {
    total_time++;

    if (running_process >= 0) {
        data[running_process].burst--;

        printf("Scheduler: Runtime: %u seconds\n", total_time);
        printf("Process %d is running with %d seconds left\n", running_process, data[running_process].burst);

        if (data[running_process].burst == 0) {
            printf("Scheduler: Terminating Process %d (Remaining Time: 0)\n", running_process);
            terminate(ps[running_process]);
            // Wait for process termination
            waitpid(ps[running_process], NULL, 0);

            // Calculate metrics (CT, TAT, WT, RT)
            data[running_process].ct = total_time;
            data[running_process].tat = data[running_process].ct - data[running_process].at;
            data[running_process].wt = data[running_process].tat - data[running_process].bt;
            data[running_process].rt = data[running_process].tat - data[running_process].bt;

            // Mark the process as terminated
            ps[running_process] = 0;

            // Check for the next process
            running_process = running_process-1;
        }
    }

    check_burst();
    // Find the next process to run based on FCFS
    ProcessData next_process = find_next_process();

    if (next_process.at > total_time) {
        printf("Process %d: has not arrived yet\n", next_process.idx);
        total_time++;
        return;
    }

    if (running_process != next_process.idx) {
        if (running_process >= 0) {
            suspend(ps[running_process]);
            printf("Scheduler: Stopping Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
        }
        running_process = next_process.idx;

        if (ps[running_process] == 0) {
            terminate(ps[running_process]);
            create_process(running_process);
        } else {
            resume(ps[running_process]);
            printf("Scheduler: Resuming Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
        }
    }

}

int main(int argc, char *argv[]) {
    // read the data file
    FILE *in_file  = fopen(argv[1], "r");
    if (in_file == NULL) {
        printf("File is not found or cannot open it!\n");
        exit(EXIT_FAILURE);
    } else {
        read_file(in_file);
    }

    // set a timer
    struct itimerval timer;

    // the timer goes off 1 second after reset
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;

    // timer increments 1 second at a time
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

    // this counts down and sends SIGALRM to the scheduler process after expiration.
    setitimer(ITIMER_REAL, &timer, NULL);

    // register the handler for SIGALRM signal
    signal(SIGALRM, schedule_handler);

    // Wait till all processes finish
    while(1); // infinite loop
}
