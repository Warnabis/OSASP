#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <string.h>
#include <time.h>

#define QUEUE_SIZE 10
#define MAX_DATA_SIZE 256
#define MAX_PROCESSES 10

typedef struct {
    unsigned char type;
    unsigned short hash;
    unsigned char size;
    char data[((MAX_DATA_SIZE + 3) / 4) * 4];
} Message;

typedef struct {
    Message buffer[QUEUE_SIZE];
    int head;
    int tail;
    int count;
    int total_added;
    int total_removed;
} MessageQueue;

MessageQueue *queue;
int shmid;
int sem_id;
volatile sig_atomic_t running = 1;

pid_t producers[MAX_PROCESSES];
pid_t consumers[MAX_PROCESSES];
int producer_count = 0;
int consumer_count = 0;

void signal_handler(int sig) {
    running = 0;
}

unsigned short calculate_hash(Message *msg) {
    unsigned short hash = 0;
    for (int i = 0; i < msg->size; i++) {
        hash += msg->data[i];
    }
    return hash;
}

void producer() {
    srand(time(NULL) ^ getpid());
    while (running) {
        struct sembuf lock = {0, -1, 0};
        struct sembuf unlock = {0, 1, 0};
        struct sembuf full = {1, 1, 0};
        struct sembuf empty = {2, -1, 0};
        
        semop(sem_id, &empty, 1);
        semop(sem_id, &lock, 1);
        
        Message msg;
        msg.type = rand() % 256;
        msg.size = (rand() % 256) + 1;
        for (int i = 0; i < msg.size; i++) {
            msg.data[i] = rand() % 256;
        }
        msg.hash = calculate_hash(&msg);

        queue->buffer[queue->tail] = msg;
        queue->tail = (queue->tail + 1) % QUEUE_SIZE;
        queue->count++;
        queue->total_added++;
        
        printf("Producer %d: Added message %d, total added: %d\n", getpid(), msg.type, queue->total_added);
        
        semop(sem_id, &unlock, 1);
        semop(sem_id, &full, 1);
        sleep(1);
    }
}

void consumer() {
    while (running) {
        struct sembuf lock = {0, -1, 0};
        struct sembuf unlock = {0, 1, 0};
        struct sembuf full = {1, -1, 0};
        struct sembuf empty = {2, 1, 0};
        
        semop(sem_id, &full, 1);
        semop(sem_id, &lock, 1);
        
        Message msg = queue->buffer[queue->head];
        queue->head = (queue->head + 1) % QUEUE_SIZE;
        queue->count--;
        queue->total_removed++;
        
        semop(sem_id, &unlock, 1);
        semop(sem_id, &empty, 1);
        
        unsigned short computed_hash = calculate_hash(&msg);
        printf("Consumer %d: Removed message %d, Hash %s, total removed: %d\n", getpid(), msg.type, (computed_hash == msg.hash) ? "Valid" : "Invalid", queue->total_removed);
        
        sleep(2);
    }
}

void menu() {
    int choice;
    while (running) {
        printf("\nMenu:\n");
        printf("1 - Add producer\n");
        printf("2 - Add consumer\n");
        printf("3 - View status\n");
        printf("4 - Remove one producer\n");
        printf("5 - Remove one consumer\n");
        printf("6 - Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                if (producer_count < MAX_PROCESSES) {
                    producers[producer_count] = fork();
                    if (producers[producer_count] == 0) {
                        producer();
                        exit(0);
                    }
                    producer_count++;
                }
                break;
            case 2:
                if (consumer_count < MAX_PROCESSES) {
                    consumers[consumer_count] = fork();
                    if (consumers[consumer_count] == 0) {
                        consumer();
                        exit(0);
                    }
                    consumer_count++;
                }
                break;
            case 3:
                printf("Queue size: %d/%d\n", queue->count, QUEUE_SIZE);
                printf("Total added: %d, Total removed: %d\n", queue->total_added, queue->total_removed);
                break;
            case 4:
                if (producer_count > 0) {
                    kill(producers[--producer_count], SIGTERM);
                    printf("One producer removed.\n");
                } else {
                    printf("No producers to remove.\n");
                }
                break;
            case 5:
                if (consumer_count > 0) {
                    kill(consumers[--consumer_count], SIGTERM);
                    printf("One consumer removed.\n");
                } else {
                    printf("No consumers to remove.\n");
                }
                break;
            case 6:
                running = 0;
                for (int i = 0; i < producer_count; i++) kill(producers[i], SIGTERM);
                for (int i = 0; i < consumer_count; i++) kill(consumers[i], SIGTERM);
                producer_count = 0;
                consumer_count = 0;
                printf("All processes terminated.\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    }
}

int main() {
    signal(SIGINT, signal_handler);
    
    key_t key = ftok("queue", 65);
    shmid = shmget(key, sizeof(MessageQueue), 0666 | IPC_CREAT);
    queue = (MessageQueue *)shmat(shmid, NULL, 0);
    memset(queue, 0, sizeof(MessageQueue));
    
    sem_id = semget(key, 3, 0666 | IPC_CREAT);
    semctl(sem_id, 0, SETVAL, 1);
    semctl(sem_id, 1, SETVAL, 0);
    semctl(sem_id, 2, SETVAL, QUEUE_SIZE);
    
    menu();
    
    shmdt(queue);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);
    return 0;
}