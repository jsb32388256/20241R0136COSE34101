#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct 
{
    int PID;
    int cpuBurstTime;
    int ioBurstTime;
    int arrivalTime;
    int ioTiming;
    int priority;
} Process;

Process processes[5];

typedef struct
{
    Process processes[500000];
    int front, rear;
} Queue;

Queue readyQueue;
Queue waitingQueue;

int SJF_Swap()
{
    int shortJobIndex = readyQueue.front;
    Process memory;

    for (int i = readyQueue.front + 1; i < readyQueue.rear; i++)
    {
        if (readyQueue.processes[i].cpuBurstTime < readyQueue.processes[shortJobIndex].cpuBurstTime)
        {
            shortJobIndex = i;
        }
    }
    memory = readyQueue.processes[readyQueue.front];
    readyQueue.processes[readyQueue.front] = readyQueue.processes[shortJobIndex];
    readyQueue.processes[shortJobIndex] = memory;
}
int Priority_Swap()
{
    int highestPriorityIndex = readyQueue.front;
    Process memory;

    for (int i = readyQueue.front + 1; i < readyQueue.rear; i++)
    {
        if (readyQueue.processes[i].priority > readyQueue.processes[highestPriorityIndex].priority)
        {
            highestPriorityIndex = i;
        }
    }
    memory = readyQueue.processes[readyQueue.front];
    readyQueue.processes[readyQueue.front] = readyQueue.processes[highestPriorityIndex];
    readyQueue.processes[highestPriorityIndex] = memory;
}

void Create_Process()
{
    srand(0);

    for (int i = 0; i < 5; i++)
    {
        processes[i].PID = i + 2;
        processes[i].cpuBurstTime = rand() % 10 + 1;
        processes[i].ioBurstTime = rand() % 6;
        processes[i].arrivalTime = rand() % 10;
        processes[i].priority = rand() % 10;
        processes[i].ioTiming = rand() % (processes[i].cpuBurstTime + 1);
        printf("Process %d\n", i+1);
        printf("\n");
        printf("PID: %d\n", processes[i].PID);
        printf("CPU Burst Time: %d\n", processes[i].cpuBurstTime);
        printf("IO Burst Time: %d\n", processes[i].ioBurstTime);
        printf("Arrival Time: %d\n", processes[i].arrivalTime);
        printf("Priority: %d\n", processes[i].priority);
        printf("IO timing: %d\n", processes[i].cpuBurstTime-processes[i].ioTiming); 
        printf("\n");
    }
}

void Config()
{
    readyQueue.front = 0;
    readyQueue.rear = 0;

    waitingQueue.front = 0;
    waitingQueue.rear = 0;
}

