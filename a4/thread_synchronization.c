#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_THREADS 1000

typedef struct thread_info {
    char thread_id[4];  // format "txy" where x and y are digits (0-9)
    int start_time;
} thread_info_t;

sem_t mutex, even_sem, odd_sem;
int next_even_id = 0, next_odd_id = 1, even_cnt = 0, odd_cnt = 0;

void *threadRun(void *arg) {
    thread_info_t *thread_info = (thread_info_t *) arg;

    // Wait until permitted to enter critical section
    if (thread_info->thread_id[2] % 2 == 0) {
        sem_wait(&even_sem);
        even_cnt--;
    } else {
        sem_wait(&odd_sem);
        odd_cnt--;
    }

    // Enter Critical Section
    sem_wait(&mutex);
    printf("[%d] Thread with ID %s is in its critical section\n", thread_info->start_time, thread_info->thread_id);
    sem_post(&mutex);

    // Exit Critical Section
    sem_wait(&mutex);
    printf("[%d] Thread with ID %s is finished.\n", thread_info->start_time, thread_info->thread_id);
    sem_post(&mutex);

    // Release Semaphore and Unlock next thread if possible
    if (thread_info->thread_id[2] % 2 == 0) {
        if (odd_cnt > 0)
            sem_post(&odd_sem);
        else
            sem_post(&even_sem);
    } else {
        if (even_cnt > 0)
            sem_post(&even_sem);
        else
            sem_post(&odd_sem);
    }

    free(arg);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    FILE *fp;
    char line[100];
    thread_info_t *thread_info[MAX_THREADS];
    int count = 0, i;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s input_file_name\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "Cannot open input file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    sem_init(&mutex, 0, 1);
    sem_init(&even_sem, 0, 0);
    sem_init(&odd_sem, 0, 0);

    while (fgets(line, sizeof(line), fp) != NULL) {
        thread_info_t *t_info = malloc(sizeof(thread_info_t));
        sscanf(line, "%d %s", &t_info->start_time, t_info->thread_id);
        thread_info[count++] = t_info;
    }

    fclose(fp);

    pthread_t tid[MAX_THREADS];
    for (i = 0; i < count; i++) {
        pthread_create(&tid[i], NULL, threadRun, (void *) thread_info[i]);
    }

    for (i = 0; i < count; i++) {
        pthread_join(tid[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&even_sem);
    sem_destroy(&odd_sem);

    return 0;
}
