
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>


#define COUNT 200

int main(int argc, char *argv[]) {
    // Write your code here
    // Do not write a main() function. Instead, deal with mv_main() as the main function of your program.

if(argc < 3)
{
     fprintf(stderr, "Invalid Arguments for move operation error.\n");
     return -1;
}



char buff[COUNT];

int num_read;

int exitingfile_fd= open(argv[1],O_RDONLY);
 if (exitingfile_fd < 0)
 {
  fprintf(stderr, "Failed to open source file.\n");
  close(exitingfile_fd); 
  exit(-1);
 }



int createfile_fd= open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (createfile_fd < 0) {
        fprintf(stderr, "Failed to open or create destination file.\n");
        close(createfile_fd); 
        exit(-3);
    }
        while ((num_read=read(exitingfile_fd,buff,COUNT)) > 0)
        {

        if(write(createfile_fd ,buff,num_read ) < 0)
                {
                close(exitingfile_fd);
                close(createfile_fd);
                fprintf(stderr, "Can't write in created file error.\n");
                exit(-4);
                }
        }
        
unlink(argv[1]);
close(createfile_fd);
close(exitingfile_fd);

return 0 ;

}