int* FCFS()
{
    int clk = 0;
    int currentPID = 0; //idle
    int remainingProcess = 5;

    int time[1000];
    int cnt = 0;
    int processOrder[1000];

    int waitingTime[5];
    int turnaroundTime[5];
    float avgWaitingTime = 0;
    float avgTurnaroundTime = 0;

    Process fcfsProcesses[5];
    for (int i=0; i<5; i++) { fcfsProcesses[i] = processes[i]; }

    processOrder[cnt++] = currentPID;

    while (remainingProcess > 0) //if there's no remaining process, stop
    {
        
        for (int i=0; i<5; i++) // check arrival time
        {
            if (fcfsProcesses[i].arrivalTime == clk)
            {
                readyQueue.processes[readyQueue.rear++] = fcfsProcesses[i];
            }
        }

        if (currentPID == 0)
        {
            if (readyQueue.rear != readyQueue.front)
            {
                time[cnt-1] = clk;
                currentPID = readyQueue.processes[readyQueue.front++].PID; //run process
                waitingTime[currentPID-2] = clk - fcfsProcesses[currentPID-2].arrivalTime;
                processOrder[cnt++] = currentPID - 1;
            }
        }
        else if ((--fcfsProcesses[currentPID-2].cpuBurstTime) == 0) // terminated
            {
                time[cnt-1] = clk;
                remainingProcess--;

                turnaroundTime[currentPID-2] = clk - fcfsProcesses[currentPID-2].arrivalTime;

                if (readyQueue.rear != readyQueue.front)
                {
                    currentPID = readyQueue.processes[readyQueue.front++].PID; //run process
                    waitingTime[currentPID-2] = clk - fcfsProcesses[currentPID-2].arrivalTime;
                    processOrder[cnt++] = currentPID - 1;
                }
                else 
                { 
                    currentPID = 0; //idle
                    processOrder[cnt++] = currentPID;
                } 

            }        
        clk++;
    }

    printf("FCFS:\n");
    printf("\n");

    Display_Chart(time, processOrder, cnt);

    for (int i=0; i<5; i++) 
    {
        avgWaitingTime += waitingTime[i];
        avgTurnaroundTime += turnaroundTime[i];
    }

    avgWaitingTime = avgWaitingTime / 5;
    avgTurnaroundTime = avgTurnaroundTime / 5;

    printf("Average Waiting time - %f\n", avgWaitingTime);
    printf("Average Turnaround time - %f\n", avgTurnaroundTime);
    printf("\n");

    while(readyQueue.rear != readyQueue.front)
    {
        readyQueue.processes[readyQueue.front++].PID;
    }

    while(waitingQueue.rear != waitingQueue.front)
    {
        waitingQueue.processes[waitingQueue.front++].PID;
    }
    int avg[2];
    avg[0] = avgWaitingTime;
    avg[1] = avgTurnaroundTime;
    return avg;
}

