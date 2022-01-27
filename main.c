#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED "\033[0;31m"
#define RESET "\e[0m"
#define TOK_DELIM " \t\r\n"


//loop to run the shell
void loop();

//read the user input
char * read_line();

// split the commmand 
char ** split_line(char *);

//exit program
int dash_exit(char **);

//command execute
int dash_execute(char **);


int main(){

loop();


}


char * read_line(){

    int buffsize = 1024;
    int position = 0;
    char * buffer = malloc(sizeof(char)*buffsize);
    int c;

    if(!buffer){
        fprintf(stderr,"%sdash: Allocation error%s\n",RED,RESET);
        exit(EXIT_FAILURE);
    }

    while(1){
        c = getchar();
        if(c==EOF || c == '\n'){
            //printf("\n");
            buffer[position] = '\0';
            return buffer;
        }
        else{
            buffer[position]=c;
        }
        position++;

        if(position>=buffsize){
            buffsize+=1024;
            buffer = realloc(buffer,buffsize);
        }
        if(!buffer){
            fprintf(stderr,"dash: Allocation error\n");
            exit(EXIT_FAILURE);
        }
    }
}

void loop()
{

char * line;
char ** args;
int status = 1;

do{
    printf("> ");
    line = read_line();
    args = split_line(line);
    status = dash_execute(args);
    free(line);
    free(args);
    
   
    
}while(status);



}



char ** split_line(char * line){
    int buffsize = 1024,position = 0;
    char ** tokens = malloc(buffsize*sizeof(char *));
    char * token;

    if(!tokens){
        fprintf(stderr,"%sdash: Allocation error%s\n",RED,RESET);
        exit(EXIT_FAILURE);
    }

    token = strtok(line,TOK_DELIM);
    while(token != NULL){
        tokens[position]=token;
        position++;
    }
    if(position>=buffsize){
        tokens = realloc(tokens,buffsize*sizeof(char *));

        if(!tokens){
            fprintf(stderr,"%sdash: Allocation error%s\n",RED,RESET);
            exit(EXIT_FAILURE);
        }

        token = strtok(NULL,TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;

}

int dash_exit(char ** args){
    return 0;
}

int dash_execute(char **args){
    pid_t cpid;
    int status;

    if(strcmp(args[0],"exit")==0){
        return dash_exit();
    }

    cpid = fork();

    if(cpid == 0)
    {
        if(execvp(args[0],args)<0)
        
       
            printf("dash: command not found: %s\n",args[0]);
            exit(EXIT_FAILURE);
    }
     else if(cpid<0)
        {
            printf(RED"Error forking"RESET"\n");
        }
        else{
            waitpid(cpid,& status, WUNTRACED);
        }
        return 1;
   

}