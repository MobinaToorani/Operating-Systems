#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <semaphore.h>

void logStart(char* tID);
void logFinish(char* tID);
void startClock();
long getCurrentTime();

time_t programClock;
sem_t even;
sem_t odd;
int remaining_odd = 0;
int remaining_even = 0;


typedef struct thread //represents a single thread, you can add more members if required
{
	char tid[4];//id of the thread as read from file
	unsigned int startTime;
	int state;
	pthread_t handle;
	int retVal;
	int check_odd; //if the id is even or odd
} Thread;

//you can add more functions here if required

int threadsLeft(Thread* threads, int threadCount);
int threadToStart(Thread* threads, int threadCount);
void* threadRun(void* t);//the thread function, the code executed by each thread
int readFile(char* fileName, Thread** threads);//function to read the file content and build array of threads
void determine_even_odd(Thread* threads, int threadCount); // determines which threads are even and which are odd

int main(int argc, char *argv[])
{
	if(argc<2)
	{
		printf("Input file name missing...exiting with error code -1\n");
		return -1;
	}

    //you can add some suitable code anywhere in main() if required

	Thread* threads = NULL;
	int threadCount = readFile(argv[1],&threads);
	
    sem_init(&even, 0, 0);
	sem_init(&odd, 0, 0);
	int i = 0;
	startClock();
    //classifying the threads as even or odd
	determine_even_odd(threads, threadCount);

	while(threadsLeft(threads, threadCount)>0)//put a suitable condition here to run your program
	{
    //you can add some suitable code anywhere in this loop if required
		
        if (threads[i].check_odd == 1)
			sem_post(&odd);
		else
			sem_post(&even);
		
		while((i=threadToStart(threads, threadCount))>-1)
		{
            //check how many odd threads remain
			remaining_odd = 0;
            for (int i = 0; i < threadCount; i++)
            {
                if (threads[i].state != -1 && threads[i].check_odd == 1)
                    remaining_odd++;
            }
            // check how many even threads remain
            remaining_even = 0;
            for (int j = 0; j < threadCount; j++)
            {
                if (threads[j].state != -1 && threads[j].check_odd == 0)
                    remaining_even++;
            }
			threads[i].state = 1;
			threads[i].retVal = pthread_create(&(threads[i].handle),NULL,threadRun,&threads[i]);
			
		}
        // if the thread that is meant to run is even and there are no even threads left, start the odd threads
        if (remaining_odd == 0)
			sem_post(&even);
		else if (remaining_even == 0)
			sem_post(&odd);
	}
	sem_destroy(&even);
	sem_destroy(&odd);
	return 0;
}

int readFile(char* fileName, Thread** threads)//do not modify this method
{
	FILE *in = fopen(fileName, "r");
	if(!in)
	{
		printf("Child A: Error in opening input file...exiting with error code -1\n");
		return -1;
	}

	struct stat st;
	fstat(fileno(in), &st);
	char* fileContent = (char*)malloc(((int)st.st_size+1)* sizeof(char));
	fileContent[0]='\0';	
	while(!feof(in))
	{
		char line[100];
		if(fgets(line,100,in)!=NULL)
		{
			strncat(fileContent,line,strlen(line));
		}
	}
	fclose(in);

	char* command = NULL;
	int threadCount = 0;
	char* fileCopy = (char*)malloc((strlen(fileContent)+1)*sizeof(char));
	strcpy(fileCopy,fileContent);
	command = strtok(fileCopy,"\r\n");
	while(command!=NULL)
	{
		threadCount++;
		command = strtok(NULL,"\r\n");
	}
	*threads = (Thread*) malloc(sizeof(Thread)*threadCount);

	char* lines[threadCount];
	command = NULL;
	int i=0;
	command = strtok(fileContent,"\r\n");
	while(command!=NULL)
	{
		lines[i] = malloc(sizeof(command)*sizeof(char));
		strcpy(lines[i],command);
		i++;
		command = strtok(NULL,"\r\n");
	}

	for(int k=0; k<threadCount; k++)
	{
		char* token = NULL;
		int j = 0;
		token =  strtok(lines[k],";");
		while(token!=NULL)
		{
//if you have extended the Thread struct then here
//you can do initialization of those additional members
//or any other action on the Thread members
			(*threads)[k].state=0;
			if(j==0)
				strcpy((*threads)[k].tid,token);
			if(j==1)
				(*threads)[k].startTime=atoi(token);
			j++;
			token = strtok(NULL,";");
		}
	}
	return threadCount;
}

void logStart(char* tID)
{
	printf("[%ld] New Thread with ID %s is started.\n", getCurrentTime(), tID);
}

void logFinish(char* tID)
{
	printf("[%ld] Thread with ID %s is finished.\n", getCurrentTime(), tID);
}

void determine_even_odd(Thread* threads, int threadCount){
	
	for(int k=0; k<threadCount; k++)
	{
		int x = (int)threads[k].tid[strlen(threads[k].tid)-1];
		if(x % 2 != 0)
			threads[k].check_odd = 1;
		else 
			threads[k].check_odd = 0;
	}
}

int threadsLeft(Thread* threads, int threadCount)
{
	int remainingThreads = 0;
	for(int k=0; k<threadCount; k++)
	{
		if(threads[k].state>-1)
			remainingThreads++;
	}
	return remainingThreads;
}

int threadToStart(Thread* threads, int threadCount)
{
	for(int k=0; k<threadCount; k++)
	{
		if(threads[k].state == 0 && threads[k].startTime==getCurrentTime())
			return k;
	}
	return -1;
}

void* threadRun(void* t)//implement this function in a suitable way
{
	logStart(((Thread*)t)->tid);
	if (((Thread*)t)->check_odd == 1)
		sem_wait(&odd);
	else
		sem_wait(&even);
	printf("[%ld] Thread %s is in its critical section\n",getCurrentTime(), ((Thread*)t)->tid);
	if (((Thread*)t)->check_odd == 0)
		sem_post(&odd);
	else
		sem_post(&even);

	logFinish(((Thread*)t)->tid);
	((Thread*)t)->state = -1;
	pthread_exit(0);
}

void startClock()
{
	programClock = time(NULL);
}

long getCurrentTime()//invoke this method whenever you want check how much time units passed
//since you invoked startClock()
{
	time_t now;
	now = time(NULL);
	return now-programClock;
}