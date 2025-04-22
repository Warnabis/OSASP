#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

void print_env_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Ошибка открытия env файла");
        return;
    }

    printf("\n Окружение из файла %s:\n", filename);
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        char *value = getenv(line);
        printf("%s=%s\n", line, value ? value : "не определено");
    }

    fclose(file);
}

void print_env_from_envp(char *envp[]) {
    printf("\n Окружение, переданное через envp:\n");
    for (int i = 0; envp[i] != NULL; i++) {
        printf("%s\n", envp[i]);
    }
}

int main(int argc, char *argv[], char *envp[]) {
    printf("Дочерний процесс: %s (PID: %d, PPID: %d)\n", argv[0], getpid(), getppid());

    if (argc > 1) {
        print_env_from_file(argv[1]); 
    } else {
        print_env_from_envp(envp); 
    }

    return 0;
}
