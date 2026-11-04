#include "minishell.h"

extern char *external_commands[155];

int check_command_type(char *command)
{
    for(int i=0;external_commands[i]!=NULL;i++)
    {
        if(strcmp(external_commands[i],command)==0)
        {
            //printf("external->%s\n",external_commands[i]);
            return EXTERNAL;
        }    
    
    }
    char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
						"set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
						"exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help","fg","bg","jobs", NULL};

    for(int i=0;builtins[i]!=NULL;i++)
    {
        
            if(strcmp(builtins[i],command)==0)
            {
           
                return BUILTIN;
            }
    }
    return NO_COMMAND;
}


