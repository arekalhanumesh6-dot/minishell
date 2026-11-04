#include "minishell.h"

extern int pid;

void fore_ground(void);
void print_list(void);
void back_ground();
int last_status = 0;

void execute_pipeline(char *input_string)
{
    char buff[200];

    strcpy(buff, input_string);

    char *command[20];

    int command_count = 0;

    char *token = strtok(buff, "|");

    while (token != NULL)
    {
        command[command_count] = token;

        command_count++;

        token = strtok(NULL, "|");
    }

    int pipe_fd[20][2];

    pid_t pid[20];

    int i;

    for (i = 0; i < command_count - 1; i++)
    {
        if (pipe(pipe_fd[i]) == -1)
        {
            perror("pipe");

            return;
        }
    }
    // system("stty icanon isig");

    for (i = 0; i < command_count; i++)
    {
        pid[i] = fork();

        if (pid[i] < 0)
        {
            perror("fork");

            return;
        }

        if (pid[i] == 0)
        {
            char *argv[50];

            int argc = 0;

            char *word = strtok(command[i], " \t");

            while (word != NULL)
            {
                argv[argc] = word;

                argc++;

                word = strtok(NULL, " \t");
            }

            argv[argc] = NULL;

            if (i > 0)
            {
                dup2(pipe_fd[i - 1][0], STDIN_FILENO);
            }

            if (i < command_count - 1)
            {
                dup2(pipe_fd[i][1], STDOUT_FILENO);
            }
            int j;
            for (j = 0; j < command_count - 1; j++)
            {
                close(pipe_fd[j][0]);
                close(pipe_fd[j][1]);
            }

            signal(SIGINT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);
            execvp(argv[0], argv);
            perror("execvp");
            exit(1);
        }
    }
    for (i = 0; i < command_count - 1; i++)
    {
        close(pipe_fd[i][0]);
        close(pipe_fd[i][1]);
    }
    for (i = 0; i < command_count; i++)
    {
        int status,exit_status;

        waitpid(pid[i], &status, WUNTRACED);

        if (WIFEXITED(status))
        {
            exit_status = WEXITSTATUS(status);
        }
        else if (WIFSIGNALED(status))
        {
            exit_status = 128 + WTERMSIG(status);
        }
    }
   
}
void execute_external_commands(char *input_string)
{
    if(strrchr(input_string,'|')!=NULL)
    {
         execute_pipeline(input_string);
         return;
    }
   
    char *cmd_opt[strlen(input_string)+1]; 
    char buf[30];
    int j=0,k=0;

    for(int i=0;input_string[i]!='\0';i++)
    { 
        if(input_string[i]!='\n' && input_string[i]!=' ' && input_string[i]!='\t')
       {
            buf[k++]=input_string[i];
        
       }
        else if(input_string[i]==' ' || input_string[i]=='\t')
        { 
            buf[k]='\0';
           
            cmd_opt[j]=malloc(strlen(buf)+1);
            if(cmd_opt[j]==NULL)
            {
                printf("Memory Allocation Failed\n");
                return;
            }
            strcpy(cmd_opt[j],buf);
            memset(buf,'\0',30);
            j++;
            k=0; 
        }        
    }
    
    if(k>0)
    {
         buf[k]='\0';
         cmd_opt[j]=malloc(strlen(buf)+1);
        strcpy(cmd_opt[j++],buf);
        cmd_opt[j]=NULL;
    }
    else
    {
        cmd_opt[j]=NULL;
    }

     int p_pid=fork();//child creating
    if(p_pid>0)
    {
        int status;
        pid = p_pid;
        waitpid(p_pid,&status,WUNTRACED);
        if(WEXITSTATUS(status) || WIFSIGNALED(status))//WIFSIGNALED(status) ->its returns non_zero value when the child is terminated by signal
        {
            pid = 0;
        }
    }
    else
    {
        signal(SIGINT,SIG_DFL);
        signal(SIGTSTP,SIG_DFL);
        
        execvp(cmd_opt[0],cmd_opt);
        exit(5);//here commads are executing
    }

}

void execute_internal_commands(char *input_string)
{
    if(strcmp(input_string,"exit")==0)
    {
        exit(0);
    }
    else if(strcmp(input_string,"pwd")==0)
    {
        char path[50];
        getcwd(path,50);
        printf("%s\n",path);
    }
        else if (strcmp(input_string, "cd") == 0)
        {
            if (chdir(getenv("HOME")) == -1)
                perror("cd");
        }
        else if (strncmp(input_string, "cd ", 3) == 0)
        {
            if (chdir(input_string + 3) == -1)
                perror("cd");
        }
        else if(strcmp(input_string,"echo $$")==0)
            {
              printf("%d\n",getpid());   
            }
            else if(strcmp(input_string,"echo $?")==0)//echo $? is gives the 0 when the previus command succuss
            {
                   printf("%d\n", last_status);
            }  
            else if(strcmp(input_string,"echo $SHELL")==0)
            {
               char *shell_path=getenv("SHELL");
               if(shell_path!=NULL)
               {
                printf("%s\n",shell_path);
               }

            }
            else if(strcmp(input_string,"fg")==0)
            {
               
               fore_ground();
            }
            else if(strcmp(input_string,"bg")==0)
            {
              back_ground();
            }
            else if(strcmp(input_string,"jobs")==0)
            {
                 print_list();
            }
            
}
