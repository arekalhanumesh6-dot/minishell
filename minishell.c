#include<stdio.h>
#include "minishell.h"
char input_string[30];
char *external_commands[155];
char *command;
int pid;
char prompt[30]="minishell$";
void signal_handler(int signum)
{
    if(signum==SIGINT)
    {
        if(pid==0)
        {
            write(STDOUT_FILENO, "\n", 1);
             write(STDOUT_FILENO, prompt, strlen(prompt));
             write(STDOUT_FILENO, " ", 1);
        }
    }
    if(signum==SIGTSTP)
    {
        if(pid == 0)
        {
            write(STDOUT_FILENO, "\n", 1);
            write(STDOUT_FILENO, prompt, strlen(prompt));
            write(STDOUT_FILENO, " ", 1);
         }
         else
         {
            insert_first(input_string,pid);

         }
    }
    if(signum==SIGCONT)
    {
       
        printf("%s\n",input_string);
    }
}
void scan_input(char *prompt, char *input_string)
{
    while(1)
    {
        printf("%s",prompt);
      memset(input_string,'\0',30);
        scanf("%[^\n]",input_string);
        
        getchar();
        if(strlen(input_string)==0)
        {
            continue;
        }
        
        if(strstr(input_string,"PS1="))
        {
          if((strrchr(input_string,'\t')!=NULL) || (strrchr(input_string,' ')!=NULL) || (strrchr(input_string,'$')==NULL))
          {
            char *dollar=strrchr(input_string,'$');
            if(dollar+1!=NULL)
            {
                 printf("Invalid Command\n");

            }

    
          }
             else
            {
                strcpy(prompt,input_string+4);
            }
        }
        else
        {
           command = get_command(input_string);
            int type=check_command_type(command);
            if(type==EXTERNAL)
            { 
                
                if(strstr(input_string,"echo"))
                {
                    if((strcmp(input_string+4," $$")==0) || (strcmp(input_string+4," $?")==0) || (strcmp(input_string+4," $SHELL")==0))
                    {
                       
                      execute_internal_commands(input_string); 
                      continue;  
                    }
                   
                }
                 execute_external_commands(input_string);
               
            }
            else  if(type==BUILTIN)
            {
                execute_internal_commands(input_string);
            }  
            else if(strcmp(input_string,"clear")==0)
            {
                execute_external_commands(input_string);
            }
            else
            {
                printf("Command not found\n");

            }
        }
    }
}

int main()
{

    
    system("clear");
   
    
    extract_external_commands(external_commands);
    signal(SIGINT,signal_handler);
    signal(SIGTSTP,signal_handler);
    signal(SIGCONT,signal_handler);
    scan_input(prompt, input_string);
}
