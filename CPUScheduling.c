#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define PROCESS_COUNT 5
#define COLUMN_COUNT 3
#define QUANTUM_TIME 10

int Process[PROCESS_COUNT][COLUMN_COUNT] = {
    {1, 20, 4},
    {2, 25, 2},
    {3, 25, 3},
    {4, 15, 3},
    {5, 10, 10}};

int **FCFSProcess;
int **SJFProcess;
int **PrioProcess;
int **RRProcess;
size_t RRProcessSize;

void print(int** printProcess, size_t columnCount)
{
	for (int i = 0; i < columnCount; i++) //Prints top set of lines of gantt chart
	{
		printf(" ");
		for (int j = 0; j < printProcess[i][1]; j++)
			printf("-");
	}
	printf("\n");

	for (int i = 0; i < columnCount; i++) //Print process number of gantt chart
	{
		printf("|");

		int spaceLen = printProcess[i][1] / 2 - 1; //findsthe amount of spaces for each time interval

		for (int j = 0; j < spaceLen; j++)
			printf(" ");
		printf("P%d", printProcess[i][0]); //prints process

		for (int j = 0; j < spaceLen; j++)
			printf(" ");

		if (printProcess[i][1] % 2 == 1) //for odd numbers of spaces print an extra space
			printf(" ");
	}
	printf("|\n");

	for (int i = 0; i < columnCount; i++) //Print lower set of lines of gantt chart
	{
		printf(" ");
		for (int j = 0; j < printProcess[i][1]; j++)
			printf("-");
	}
	printf("\n");

	int waitTime = 0, totalWaitTime = 0, totalTurnAroundTime = 0;
	for (int i = 0; i < PROCESS_COUNT; i++) //prints time of gantt chart and calculates totalWaitTime and totalTurnAroundTime
	{
		printf("%d", waitTime);

		waitTime += printProcess[i][1]; //increases wait time

		if (i < PROCESS_COUNT - 1)
			totalWaitTime += waitTime; //adds to totalWaitTime

		totalTurnAroundTime += waitTime; //adds to totalTurnAroundTime

		for (int j = 0; j < printProcess[i][1] - 1; j++) //Prints Sapces
			printf(" ");
	}
	if (columnCount > PROCESS_COUNT)//To Calculate times of RR search
		for (int i = PROCESS_COUNT; i < columnCount; i++) //prints time of gantt chart and calculates totalWaitTime and totalTurnAroundTime
		{
			printf("%d", waitTime);

			int currWait = 0;
			for (int lastProcessIndex = i - 1; printProcess[i][0] != printProcess[lastProcessIndex][0]; lastProcessIndex--) //finds the last time a process was run, stores as lastProcessIndex
				currWait += printProcess[lastProcessIndex][1];//Finds the amount of time between the current and last process

			totalWaitTime += currWait; //adds to totalWaitTime

			waitTime += printProcess[i][1]; //increases wait time

			totalTurnAroundTime += currWait + printProcess[i][1]; //adds to totalTurnAroundTime

			for (int j = 0; j < printProcess[i][1] - 1; j++) //Prints Sapces
				printf(" ");
		}
	printf("%d", waitTime);
	printf("\n\n");

	int averageWaitTime = totalWaitTime / PROCESS_COUNT;             //Calculates Average wait time
	int averageTurnAroundTime = totalTurnAroundTime / PROCESS_COUNT; //Calculates average Turn AroundTime

	printf("Total Waiting Time\t: %d", totalWaitTime); //prints times
	printf("\nAverage Waiting Time\t: %d", averageWaitTime);
	printf("\nTotal Turn Around Time\t: %d", totalTurnAroundTime);
	printf("\nAverage Turn Around Time: %d", averageTurnAroundTime);
};

void FCFS() //calcuates order of FCFS Scheduling Algorithm
{
	FCFSProcess = (int**)malloc(PROCESS_COUNT * sizeof(int*)); //Copies order of orginal process
	for (int i = 0; i < PROCESS_COUNT; i++)
	{
		FCFSProcess[i] = (int*)malloc((COLUMN_COUNT - 1) * sizeof(int));

		FCFSProcess[i][0] = Process[i][0];
		FCFSProcess[i][1] = Process[i][1];
	}
};

void SJF() //calcuates order of SJF Scheduling Algorithm
{
	SJFProcess = (int**)malloc(PROCESS_COUNT * sizeof(int*)); //Create SJFProcess and assigns all values to 0
	for (int i = 0; i < PROCESS_COUNT; i++)
	{
		SJFProcess[i] = (int*)malloc((COLUMN_COUNT - 1) * sizeof(int));

		SJFProcess[i][0] = SJFProcess[i][1] = 0;
	}

	int** tempProcess;
	tempProcess = (int**)malloc(PROCESS_COUNT * sizeof(int*)); //Copies order of orginal process
	for (int i = 0; i < PROCESS_COUNT; i++)
	{
		tempProcess[i] = (int*)malloc((COLUMN_COUNT - 1) * sizeof(int));

		tempProcess[i][0] = Process[i][0];
		tempProcess[i][1] = Process[i][1];
	}

	for (int i = 0; i < PROCESS_COUNT; i++)
	{
		for (int j = 0; j < PROCESS_COUNT; j++) //Finds shortest CPU burst time and assigns it to SJFProcess
			if ((SJFProcess[i][1] > tempProcess[j][1] || SJFProcess[i][1] == 0) && tempProcess[j][1] != -1)
			{
				SJFProcess[i][0] = j + 1;
				SJFProcess[i][1] = tempProcess[j][1];
			}

		tempProcess[SJFProcess[i][0] - 1][1] = -1; //Flags that the process has been used
	}

	for (int i = 0; i < PROCESS_COUNT; i++) //free temp process
		free(tempProcess[i]);
	free(tempProcess);
};

