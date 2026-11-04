#include"minishell.h"

typedef struct minishell
{
    int pid;
    char name[30];
    struct minishell *link;

}list;
list *head =NULL;
void handler(int signum)
{
        wait(NULL);

    // if(signum==SIGCHLD)
    // {
    //     wait(NULL);
    // }
}
void insert_first(char *input_string,int pid)
{
     list *node=malloc(sizeof(list));
        
    
        node->pid=pid;
        strcpy(node->name,input_string);
        node->link=head;
        head=node;

}
void print_list()
{
    if(head==NULL)
    {
        printf("-bash: fg: current: no such job\n");

    }
    
    int i=0;
    list *temp = head;
    while(temp!=NULL)
    {
        printf("%d stoped \t %s\n",temp->pid,temp->name);i++;
       temp= temp->link;
    }
}
void delet_first()
{
    list *temp=head;
    head=head->link;
    free(temp);
}

void fore_ground()
{
    if(head==NULL)
    {
        printf("-bash: fg: current: no such job\n");

    }
    else 
    {
        printf("%s\n",head->name);
        int pid = head->pid;

        int status;
      
        if (kill(pid, SIGCONT) == -1)
        {
            perror("kill");
            return;
        }

         waitpid(pid,&status,WUNTRACED);

         delet_first();
        

    }
}

void back_ground()
{
    if(head == NULL)
    {
        printf("-bash: bg: current: no such job\n");
        return;
    }
    else
    {
        int pid = head->pid;
        
            int status;
            if(kill(pid, SIGCONT) == -1)
            {
                printf("kill");
                return;
            }
             printf("[%d] %s &\n", pid, head->name);
             signal( SIGCHLD,handler);
             
             delet_first();

    }
    
}