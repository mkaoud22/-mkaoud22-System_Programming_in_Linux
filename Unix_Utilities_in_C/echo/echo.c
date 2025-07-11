

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define COUNT 100


int main(int argc, char *argv[]) {
  

        char * buff [COUNT];
        int counter ;
        size_t string_len=0;


        for(counter=1 ; counter < argc ; counter++)
        {

                buff[counter-1] = argv[counter];
                string_len = strlen(buff[counter-1]);
                if(write(1,buff[counter-1],string_len) < 0)
                {
                 printf("Can't write in the stdout.\n");
                 exit(-1);
                }
                if(counter+1 < argc){
                if( write(1, " ",1 ) < 0 )
		{
                 printf("Can't write in the stdout.\n");
                 exit(-1);
                }

	
		}    
        }

        if( write(1, " ",1 ) < 0 )
        {
          printf("Can't write in the stdout.\n");
          exit(-1);
        }

return 0 ;

}