void Prio() //calcuates order of Priority Scheduling Algorithm
{
	PrioProcess = (int**)malloc(PROCESS_COUNT * sizeof(int*)); //Create PrioProcess and assigns all values to 0
	for (int i = 0; i < PROCESS_COUNT; i++)
	{
		PrioProcess[i] = (int*)malloc(COLUMN_COUNT * sizeof(int));

		PrioProcess[i][0] = PrioProcess[i][1] = PrioProcess[i][2] = 0;
	}

	int** tempProcess;
	tempProcess = (int**)malloc(PROCESS_COUNT * sizeof(int*)); //Copies order of orginal process
	for (int i = 0; i < PROCESS_COUNT; i++)
	{
		tempProcess[i] = (int*)malloc(COLUMN_COUNT * sizeof(int));

		tempProcess[i][0] = Process[i][0];
		tempProcess[i][1] = Process[i][1];
		tempProcess[i][2] = Process[i][2];
	}

	for (int i = 0; i < PROCESS_COUNT; i++)
	{
		for (int j = 0; j < PROCESS_COUNT; j++) //Finds the highest priority and assigns it to PrioProcess
			if ((PrioProcess[i][2] > tempProcess[j][2] || PrioProcess[i][2] == 0) && tempProcess[j][2] != -1)
			{
				PrioProcess[i][0] = j + 1;
				PrioProcess[i][2] = tempProcess[j][2];
			}

		PrioProcess[i][1] = tempProcess[PrioProcess[i][0] - 1][1];
		tempProcess[PrioProcess[i][0] - 1][2] = -1;
	}

	for (int i = 0; i < PROCESS_COUNT; i++) //free temp process
		free(tempProcess[i]);
	free(tempProcess);
};

void RR() //calcuates order of Round Robin Scheduling Algorithm
{
	RRProcessSize = PROCESS_COUNT;
	RRProcess = (int**)malloc(PROCESS_COUNT * sizeof(int*)); //Create RRProcess and assigns all values to 0
	for (int i = 0; i < PROCESS_COUNT; i++)
	{
		RRProcess[i] = (int*)malloc((COLUMN_COUNT - 1) * sizeof(int));
		RRProcess[i][0] = Process[i][0];
		RRProcess[i][1] = Process[i][1];
	}

	for (int i = 0; i < RRProcessSize; i++)
	{
		if (RRProcess[i][1] > QUANTUM_TIME)
		{
			//Adds one row to RRProcess
			RRProcessSize++;
			RRProcess = realloc(RRProcess, RRProcessSize * sizeof(int*));
			RRProcess[RRProcessSize - 1] = (int*)malloc((COLUMN_COUNT - 1) * sizeof(int));

			//Stores Process On New Row with 10 ms subtracted from the CPU Burst time and sets the current value of cpu burst time to 10
			RRProcess[RRProcessSize - 1][0] = RRProcess[i][0];
			RRProcess[RRProcessSize - 1][1] = RRProcess[i][1] - QUANTUM_TIME;
			RRProcess[i][1] = QUANTUM_TIME;
		}
	}
};

void terminateProcesses() //Deallocates all memory from Processes
{
	for (int i = 0; i < PROCESS_COUNT - 1; i++)
		free(FCFSProcess[i]);
	free(FCFSProcess);

	for (int i = 0; i < PROCESS_COUNT - 1; i++)
		free(SJFProcess[i]);
	free(SJFProcess);

	for (int i = 0; i < PROCESS_COUNT; i++)
		free(PrioProcess[i]);
	free(PrioProcess);

	for (int i = 0; i < RRProcessSize - 1; i++)
		free(RRProcess[i]);
	free(RRProcess);
};

int main()
{
    pthread_t FCFSThread, SJFThread, PrioThread, RRThread; //Creates Threads

    pthread_create(&FCFSThread, NULL, FCFS, NULL); //starts threads
    pthread_create(&SJFThread, NULL, SJF, NULL);
    pthread_create(&PrioThread, NULL, Prio, NULL);
    pthread_create(&RRThread, NULL, RR, NULL);
    pthread_join(FCFSThread, NULL); //joins threads
    pthread_join(SJFThread, NULL);
    pthread_join(PrioThread, NULL);
    pthread_join(RRThread, NULL);

    printf("\n\tFCFS Scheduling\n\n\tGANTT CHART\n\n"); //prints results
    print(FCFSProcess, PROCESS_COUNT);
    printf("\n\n\n\tSJF Scheduling\n\n\tGANTT CHART\n\n");
    print(SJFProcess, PROCESS_COUNT);
    printf("\n\n\n\tPriority Scheduling\n\n\tGANTT CHART\n\n");
    print(PrioProcess, PROCESS_COUNT);
    printf("\n\n\n\tRound Robin Scheduling\n\n\tGANTT CHART\n\n");
    print(RRProcess, RRProcessSize);

    terminateProcesses(); //Deallocates all memory from Processes
    return 0;
};