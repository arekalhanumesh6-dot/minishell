#include"minishell.h"
char *get_command(char *input_string)
{
   
    char *command=calloc(20,sizeof(char));
    int i=0;
    while(input_string[i]!='\0')
    { 
        if(input_string[i]==' ' || input_string[i]=='\t')
        {
           
           return command;
        }
        command[i]=input_string[i];
        i++;
    }
     return command;
     
}
