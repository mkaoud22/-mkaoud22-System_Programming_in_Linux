#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define COUNT 100

int main(int argc, char *argv[]) {
    
    char buf[COUNT];
    int num_read = 0;

    if (argc != 3)
    {
        printf("Usage: %s source-file destination-file.\n", argv[0]);
        exit(-1);
    }

    int fd_source = open(argv[1], O_RDONLY);
    if (fd_source < 0)
    {
        printf("Could not open the source file. \n");
        exit(-2);
    }

    // Update destination file opening mode to include O_TRUNC for overwriting content
    int fd_destination = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd_destination < 0)
    {
        printf("Could not open the destination file. \n");
        exit(-3);
    }

    while ((num_read = read(fd_source, buf, COUNT)) > 0)
    {
        if (write(fd_destination, buf, num_read) < 0)
        {
            printf("Could not write in the destination file \n");
            exit(-4);
        }
    }

    close(fd_source);
    close(fd_destination);

    return 0;
}

