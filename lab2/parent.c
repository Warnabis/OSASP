#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ENV_FILE "env"
#define MAX_CHILDREN 100

int env_cmp(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

char **load_env(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Ошибка открытия env файла");
        return NULL;
    }

    char **new_env = malloc(20 * sizeof(char *));
    size_t count = 0;
    char line[256];

    while (fgets(line, sizeof(line), file) && count < 20) {
        line[strcspn(line, "\n")] = 0; 
        char *value = getenv(line); 
        if (value) {
            asprintf(&new_env[count], "%s=%s", line, value);
            count++;
        }
    }

    fclose(file);
    new_env[count] = NULL; 
    return new_env;
}

int main(int argc, char *argv[], char *envp[]) {
    printf("Родительский процесс PID: %d\n", getpid());

    size_t env_count = 0;
    while (envp[env_count]) env_count++;

    char **sorted_env = malloc(env_count * sizeof(char *));
    memcpy(sorted_env, envp, env_count * sizeof(char *));
    qsort(sorted_env, env_count, sizeof(char *), env_cmp);

    printf("\n Окружение родительского процесса:\n");
    for (size_t i = 0; i < env_count; i++) {
        printf("%s\n", sorted_env[i]);
    }
    free(sorted_env);

    char **child_env = load_env(ENV_FILE);
    if (!child_env) exit(EXIT_FAILURE);

    char *child_path = getenv("CHILD_PATH");
    if (!child_path) {
        fprintf(stderr, "Переменная CHILD_PATH не установлена\n");
        exit(EXIT_FAILURE);
    }

    char child_exec[256];
    snprintf(child_exec, sizeof(child_exec), "%s/child", child_path);

    int child_counter = 0;
    char command;
    printf("\n Введите команду (+ - создать процесс с env, * - создать процесс с envp, q - выход): ");

    while (1) {
        command = getchar();
        while (getchar() != '\n'); 

        if (command == '+') {
            if (child_counter >= MAX_CHILDREN) {
                printf("Достигнуто максимальное количество дочерних процессов!\n");
                continue;
            }

            pid_t pid = fork();
            if (pid == 0) { 
                char child_name[20];
                snprintf(child_name, sizeof(child_name), "child_%02d", child_counter);
                char *args[] = {child_name, ENV_FILE, NULL};
                execve(child_exec, args, child_env);
                perror("Ошибка запуска дочернего процесса");
                exit(EXIT_FAILURE);
            } else if (pid > 0) { 
                child_counter++;
            } else {
                perror("Ошибка fork()");
            }
        } else if (command == '*') {
            if (child_counter >= MAX_CHILDREN) {
                printf("Достигнуто максимальное количество дочерних процессов!\n");
                continue;
            }

            pid_t pid = fork();
            if (pid == 0) { 
                char child_name[20];
                snprintf(child_name, sizeof(child_name), "child_%02d", child_counter);
                char *args[] = {child_name, NULL};
                execve(child_exec, args, envp);
                perror("Ошибка запуска дочернего процесса");
                exit(EXIT_FAILURE);
            } else if (pid > 0) {
                child_counter++;
            } else {
                perror("Ошибка fork()");
            }
        } else if (command == 'q') {
            printf("Завершение работы...\n");
            while (wait(NULL) > 0); 
            break;
        }
    }

    for (size_t i = 0; child_env[i]; i++) free(child_env[i]);
    free(child_env);
    return 0;
}
