/*
    Q0: take a time to inspect code and its comment, try to understand what every line does.
    Q1: complete TODOs
    Q2: build & run pipe.c, what does parent process do? what does child process do?
    Q3: does this IPC approach currently needs any level of synchronization? if yes, why? if no, when will it be needed?
    Q3: how would you share an int variables instead of strings using pipes?
*/

#include <stdio.h> //printf
#include <stdlib.h> //exit
#include <unistd.h> //fork, _exit
#include <sys/types.h> //pid_t
#include <sys/wait.h> //wait
#include <string.h> //strlen

pid_t cpid;
int pipefd[2];

void create_pipe();
void parent_send();
void child_print();

int main()
{
    create_pipe();

    cpid = fork();
    if (cpid == -1) //error, couldnt fork
    {
        perror("fork() failed"); //see ref: https://en.cppreference.com/w/c/io/perror
        exit(EXIT_FAILURE);
    }

    if (cpid == 0)
    {
        //we are in child process
        printf("hello from child, my pid is=%1$d (%1$X)\n", getpid());
        child_print();
    }
    else
    {
        //we are in parent process,
        printf("hello from parent, my pid is=%1$d (%1$X)\n", getpid());
        parent_send();
    }

    return 0;
}

void create_pipe()
{
    if(pipe(pipefd) < 0)
    {
        perror("pipe() failed");
        exit(EXIT_FAILURE);
    }
}

//this function will be running in parent process
void parent_send()
{
    close(pipefd[0]); //we wont be reading from pipe, so close reading endpoint

    char sentences[5][200];
    strcpy(sentences[0], "Lorem ipsum dolor sit amet, consectetur adipiscing elit. ");
    strcpy(sentences[1], "Duis id lorem ac mauris pulvinar varius semper in turpis. ");
    strcpy(sentences[2], "Maecenas id neque vel augue semper iaculis. ");
    strcpy(sentences[3], "Phasellus a risus facilisis, laoreet tellus in, pellentesque arcu. ");
    strcpy(sentences[4], "Proin dignissim odio quis tortor aliquet, sed efficitur tortor ullamcorper.");

    for (int i = 0; i < 5; i++)
    {
        write(pipefd[1], sentences[i], strlen(sentences[i])); //write to pipe (like writing to file). notice that we are using pipefd[1] to write 
    }

    close(pipefd[1]); //we are done writing, close pipe (this will send EOF to child so child knows there is nothing more to read)

    wait(NULL); //wait for child process to exit
    exit(EXIT_SUCCESS);
}

//this function will be running in child process which created after calling fork
void child_print()
{
    char tmp;
    int nbytes;
    close(pipefd[1]); //we wont be writing to pipe, so close writing endpoint

    //TODO: read from pipe char by char and print to console (just use printf("%c", ..) dont use any newlines and/or formatting)


    printf("\n");
    _exit(EXIT_SUCCESS); //use _exit instead of exit to terminate child
}