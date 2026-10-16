
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int fd;

    fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    printf("Before redirection\n");
    fflush(stdout);

    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("dup2");
        close(fd);
        return EXIT_FAILURE;
    }

    close(fd);

    printf("Hello from redirected standard output!\n");
    printf("This message is stored in output.txt\n");

    return 0;
}
