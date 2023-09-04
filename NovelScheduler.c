#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PROC 15

struct Process {
    int id;
    int remaining_burst_time;
    int response_time;
    int wait_time;
    int turnaround_time;
};

void generateProcesses(struct Process* processes[]) {
    srand(time(0));

    for (int i = 0; i < MAX_PROC; i++) {
        processes[i] = (struct Process*)malloc(sizeof(struct Process));
        processes[i]->id = i + 1;
        processes[i]->remaining_burst_time = 5 + rand() % 26;
        processes[i]->response_time = -1;
        processes[i]->wait_time = 0;
        processes[i]->turnaround_time = 0;
    }
}

void NovelScheduler(struct Process* processes[]) {
    int completedProcesses = 0;
    int currentTime = 0;

    while (completedProcesses < MAX_PROC) {
        // Find runnable processes
        int runnable[MAX_PROC];
        int numRunnable = 0;

        for (int i = 0; i < MAX_PROC; i++) {
            if (processes[i]->remaining_burst_time > 0) {
                runnable[numRunnable++] = i;
            }
        }

        if (numRunnable > 0) {
            // Select a random runnable process
            int selected = runnable[rand() % numRunnable];
            struct Process* process = processes[selected];

            if (process->response_time == -1) {
                process->response_time = currentTime;
            }

            // Simulate execution
            int burst_time = 1 + rand() % process->remaining_burst_time;
            currentTime += burst_time;
            process->remaining_burst_time -= burst_time;

            if (process->remaining_burst_time == 0) {
                process->turnaround_time = currentTime;
                process->wait_time = process->turnaround_time - process->response_time;
                completedProcesses++;
            }
        } else {
            // If no runnable processes, advance time
            currentTime++;
        }
    }
}

int main() {
    struct Process* processes[MAX_PROC];
    generateProcesses(processes);

    NovelScheduler(processes);

    // Open the output file for writing
    FILE* outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        perror("Error opening the file");
        return 1;
    }

    // Print process details to the file
    fprintf(outputFile, "Process\tResponse Time\tWait Time\tTurnaround Time\n");
    int totalResponseTime = 0;
    int totalWaitTime = 0;
    int totalTurnaroundTime = 0;

    for (int i = 0; i < MAX_PROC; i++) {
        struct Process* process = processes[i];
        fprintf(outputFile, "P%d\t%d\t\t%d\t\t%d\n",
               process->id, process->response_time, process->wait_time, process->turnaround_time);

        totalResponseTime += process->response_time;
        totalWaitTime += process->wait_time;
        totalTurnaroundTime += process->turnaround_time;
    }

    // Calculate and print averages to the file
    float avgResponseTime = (float)totalResponseTime / MAX_PROC;
    float avgWaitTime = (float)totalWaitTime / MAX_PROC;
    float avgTurnaroundTime = (float)totalTurnaroundTime / MAX_PROC;

    fprintf(outputFile, "\nAverage Response Time: %.2f\n", avgResponseTime);
    fprintf(outputFile, "Average Wait Time: %.2f\n", avgWaitTime);
    fprintf(outputFile, "Average Turnaround Time: %.2f\n", avgTurnaroundTime);

    // Close the output file
    fclose(outputFile);

    printf("Output written to 'output.txt'.\n");

    return 0;
}
