
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define SIZE (100 * 1024 * 1024)

int main(void)
{
    char *data = malloc(SIZE);

    if (data == NULL)
    {
        perror("malloc");
        return 1;
    }

    for (size_t i = 0; i < SIZE; i++)
        data[i] = 1;

    printf("Parent PID: %d\n", getpid());
    printf("Allocated and initialized 100 MB.\n");
    printf("Press Enter to perform fork()...");
    fflush(stdout);
    getchar();

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        free(data);
        return 1;
    }

    if (pid == 0)
    {
        printf("\nChild PID: %d\n", getpid());
        printf("Child has inherited the memory.\n");
        printf("Press Enter before modifying memory...");
        fflush(stdout);
        getchar();

        for (size_t i = 0; i < SIZE; i += 4096)
            data[i] = 2;

        printf("Child modified memory pages.\n");
        printf("Press Enter to exit child...");
        fflush(stdout);
        getchar();

        free(data);
        return 0;
    }

    printf("\nParent PID: %d\n", getpid());
    printf("Child PID: %d\n", pid);
    printf("Parent and child initially share physical pages using COW.\n");
    printf("Press Enter to let the child modify memory...");
    fflush(stdout);
    getchar();

    waitpid(pid, NULL, 0);
    printf("Child has terminated.\n");

    free(data);
    return 0;
}