int* Non_Preemptive_SJF()
{
    int clk = 0;
    int currentPID = 0; //idle
    int remainingProcess = 5;

    int time[1000];
    int cnt = 0;
    int processOrder[1000];

    int waitingTime[5];
    int turnaroundTime[5];
    float avgWaitingTime = 0;
    float avgTurnaroundTime = 0;

    Process sjfProcesses[5];
    for (int i=0; i<5; i++) { sjfProcesses[i] = processes[i]; }

    processOrder[cnt++] = currentPID;

    while (remainingProcess > 0) //if there's no remaining process, stop
    {
        
        for (int i=0; i<5; i++) // check arrival time
        {
            if (sjfProcesses[i].arrivalTime == clk)
            {
                readyQueue.processes[readyQueue.rear++] = sjfProcesses[i];
            }
        }

        if (currentPID == 0)
        {
            if (readyQueue.rear != readyQueue.front)
            {
                time[cnt-1] = clk;

                SJF_Swap();
                currentPID = readyQueue.processes[readyQueue.front++].PID; //run process

                waitingTime[currentPID-2] = clk - sjfProcesses[currentPID-2].arrivalTime;
                processOrder[cnt++] = currentPID - 1;
            }
        }
        else if ((--sjfProcesses[currentPID-2].cpuBurstTime) == 0) // terminated
            {
                time[cnt-1] = clk;
                remainingProcess--;

                turnaroundTime[currentPID-2] = clk - sjfProcesses[currentPID-2].arrivalTime;

                if (readyQueue.rear != readyQueue.front)
                {
                    SJF_Swap();
                    currentPID = readyQueue.processes[readyQueue.front++].PID; //run process

                    waitingTime[currentPID-2] = clk - sjfProcesses[currentPID-2].arrivalTime;
                    processOrder[cnt++] = currentPID - 1;
                }
                else 
                { 
                    currentPID = 0; //idle
                    processOrder[cnt++] = currentPID;
                } 

            }        
        clk++;
    }

    printf("Non-Preemptive SJF:\n");
    printf("\n");

    Display_Chart(time, processOrder, cnt);

    for (int i=0; i<5; i++) 
    {
        avgWaitingTime += waitingTime[i];
        avgTurnaroundTime += turnaroundTime[i];
    }

    avgWaitingTime = avgWaitingTime / 5;
    avgTurnaroundTime = avgTurnaroundTime / 5;

    printf("Average Waiting time - %f\n", avgWaitingTime);
    printf("Average Turnaround time - %f\n", avgTurnaroundTime);
    printf("\n");

    while(readyQueue.rear != readyQueue.front)
    {
        readyQueue.processes[readyQueue.front++].PID;
    }

    while(waitingQueue.rear != waitingQueue.front)
    {
        waitingQueue.processes[waitingQueue.front++].PID;
    }
    int avg[2];
    avg[0] = avgWaitingTime;
    avg[1] = avgTurnaroundTime;
    return avg;
}

 int* Non_Preemptive_priority()
{
    int clk = 0;
    int currentPID = 0; //idle
    int remainingProcess = 5;

    int time[1000];
    int cnt = 0;
    int processOrder[1000];

    int waitingTime[5];
    int turnaroundTime[5];
    float avgWaitingTime = 0;
    float avgTurnaroundTime = 0;

    Process priorityProcesses[5];
    for (int i=0; i<5; i++) { priorityProcesses[i] = processes[i]; }

    processOrder[cnt++] = currentPID;

    while (remainingProcess > 0) //if there's no remaining process, stop
    {
        
        for (int i=0; i<5; i++) // check arrival time
        {
            if (priorityProcesses[i].arrivalTime == clk)
            {
                readyQueue.processes[readyQueue.rear++] = priorityProcesses[i];
            }
        }

        if (currentPID == 0)
        {
            if (readyQueue.rear != readyQueue.front)
            {
                time[cnt-1] = clk;

                Priority_Swap();
                currentPID = readyQueue.processes[readyQueue.front++].PID; //run process

                waitingTime[currentPID-2] = clk - priorityProcesses[currentPID-2].arrivalTime;
                processOrder[cnt++] = currentPID - 1;
            }
        }
        else if ((--priorityProcesses[currentPID-2].cpuBurstTime) == 0) // terminated
            {
                time[cnt-1] = clk;
                remainingProcess--;

                turnaroundTime[currentPID-2] = clk - priorityProcesses[currentPID-2].arrivalTime;

                if (readyQueue.rear != readyQueue.front)
                {
                    Priority_Swap();
                    currentPID = readyQueue.processes[readyQueue.front++].PID; //run process

                    waitingTime[currentPID-2] = clk - priorityProcesses[currentPID-2].arrivalTime;
                    processOrder[cnt++] = currentPID - 1;
                }
                else 
                { 
                    currentPID = 0; //idle
                    processOrder[cnt++] = currentPID;
                } 

            }        
        clk++;
    }

    printf("Non-Preemptive Priority:\n");
    printf("\n");

    Display_Chart(time, processOrder, cnt);

    for (int i=0; i<5; i++) 
    {
        avgWaitingTime += waitingTime[i];
        avgTurnaroundTime += turnaroundTime[i];
    }

    avgWaitingTime = avgWaitingTime / 5;
    avgTurnaroundTime = avgTurnaroundTime / 5;

    printf("Average Waiting time - %f\n", avgWaitingTime);
    printf("Average Turnaround time - %f\n", avgTurnaroundTime);
    printf("\n");

    while(readyQueue.rear != readyQueue.front)
    {
        readyQueue.processes[readyQueue.front++].PID;
    }

    while(waitingQueue.rear != waitingQueue.front)
    {
        waitingQueue.processes[waitingQueue.front++].PID;
    }
    int avg[2];
    avg[0] = avgWaitingTime;
    avg[1] = avgTurnaroundTime;
    return avg;
}

