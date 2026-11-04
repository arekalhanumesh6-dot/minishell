#include "minishell.h"
void extract_external_commands(char **external_commands)
{
    int fd=open("ext_cmds.txt",O_RDONLY);
    if (fd == -1)
    {
       perror("open");
        return;
    }
    int i=0,j=0;
    char buf[30]={},ch;
    while( read(fd,&ch,1) > 0)
    {
        if(ch != '\n')
        {
            buf[i++]=ch;
        }
        else if(ch == '\r')
        {
            i++;
            continue;
        }
        if(ch=='\n')
        {
            buf[i]='\0';
            external_commands[j]=malloc(strlen(buf)+1);
            if(external_commands[j] == NULL)
            {
                printf("Memory Allocation Failed\n");
                return;
            }
            strcpy(external_commands[j],buf); 
            memset(buf,0,30);
            j++;
             i=0;
        }

    }
    close(fd);
}
