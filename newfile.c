#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SliceTime 2
#define MAX_PROC 15

void generateProcessesIO(int bt[], int io_ct[], int at[], int rt[], int tb[], int st[]) {
    srand(time(0));

    for (int i = 0; i < MAX_PROC; i++) {
        bt[i] = 5 + rand() % 26;
        io_ct[i] = 10 + rand() % 11;

        at[i] = 0;
        rt[i] = -1;
        tb[i] = bt[i];
        st[i] = -1;

        sleep(1 + rand() % 5);
    }
}

int main() {
    int count = 0, rem_procs, wait_time = 0, turnaround_time = 0;
    float avg_wait_time, avg_turnaround_time, avg_response_time;

    int at[MAX_PROC];
    int bt[MAX_PROC];
    int tb[MAX_PROC];
    int rt[MAX_PROC];
    int io_ct[MAX_PROC];
    int st[MAX_PROC];

    rem_procs = MAX_PROC;

    generateProcessesIO(bt, io_ct, at, rt, tb, st);

    int i = 0, sum = 0;

    // Open a file for writing
    FILE *outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        perror("Error opening the file");
        return 1;
    }

    fprintf(outputFile, "Process\tBurst Time\tTurnaround Time\tWaiting Time\tResponse Time\n");

    printf("Process\tBurst Time\tTurnaround Time\tWaiting Time\tResponse Time\n");

    while (rem_procs != 0) {
        if (tb[i] <= SliceTime && tb[i] > 0) {
            sum += tb[i];
            tb[i] = 0;

            if (st[i] == -1)
                st[i] = sum;

            if (rt[i] == -1)
                rt[i] = st[i] - at[i];

            int turnaround = sum - at[i];
            int wait = turnaround - bt[i];

            fprintf(outputFile, "P%d\t%d\t\t%d\t\t%d\t\t%d\n", i + 1, bt[i], turnaround, wait, rt[i]);

            wait_time += wait;
            turnaround_time += turnaround;
            count = 0;
            rem_procs--;
        } else if (tb[i] > 0) {
            if (st[i] == -1)
                st[i] = sum;

            tb[i] -= SliceTime;
            sum += SliceTime;

            if (rt[i] == -1)
                rt[i] = st[i] - at[i];
        }

        if (i == MAX_PROC - 1) {
            i = 0;
        } else if (at[i + 1] <= sum) {
            i++;
        } else {
            i = 0;
        }
    }

    fclose(outputFile);

    avg_wait_time = (float)wait_time / MAX_PROC;
    avg_turnaround_time = (float)turnaround_time / MAX_PROC;

    avg_response_time = 0;

    for (i = 0; i < MAX_PROC; i++) {
        avg_response_time += rt[i];
    }

    avg_response_time /= MAX_PROC;

    // Open the file again to append average times
    outputFile = fopen("output.txt", "a");
    if (outputFile == NULL) {
        perror("Error opening the file for appending");
        return 1;
    }

    fprintf(outputFile, "\nAverage Waiting Time: %.2f", avg_wait_time);
    fprintf(outputFile, "\nAverage Response Time: %.2f", avg_response_time);
    fprintf(outputFile, "\nAverage Turnaround Time: %.2f\n", avg_turnaround_time);

    fclose(outputFile);

    printf("\nOutput written to 'output.txt'.\n");
    printf("\nAverage Waiting Time: %.2f", avg_wait_time);
    printf("\nAverage Response Time: %.2f\n", avg_response_time);
    printf("\nAverage Turnaround Time: %.2f\n", avg_turnaround_time);

    return 0;
}

