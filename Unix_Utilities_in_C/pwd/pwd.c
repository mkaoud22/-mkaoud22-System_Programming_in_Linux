

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


int main() {
  
 size_t size = 100; // Initial buffer size
    char* buff = (char*) malloc(size);

    if (buff == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        return -1;
    }

    while (1) {
        if (getcwd(buff, size) != NULL) {
            size_t string_len = strlen(buff);
            if (write(1, buff, string_len) < 0) {
                fprintf(stderr, "Can't write to stdout.\n");
                free(buff);
                return -1;
            }
          
	    if(write(1, "\n", 1) < 0) 
	    {
             fprintf(stderr, "Can't write to stdout.\n");
	     return -1;
	    }
            free(buff);
            return 0;          //success
        } else {
            if (errno == ERANGE) {                  //ERANGE : Longpath (pathsize > buffsize)
                size *= 2; // Double the buffer size
                char* temp = (char*) realloc(buff, size);
                if (temp == NULL) {
                    fprintf(stderr, "Memory reallocation error.\n");
                    free(buff);
                    return -1;
                }
                buff = temp;
            } else {
                fprintf(stderr, "getcwd error.\n");
                free(buff);
                return -1;
            }
        }
    }
}


    
    
    
    
    
    
    
    
    
    