void Display_Chart(int time[], int processOrder[], int cnt)
{
    for (int i = 0; i < cnt-1; i++)
    {
        if(processOrder[i]) { printf("|  P %d  ", processOrder[i]); }
        else { printf("| IDLE  "); }
    }
    printf("|\n");
    printf("0");
    for (int i = 0; i < cnt-1; i++)
    {
        printf("\t%d", time[i]);
    }
    printf("\n");
}
/*
void Preemptive_priority()
{
    int clk = 0;
    int currentPID = 0; //idle
    int remainingProcess = 5;

    int time[1000];
    int cnt = 0;
    int processOrder[1000];

    int waitingTime[5];
    int turnaroundTime[5];
    float avgWaitingTime = 0;
    float avgTurnaroundTime = 0;

    Process priorityProcesses[5];
    for (int i=0; i<5; i++) { priorityProcesses[i] = processes[i]; }

    processOrder[cnt++] = currentPID;

    while (remainingProcess > 0) //if there's no remaining process, stop
    {
        Priority_Swap();
        for (int i=0; i<5; i++) // check arrival time
        {
            if (priorityProcesses[i].arrivalTime == clk)
            {
                readyQueue.processes[readyQueue.rear++] = priorityProcesses[i];
            }
        }

        if (currentPID == 0)
        {
            if (readyQueue.rear != readyQueue.front)
            {
                time[cnt-1] = clk;
                currentPID = readyQueue.processes[readyQueue.front++].PID; //run process
                processOrder[cnt++] = currentPID - 1;
            }
        }
        else if ((--priorityProcesses[currentPID-2].cpuBurstTime) == 0) // terminated
            {
                time[cnt-1] = clk;
                remainingProcess--;
                waitingTime[currentPID-2] = clk - priorityProcesses[currentPID-2].arrivalTime - priorityProcesses[currentPID-2].cpuBurstTime;
                turnaroundTime[currentPID-2] = clk - priorityProcesses[currentPID-2].arrivalTime;

                if (readyQueue.rear != readyQueue.front)
                {
                    currentPID = readyQueue.processes[readyQueue.front++].PID; //run process

                    processOrder[cnt++] = currentPID - 1;
                }
                else 
                { 
                    currentPID = 0; //idle
                    processOrder[cnt++] = currentPID;
                } 

            }
        else if(readyQueue.rear != readyQueue.front)
        {
            if (readyQueue.processes[readyQueue.front].priority > priorityProcesses[currentPID-2].priority) 
            {
                time[cnt-1] = clk;
                readyQueue.processes[readyQueue.rear++] = priorityProcesses[currentPID-2];
                currentPID = readyQueue.processes[readyQueue.front++].PID; //run process
                processOrder[cnt++] = currentPID - 1;
            } 
        }     
        clk++;
    }

    printf("Preemptive Priority:\n");
    printf("\n");

    Display_Chart(time, processOrder, cnt);

    for (int i=0; i<5; i++) 
    {
        avgWaitingTime += waitingTime[i];
        avgTurnaroundTime += turnaroundTime[i];
    }

    avgWaitingTime = avgWaitingTime / 5;
    avgTurnaroundTime = avgTurnaroundTime / 5;

    printf("Average Waiting time - %f\n", avgWaitingTime);
    printf("Average Turnaround time - %f\n", avgTurnaroundTime);
    printf("\n");

    while(readyQueue.rear != readyQueue.front)
    {
        readyQueue.processes[readyQueue.front++].PID;
    }

    while(waitingQueue.rear != waitingQueue.front)
    {
        waitingQueue.processes[waitingQueue.front++].PID;
    }
}
*/

void Display_Chart(int time[], int processOrder[], int cnt)
{
    for (int i = 0; i < cnt-1; i++)
    {
        if(processOrder[i]) { printf("|  P %d  ", processOrder[i]); }
        else { printf("| IDLE  "); }
    }
    printf("|\n");
    printf("0");
    for (int i = 0; i < cnt-1; i++)
    {
        printf("\t%d", time[i]);
    }
    printf("\n");
}
/*
void Evaluation(int** avgs)
    {
        printf("%f\n", max(max(avgs[0][0], avgs[1][0]), avgs[2][0]));
        printf("%f\n", max(max(avgs[0][1], avgs[1][1]), avgs[2][1]));
    }
*/
int main()
{
    int* avgs[3];
    Config();
    Create_Process();
    avgs[0] = FCFS();
    avgs[1] = Non_Preemptive_SJF();
    avgs[2] = Non_Preemptive_priority();

    //Evaluation(avgs);
    //Preemptive_priority();
    
    return 0;
}
